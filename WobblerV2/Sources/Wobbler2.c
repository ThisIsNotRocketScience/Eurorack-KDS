#include "Wobbler2.h"
#include <Math.h>
#include "../../EurorackShared/EURORACKSHARED.H"

#define __max(a,b) ((a)>(b)?(a):(b))
#define __min(a,b) ((a)<(b)?(a):(b))

#define PI (float)(3.1415926535897932384626433832795)
#define TAU (float)(3.1415926535897932384626433832795*2.0000)

#ifdef __cplusplus
extern "C"
{
#endif

	unsigned long LERP9bit(unsigned long *V, int fade)
	{
		unsigned char frac = (fade << 3) & 0xff;
		//if (frac && (frac < 255)) frac += 1;
		int I = fade >> 5;
		return ((V[I] >> 8) *(255 - frac) + (V[I + 1] >> 8) * frac);
	}

	int32_t imul(int32_t a, int32_t b)
	{
		int64_t R = (int64_t)a * (int64_t)b;
		return (int32_t)(R >> (WOBBLER_FIXBITS));
	}

	int32_t idiv(int32_t a, int32_t b)
	{
		int64_t R = (int64_t)a;
		R <<= WOBBLER_FIXBITS;
		R /= (int64_t)b;
		return (int32_t)R;
	}

	void Wobbler2_RandomSeed(Wobbler2_RandomGen *R, unsigned int seed)
	{
		R->RandomMemory = (long)seed;
	}

	void Wobbler2_SyncPulse(Wobbler2_LFO_t *LFO)
	{
		int Delta = LFO->timesincesync;
		LFO->timesincesync = 0;
		if (Delta < 10) return;

		if (Delta > 30000)
		{
			LFO->syncindex = 0;
		}
		else
		{
			LFO->syncindex++;
		}
		if (LFO->syncindex > 2)
		{
			LFO->synctime[0] = LFO->synctime[1];
			LFO->synctime[1] = LFO->synctime[2];
			LFO->syncindex = 2;

			LFO->SyncedPeriodTime = (LFO->synctime[0] + LFO->synctime[1] + LFO->synctime[2]) / 3;
			LFO->SyncDP = 0xffffffff / LFO->SyncedPeriodTime;
		}
		LFO->synctime[LFO->syncindex] = Delta;
	}

	int Wobbler2_Rand(Wobbler2_RandomGen *R)
	{
		return (((R->RandomMemory = R->RandomMemory * 214013L + 2531011L) >> 16) & 0x7fff);
	}

	void Wobbler2_Init(Wobbler2_LFO_t *LFO)
	{
		LFO->Output = 0;
		LFO->OutputPhased = 0;
		LFO->Phase1 = 0;

		LFO->PhasedShift = 0;
		LFO->Gate[0] = 0;
		LFO->Gate[1] = 0;
		LFO->EnvelopeVal = 0;
		LFO->PhasedCountdown = 0;
		LFO->EnvelopeState = Wobbler2_IDLE;
		LFO->Speed = 10;
		LFO->LastPhasing = 0;
#ifdef INTPENDULUM
		Wobbler2_InitIntPendulum(&LFO->Pendulum, LFO);
#else
		Wobbler2_InitPendulum(&LFO->Pendulum, LFO);
#endif
		Wobbler2_RandomSeed(&LFO->SNH.random, 0);
		for (int i = 0; i < 32; i++)
		{
			LFO->SNH.segbuffer[i] = 0;
		}
		//Wobbler2_InitIntPendulum(&LFO->Pendulum);
	}

	void Wobbler2_Trigger(Wobbler2_LFO_t *LFO, unsigned char N, struct Wobbler2_Params *Params)
	{
		if (N == 0)
		{
			LFO->TriggerLed = 255;
			LFO->Phase1 = 0;
			LFO->PhasedShift = 0;
			Wobbler2_StartTwang(LFO);
#ifdef INTPENDULUM
			Wobbler2_InitIntPendulum(&LFO->Pendulum, LFO);
#else
			Wobbler2_InitPendulum(&LFO->Pendulum, LFO);

#endif
		}
	}

	void Wobbler2_LoadSettings(Wobbler2_Settings *settings, Wobbler2_Params *params)
	{

	}

	void Wobbler2_ValidateParams(Wobbler2_Params *params)
	{

	}

	unsigned long Wobbler2_LFORange(int32_t V, int32_t SR)
	{
		return  1 + ((V*SR * 64) >> 8);
		//	return (unsigned long)(64 * pow((int32_t)((SR * 6) / 64.0), pow((int32_t)V, 0.54f)));
	}

	unsigned long Wobbler2_LFORange2(int32_t V, int32_t SR)
	{
		return  1 + V*V * 8;
		//	return (unsigned long)(64 * pow((int32_t)((SR * 6) / 64.0), pow((int32_t)V, 0.54f)));
	}

	unsigned long Wobbler2_LFORange3(int32_t V, int32_t SR)
	{
		return  1 + ((V*SR) >> 13);
		//	return (unsigned long)(64 * pow((int32_t)((SR * 6) / 64.0), pow((int32_t)V, 0.54f)));
	}

	void Wobbler2_StartTwang(Wobbler2_LFO_t *LFO)
	{
		//LFO->EnvelopeVal = 0;
		LFO->EnvelopeState = Wobbler2_ATTACK;
	}

	void Wobbler2_SampleHold(Wobbler2_LFO_SNH_t *sh, Wobbler2_LFO_t *lfo, uint32_t phase, uint16_t modin, uint32_t phase2)
	{
		int newseg = ((phase >> 29)) & 7;
		//SetSVF(&sh->filt, 0x10 + (mod / (256)), 0x150+(mod/1024));
		modin = ((modin) / 16);
		modin = ~modin;

		uint16_t mod = (modin*modin) / (1 << 14);
		mod = (~mod) * 2;
		//mod = (mod * mod)/65536;
		//mod = mod / 2 ;

		if (newseg != sh->lastseg)
		{
			sh->lastseg = newseg;
			sh->segbuffer[newseg] = ((Wobbler2_Rand(&sh->random)) >> 1);
			sh->lastval1 = sh->segbuffer[newseg];
		}

		int newseg2 = ((((phase2) >> 29))) & 7;
		sh->lastval2 = sh->segbuffer[newseg2];

		uint32_t r1 = sh->store1 * mod;
		uint16_t m2 = (~mod);
		uint32_t r2 = (sh->lastval1) * m2;
		sh->store1 = (r1 / 65536) + (r2 / 65536);

		uint32_t r1a = sh->store2 * mod;
		uint32_t r2a = (sh->lastval2) * m2;
		sh->store2 = (r1a / 65536) + (r2a / 65536);


	}

	int Wobbler2_Twang(Wobbler2_LFO_t *LFO, uint32_t phase)
	{
		return (Sine(phase) >> 16) * (LFO->EnvelopeVal >> 8);
	}

#include "FreqLerp.h"


	
	unsigned long Wobbler2_MakeFreq(int input)
	{
		return LERP9bit(FreqLerp, input)*2;
		
	}

	int Wobbler2_Get(Wobbler2_LFO_t *LFO, Wobbler2_Params *Params)
	{
		LFO->timesincesync++;
#ifdef INTPENDULUM
		Wobbler2_UpdateIntPendulumSettings(&LFO->Pendulum, LFO);
#else
		Wobbler2_UpdatePendulumSettings(&LFO->Pendulum, LFO);
#endif
		if (LFO->timesincesync > 30000 || LFO->syncindex == 0)
		{
			LFO->extsync = 0;
		}
		else
		{
			LFO->extsync = 1;
		}

		if (LFO->Gate[0] > 0)
		{
			LFO->Gate[0]--;
		}

		if (LFO->Gate[1] > 0)
		{
			LFO->Gate[1]--;
		}
		if (LFO->TriggerLed > 0) LFO->TriggerLed--;
		if (LFO->EnvelopeState != Wobbler2_IDLE)
		{
			uint32_t A = 0;
			uint32_t R = Wobbler2_LFORange3(128 << 8, WOBBLERSAMPLERATE);
			if ((LFO->Mod >> 8) < 128)
			{
				R = 1 + (Wobbler2_LFORange3(LFO->Mod, WOBBLERSAMPLERATE));
			}
			else
			{
				A = 1 + (Wobbler2_LFORange3(LFO->Mod - (128 << 8), WOBBLERSAMPLERATE));
			}
			if (LFO->EnvelopeState == Wobbler2_ATTACK)
			{
				if (A == 0)
				{
					LFO->EnvelopeState = Wobbler2_RELEASE;
					LFO->EnvelopeVal = 1 << 24;
				}
				else
				{
					LFO->EnvelopeVal += ((1 << 24) - 1) / A;
					if (LFO->EnvelopeVal >= 1 << 24)
					{
						LFO->EnvelopeVal = 1 << 24;
						LFO->EnvelopeState = Wobbler2_RELEASE;
					}
				}
			}
			else
			{
				LFO->EnvelopeVal -= ((1 << 24) - 1) / R;
				if (LFO->EnvelopeVal <= 0)
				{
					LFO->EnvelopeState = Wobbler2_IDLE;
					LFO->EnvelopeVal = 0;
				}
			}
		}


		uint32_t DP = 0;
		if (LFO->extsync)
		{
			SteppedResult_t SpeedGrade;
			Wobbler2_GetSteppedResult(LFO->Speed, 11, &SpeedGrade);
#define S(x,y) ((x*65536)/y)
			int32_t Speeds[11] = { S(1,8),S(1,6),S(1,4),S(1,3), S(1,2),
									S(1,1),
									S(2,1),S(3,1),S(4,1),S(6,1),S(8,1) };

			DP = (LFO->SyncDP * GetInterpolatedResultInt(Speeds, &SpeedGrade)) >> 16;
		}
		else
		{
			DP = Wobbler2_MakeFreq(LFO->Speed);// Wobbler2_LFORange2(LFO->Speed << 2, 0);;
		}
		LFO->Phase1 += DP;

		uint32_t DP2 = LFO->Phasing * 0x100000;
		//DP2 <<= 24;
		LFO->Phase2 = LFO->Phase1 + DP2;

		LFO->PhasedShift += DP2 >> 8;



		if (LFO->Phase1 < LFO->OldPhase1)
		{
			LFO->Gate[1] = Wobbler2_GATECOUNTDOWN;
			//		if (LFO->PhasedCountdown > 0)
			//		{
			//			LFO->Gate[0] = Wobbler2_GATECOUNTDOWN;
			//		}
			//		LFO->PhasedCountdown = LFO->Phasing << 24;
		}

		//uint32_t last = LFO->PhasedCountdown;
		//LFO->PhasedCountdown -= __min(DP, LFO->PhasedCountdown);
	//	if (LFO->PhasedCountdown == 0 && last != 0)
		//{
			//LFO->Gate[0] = Wobbler2_GATECOUNTDOWN;
	//	}

		LFO->OldPhase1 = LFO->Phase1;
		//	LFO->OldPhase2 = LFO->Phase2;


#ifdef INTPENDULUM
		Wobbler2_DoublePendulumInt(&LFO->Pendulum);
#else
		Wobbler2_DoublePendulum(&LFO->Pendulum, 0.05f);
#endif
		Wobbler2_SampleHold(&LFO->SNH, LFO, LFO->Phase1, LFO->Mod, LFO->Phase1 - DP2);

		CalculateCompensation(&LFO->CompensationVals, LFO->Mod >> 8);
		//Shapes_t BSO, &LFO->BasicShapesB;
		LFO->OutputsNormal[0] = FillBasicShapes(LFO->Phase1, LFO->Mod >> 8, &LFO->BasicShapesA, &LFO->CompensationVals);
		LFO->OutputsNormal[1] = (BasicShapes(LFO->Phase1 + LFO->PhasedShift, LFO->Mod >> 8, &LFO->CompensationVals) + LFO->OutputsNormal[0]) / 2;
		LFO->OutputsNormal[2] = Wobbler2_Twang(LFO, LFO->Phase1);
		LFO->OutputsNormal[3] = LFO->Pendulum.As;// + 0x80000000;
		LFO->OutputsNormal[4] = (int32_t)(LFO->SNH.store1 << 16) - (1 << 29);

		LFO->OutputsPhased[0] = FillBasicShapes(LFO->Phase2, LFO->Mod >> 8, &LFO->BasicShapesB, &LFO->CompensationVals);
		LFO->OutputsPhased[1] = (BasicShapes(LFO->Phase2 + LFO->PhasedShift, LFO->Mod >> 8, &LFO->CompensationVals) + LFO->OutputsPhased[0]) / 2;
		LFO->OutputsPhased[2] = Wobbler2_Twang(LFO, LFO->Phase2);
		LFO->OutputsPhased[3] = LFO->Pendulum.Bs;// + 0x80000000;
		LFO->OutputsPhased[4] = (int32_t)(LFO->SNH.store2 << 16) - (1 << 29);


		if ((LFO->BasicShapesB.Sine > 0) && (LFO->OldPhase2 < 0))
		{
			LFO->Gate[0] = Wobbler2_GATECOUNTDOWN;
		}


		LFO->OldPhase2 = LFO->BasicShapesB.Sine;

		Wobbler2_GetSteppedResult(LFO->Shape, 4, &LFO->ShapeStepped);

		LFO->Output = GetInterpolatedResultInt(LFO->OutputsNormal, &LFO->ShapeStepped) / (0xffff * 4);
		LFO->OutputPhased = GetInterpolatedResultInt(LFO->OutputsPhased, &LFO->ShapeStepped) / (0xffff * 4);

		LFO->Output = (LFO->Output *LFO->Amount1) / (int)(1 << 14);
		LFO->OutputPhased = (LFO->OutputPhased *LFO->Amount2) / (int)(1 << 14);


		LFO->Output += 2048;// + (2540 - 2048);
		LFO->OutputPhased += 2048;// +(2540 - 2048);

		if (LFO->Output > 4095) LFO->Output = 4095; else if (LFO->Output < 0) LFO->Output = 0;
		if (LFO->OutputPhased > 4095) LFO->OutputPhased = 4095; else if (LFO->OutputPhased < 0) LFO->OutputPhased = 0;
		//if (LFO->Output > 1) LFO->Output = 1; else if (LFO->Output < -1) LFO->Output = -1;

		Wobbler2_DoLeds(LFO);
		LFO->LastPhasing = LFO->Phasing;
		return LFO->Output;
	}

#include "BasicShapeCompensation.h"

	void CalculateCompensation(ShapeCompensationVals_t *Comp, int mod)
	{
		Comp->min = LERP(BasicShapeLow, SHAPECOMPENSATIONCOUNT - 1, mod);
		Comp->mul = LERP(BasicShapeMult, SHAPECOMPENSATIONCOUNT - 1, mod);
	}

	void Wobbler2_DoLeds(Wobbler2_LFO_t *LFO)
	{

		for (int i = 0; i < 9; i++)
		{
			LFO->Led[0][i] = 0;
			LFO->Led[1][i] = 0;
		}

		int32_t LedIdxB = (LFO->Output * 8);
		int iLedIdxB = LedIdxB >> 12;
		int IdxB = ((LedIdxB - (iLedIdxB << 12))) >> 4;

		LFO->Led[0][(iLedIdxB + 9) % 9] = 255 - IdxB;
		LFO->Led[0][(iLedIdxB + 10) % 9] = IdxB;

		int32_t LedIdxA = (LFO->OutputPhased * 8);
		int iLedIdxA = LedIdxA >> 12;
		int IdxA = ((LedIdxA - (iLedIdxA << 12))) >> 4;

		LFO->Led[1][(iLedIdxA + 9) % 9] = 255 - IdxA;
		LFO->Led[1][(iLedIdxA + 10) % 9] = IdxA;
		for (int i = 0; i < 5; i++)
		{
			LFO->ModeLed[i] = 0;
		};
		LFO->ModeLed[LFO->ShapeStepped.index] = 255 - LFO->ShapeStepped.fractional;
		if (LFO->ShapeStepped.index < 4)
		{
			LFO->ModeLed[LFO->ShapeStepped.index + 1] = LFO->ShapeStepped.fractional;
		}
	}

	int32_t GetInterpolatedResultInt(int32_t *table, SteppedResult_t *inp)
	{
		unsigned char F = inp->fractional;
		unsigned char IF = ~inp->fractional;
		return ((table[inp->index] / 256) * IF) + ((table[inp->index + 1] / 256) * F);
	}

	void Wobbler2_GetSteppedResult(uint16_t param, uint8_t steps, SteppedResult_t *out)
	{
		//max(floor(x + 0.25), (x + 0.25 - floor(x + 0.25)) * 2 + floor(x + 0.25) - 1)

		uint32_t X = (param * steps) / 256;
		X += 64;

		int FloorX = X & 0xffffff00;
		int Aside = FloorX;
		int Bside = (X - (FloorX)) * 2 + (FloorX)-256;
		int M = (Aside > Bside) ? Aside : Bside;

		out->index = M >> 8;
		out->fractional = M & 0xff;

	}


#ifdef __cplusplus
}
#endif
