#include "ADSREnvelope.h"
#include <math.h>
#include <stdlib.h>

#define ENVFIXMAX ((1<<ENVFIXEDBITS)-1)
#define EXPTABLEBITS 8
#define EXPTABLELENGTH (1<<EXPTABLEBITS)

uint32_t exptable[__ENVTABLE_COUNT][EXPTABLELENGTH];

void ADSR_BuildTable()
{
	float End = -64.0 / 24.0;
	float Max = exp(End);
	float Min = exp(0);
	float iMax = 65535.0f / (Max-Min);
	float EndL = 40;
	float MaxL = log(EndL);
	float MinL = log(1);
	float iMaxL = 65535.0f / (MaxL - MinL);
	float lower = 100000;
	float upper = 0;
	for (int i = 0; i < EXPTABLELENGTH; i++)
	{
		exptable[ENVTABLE_EXP][i] = (exp((i * End) / (float)(EXPTABLELENGTH - 1)) - Min) * iMax ;		
		exptable[ENVTABLE_LOG][i] = (log(1 + (i * (EndL-1)) / (float)(EXPTABLELENGTH - 1)) - MinL) * iMaxL;
	};
};

__inline uint32_t FixedMac(uint32_t A, uint32_t B, uint32_t C) // res = A + B*C;
{
	uint64_t R = A;
	A += ((uint64_t)B) * ((uint64_t)C) >> ENVFIXEDBITS;
	return (uint32_t)A;
}

__inline uint32_t FixedScale(uint32_t A, uint32_t B)
{
	uint64_t R = A;
	R *= (int64_t)ENVFIXMAX;
	R /= (int64_t)B;
	return (int)R;

}

uint32_t GetExpTable(uint32_t inp,int table)
{
	int i1 = inp >> 24;
	int i2 = i1 + 1;
	if (i2 == EXPTABLELENGTH)
	{
		return exptable[table][EXPTABLELENGTH - 1] << (ENVFIXEDBITS - 16);
	}
	uint32_t fracmask = 0x00ffffff;
	uint32_t frac = (inp & fracmask);
	frac >>= EXPTABLEBITS;
	uint32_t ifrac = 0x10000 - frac;
	return (((exptable[table][i1] * ifrac)>>16)  + ((exptable[table][i2] * frac)>>16)) << (ENVFIXEDBITS-16);
}

void ADSR_Init(struct ADSR_Envelope_t *Env, int Mode, int Speed, int AttackCurve)
{
	Env->Mode = Mode;
	Env->AttackCurve = AttackCurve;
	Env->Speed = Speed;
	Env->TriggerState = 0;
	Env->LinearOutput = 0;
	Env->State = ENVSTATE_IDLE;
	Env->CurvedOutput = 0;
	Env->Current = 0;
	Env->AttackProgress = 0;
	Env->DecayProgress = 0;
	Env->ReleaseProgress = 0;
	for (int i = 0; i < 4; i++)
	{
		Env->Gates[i] = 0;
	}
}

static void SwitchToState(struct ADSR_Envelope_t *Env, int newstate)
{
	switch (Env->State)
	{
	case ENVSTATE_SUSTAIN:
		Env->StateLeds[7] = 0;
		Env->StateLeds[8] = 0;
		break;

	case ENVSTATE_ATTACK:
		Env->Gates[GATE_ATTACKEND] = GATE_COUNTDOWN;
		break;

	case ENVSTATE_DECAY:
		Env->Gates[GATE_DECAYEND] = GATE_COUNTDOWN;
		break;

	case ENVSTATE_RELEASE:
		Env->Gates[GATE_RELEASEEND] = GATE_COUNTDOWN;
		break;
	}

	Env->State = newstate;

	switch (Env->State)
	{
	case ENVSTATE_IDLE:
		Env->AttackProgress = 0;
		Env->DecayProgress = 0;
		Env->ReleaseProgress = 0;
		break;

	case ENVSTATE_RELEASE:
		Env->AttackProgress = ENVFIXED(1);
		Env->DecayProgress = ENVFIXED(1);
		Env->ReleaseProgress = 0;
		Env->ReleaseStart = Env->Current;
		Env->ReleaseStartCurved = Env->CurvedOutput;
		if (Env->ReleaseStart == 0) Env->State = ENVSTATE_IDLE;
		Env->Gates[GATE_RELEASESTART] = GATE_COUNTDOWN;
		break;

	case ENVSTATE_DECAY:
		Env->AttackProgress = ENVFIXED(1);
		Env->DecayProgress = 0;
		Env->ReleaseProgress = 0;
		Env->DecayStart = Env->Current;
		break;

	case ENVSTATE_ATTACK:
		Env->AttackProgress = 0;
		Env->DecayProgress = 0;
		Env->ReleaseProgress = 0;
		Env->AttackStart = Env->Current;
		break;

	case ENVSTATE_SUSTAIN:
		Env->AttackProgress = ENVFIXED(1);
		Env->DecayProgress = ENVFIXED(1);
		Env->ReleaseProgress = 0;
		Env->StateLeds[7] = 255;
		Env->StateLeds[8] = 255;
		break;
	}
}

void ADSR_Trigger(struct ADSR_Envelope_t *Env, unsigned char N)
{
	if (N > 0)
	{
		if (Env->TriggerState == 0)
		{
			SwitchToState(Env, ENVSTATE_ATTACK);
			Env->TriggerState = 1;
		}
		else
		{
			if (Env->Mode == ENVMODE_TRIGGER && Env->State != ENVSTATE_ATTACK)
			{
				SwitchToState(Env, ENVSTATE_ATTACK);

			}
			else
			{
				SwitchToState(Env, ENVSTATE_ATTACK);
			}
			//retrigger?
		}
	}
	else
	{
		if (Env->Mode != ENVMODE_TRIGGER &&  Env->State != ENVSTATE_IDLE && Env->State != ENVSTATE_RELEASE)
		{
			if (Env->State == ENVSTATE_ATTACK)
			{

			}
			SwitchToState(Env, ENVSTATE_RELEASE);
		}
		Env->TriggerState = 0;
	}
}


static unsigned long EnvelopeRange(uint32_t V, int speed)
{
	return 1 + (((speed ? 1 : 10) * V) >> 8);
}

static int32_t EnvelopeLength(int inp, int speed, int samplerate)
{
	float V = inp / 1024.0f;
	V = (V * 0.95f + 0.05f);
	int R = 1 + (int)((V * V * V) * samplerate * (speed ? 1 : 2));
	if (R < 2) R = 2;
	return R;
}

static int32_t SustainLevel(int sus)
{
	return (sus * ENVFIXED(1)) >> 10;
}

void ADSR_Update(struct ADSR_Envelope_t *Env, int SampleRate)
{
	Env->Aconv = EnvelopeLength(Env->A, Env->Speed, SampleRate);
	Env->Dconv = EnvelopeLength(Env->D, Env->Speed, SampleRate);
	Env->Sconv = SustainLevel(Env->S);
	Env->Rconv = EnvelopeLength(Env->R, Env->Speed, SampleRate);
}

int ADSR_Get(struct ADSR_Envelope_t *Env)
{
	for (int i = 0; i < 4; i++)
	{
		if (Env->Gates[i]) Env->Gates[i]--;
	}

	switch (Env->State)
	{
	case ENVSTATE_ATTACK:
	{
		Env->CurrentTarget = ENVFIXED(1);
		int L = Env->Aconv;
		int32_t Delta = ENVFIXED(1) / L;
		Env->Current += Delta;
		Env->AttackProgress = FixedScale((Env->Current - Env->AttackStart) , __max(1, (ENVFIXED(1) - Env->AttackStart)));



		if (Env->Current >= ENVFIXED(1))
		{
			Env->CurvedOutput = Env->Current = ENVFIXED(1);
			SwitchToState(Env, ENVSTATE_DECAY);
		}
		else
		{
			uint32_t AP = Env->AttackProgress;
			int M = ENVFIXMAX;
			if (AP > M)
			{
				AP = 0xffffffff;
			}
			else
			{
				AP <<= (32 - ENVFIXEDBITS);
			}
			uint32_t ExpT = GetExpTable(AP, Env->AttackCurve);

			
//			uint32_t ExpTM = ExpT * (ENVFIXED(1) - Env->AttackStart);
	//		uint32_t ExpTD = ExpTM / ENVFIXED(1);
		//	ExpTD += Env->AttackStart;
			Env->CurvedOutput = FixedMac(Env->AttackStart, ExpT, ENVFIXED(1) - Env->AttackStart);;
		}
	}
	break;

	case ENVSTATE_DECAY:
	{
		int32_t SusLev = Env->Sconv;
		Env->CurrentTarget = SusLev;

		int32_t Delta =  -(ENVFIXED(1) - SusLev) / Env->Dconv;
		Env->Current += Delta;
		if (Env->DecayStart > SusLev)
		{			
			
			Env->DecayProgress = FixedScale(Env->Current - SusLev, __max(1, ENVFIXED(1) - SusLev));
			Env->DecayProgress = ENVFIXED(1) - Env->DecayProgress;
		}
		else
		{
			Env->DecayProgress = ENVFIXED(1);
			Env->Current = SusLev;
		}
		if (Env->Current <= SusLev)
		{
			Env->Current = SusLev;
			Env->CurvedOutput = SusLev;
			if (Env->Mode == ENVMODE_TRIGGER || Env->Mode == ENVMODE_LOOP)
			{
				SwitchToState(Env, ENVSTATE_RELEASE);
			}
			else
			{
				SwitchToState(Env, ENVSTATE_SUSTAIN);
			}
		}
		else
		{
			uint32_t DP = Env->DecayProgress;
			if (DP > ENVFIXMAX ) DP = 0xffffffff; else DP <<= (32 - ENVFIXEDBITS);			
			uint32_t EXPT = ENVFIXMAX - GetExpTable(DP, ENVTABLE_EXP);
			uint32_t scaler = ENVFIXMAX - SusLev;

			Env->CurvedOutput = FixedMac(SusLev, EXPT, scaler);
//			uint32_t curved = (EXPT * scaler) / ENVFIXED(1);
	//		Env->CurvedOutput = SusLev + curved;
		}
	}
	break;

	case ENVSTATE_SUSTAIN:
	{
		int32_t SusLev = Env->Sconv;
		Env->CurrentTarget = SusLev;

		int32_t Delta = (Env->Sconv - Env->Current) / 5;
		Env->Current += Delta;
		Env->CurvedOutput = Env->Current;
	}
	break;

	case ENVSTATE_RELEASE:
	{
		Env->CurrentTarget = 0;

		int32_t Delta = -Env->ReleaseStart / Env->Rconv;
		if (Delta == 0) Delta = -1;
		Env->Current += Delta;
		Env->ReleaseProgress = FixedScale(Env->ReleaseStart - Env->Current, __max(1, (Env->ReleaseStart)));

		if (Env->Current <= 0)
		{
			Env->CurvedOutput  = Env->Current = 0;

			if (Env->Mode == ENVMODE_LOOP && Env->TriggerState)
			{
				SwitchToState(Env, ENVSTATE_ATTACK);
			}
			else
			{
				SwitchToState(Env, ENVSTATE_IDLE);
			}
		}
		else
		{
			uint32_t RP = Env->ReleaseProgress;
			if (RP > ENVFIXMAX) RP = 0xffffffff; else RP <<= (32 - ENVFIXEDBITS);

			uint32_t EXPT = ENVFIXMAX - GetExpTable(((uint32_t)RP), ENVTABLE_EXP) ;
			uint32_t scaler = Env->ReleaseStartCurved;
			uint32_t curved = FixedMac(0, EXPT, scaler);
			Env->CurvedOutput = curved;
		}		
	}
	break;

	case ENVSTATE_IDLE:
		Env->CurrentTarget = 0;
		break;
	}

	
	


	for (int i = 0; i < 13; i++)
	{
		Env->StateLeds[i] = 0;
	}

	if (Env->State != ENVSTATE_IDLE)
	{
		int32_t L1 = (Env->AttackProgress * 7 + Env->DecayProgress * 7) / 2;
		int idx = L1 >> ENVFIXEDBITS;
		uint8_t frac = (uint8_t)((L1 & ENVFRACMASK) >> (ENVFIXEDBITS - 8));

		if (idx >= 0)
		{
			if (idx <= 7) Env->StateLeds[(idx + 1)] = frac;
			if (idx < 7)  Env->StateLeds[(idx)] = ~frac;
		}

		if (Env->State == ENVSTATE_SUSTAIN)
		{
			Env->StateLeds[7] = 255;
			Env->StateLeds[8] = 255;
		}

		if (Env->State == ENVSTATE_RELEASE)
		{
			L1 = (Env->ReleaseProgress) * (4) + (0);
			idx = (L1 >> ENVFIXEDBITS) + 9;
			frac = (uint8_t)((L1 & ENVFRACMASK) >> (ENVFIXEDBITS - 8));

			if (idx < 12) Env->StateLeds[(idx + 1)] = frac;
			if (idx < 13) Env->StateLeds[(idx)] = ~frac;
		}
	}

	Env->LinearOutput = Env->Current;

	return Env->LinearOutput >> (ENVFIXEDBITS - 16);
}



int ADSR_GetCurved(struct ADSR_Envelope_t *Env)
{
	ADSR_Get(Env);
	return Env->CurvedOutput >> (ENVFIXEDBITS - 16);
}