#include "EdgeCutter2.h"
#include <math.h>

#ifdef __cplusplus
extern "C"
{
#endif

	void EdgeCutter2_Init(struct EdgeCutter2_Envelope *Env)
	{
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

	static void SwitchToState(struct EdgeCutter2_Envelope *Env, int newstate)
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
			Env->AttackProgress = FIXED(1);
			Env->DecayProgress = FIXED(1);
			Env->ReleaseProgress = 0;
			Env->ReleaseStart = Env->Current;
			Env->Gates[GATE_RELEASESTART] = GATE_COUNTDOWN;
			break;

		case ENVSTATE_DECAY:
			Env->AttackProgress = FIXED(1);
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
			Env->AttackProgress = FIXED(1);
			Env->DecayProgress = FIXED(1);
			Env->ReleaseProgress = 0;
			Env->StateLeds[7] = 255;
			Env->StateLeds[8] = 255;
			break;
		}
	}

	void EdgeCutter2_Trigger(struct EdgeCutter2_Envelope *Env, unsigned char N, struct EdgeCutter2_Params *Params)
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
				//retrigger?
			}
		}
		else
		{
			if (Params->mode != ENVMODE_TRIGGER &&  Env->State != ENVSTATE_IDLE && Env->State != ENVSTATE_RELEASE)
			{
				if (Env->State == ENVSTATE_ATTACK)
				{

				}
				SwitchToState(Env, ENVSTATE_RELEASE);
			}
			Env->TriggerState = 0;
		}
	}

	void EdgeCutter2_LoadSettings(struct EdgeCutter2_Settings *settings, struct EdgeCutter2_Params *params)
	{

	}

	void EdgeCutter2_ValidateParams(struct EdgeCutter2_Params *params)
	{

	}

	static unsigned long EnvelopeRange(uint32_t V, int speed)
	{
		return 1 + (((speed?1:10) * V) >> 8);
	}

	static int32_t EnvelopeLength(int inp, int speed)
	{
		return 1 + (((speed ? 200 : 2000) * inp) >> 8);
	}

	static int32_t SustainLevel(int sus)
	{
		return (sus * FIXED(1)) >> 8;
	}

	int EdgeCutter2_GetEnv(struct EdgeCutter2_Envelope *Env, struct EdgeCutter2_Params *Params)
	{
		for (int i = 0; i < 4; i++)
		{
			if (Env->Gates[i]) Env->Gates[i]--;
		}

		switch (Env->State)
		{
			case ENVSTATE_ATTACK:
			{
				Env->CurrentTarget = FIXED(1);
				int L = EnvelopeLength(Env->A, Params->speed);;
				int32_t Delta = FIXED(1) / L;
				Env->Current += Delta;
				Env->AttackProgress = (Env->Current * FIXED(1)) / (FIXED(1) - Env->AttackStart);
				if (Env->Current >= FIXED(1))
				{
					Env->Current = FIXED(1);
					SwitchToState(Env, ENVSTATE_DECAY);
				}
			}
			break;

			case ENVSTATE_DECAY:
			{
				int32_t SusLev = SustainLevel(Env->S);
				Env->CurrentTarget = SusLev;

				int32_t Delta = -(FIXED(1) - SusLev) / EnvelopeLength(Env->D, Params->speed);
				Env->Current += Delta;
				if (Env->DecayStart > SusLev)
				{
					int32_t waytogo = Env->Current - SusLev;

					Env->DecayProgress = ((Env->Current - SusLev) * FIXED(1)) / (FIXED(1) - SusLev);
					Env->DecayProgress = FIXED(1) - Env->DecayProgress;
				}
				else
				{
					Env->DecayProgress = FIXED(1);
					Env->Current = SusLev;
				}
				if (Env->Current <= SusLev)
				{
					Env->Current = SusLev;
					if (Params->mode == ENVMODE_TRIGGER || Params->mode == ENVMODE_LOOP)
					{
						SwitchToState(Env, ENVSTATE_RELEASE);
					}
					else
					{
						SwitchToState(Env, ENVSTATE_SUSTAIN);
					}
				}
			}
			break;

			case ENVSTATE_SUSTAIN:
			{
				int32_t SusLev = SustainLevel(Env->S);
				Env->CurrentTarget = SusLev;

				int32_t Delta = (SustainLevel(Env->S) - Env->Current)/5;
				Env->Current += Delta;

			}
			break;

			case ENVSTATE_RELEASE:
			{
				Env->CurrentTarget = 0;

				int32_t Delta = -Env->ReleaseStart / EnvelopeLength(Env->R, Params->speed);
				Env->Current += Delta;
				Env->ReleaseProgress = (((Env->ReleaseStart - Env->Current)) * FIXED(1)) / (Env->ReleaseStart);

				if (Env->Current <= 0)
				{
					Env->Current = 0;

					if (Params->mode == ENVMODE_LOOP && Env->TriggerState)
					{
						SwitchToState(Env, ENVSTATE_ATTACK);
					}
					else
					{
						SwitchToState(Env, ENVSTATE_IDLE);
					}
				}
			}
			break;

			case ENVSTATE_IDLE:
				Env->CurrentTarget = 0;
			break;
		}

		switch (Env->State)
		{
			case ENVSTATE_ATTACK:
			{
				int32_t DCurved = (Env->CurrentTarget - Env->CurvedOutput) * (0.004 / ((1 + Env->A) / 255.0));
				Env->CurvedOutput += DCurved;
			}
			break;
			
			case ENVSTATE_SUSTAIN:
			case ENVSTATE_DECAY:
			{
				int32_t DCurved = (Env->CurrentTarget - Env->CurvedOutput) * (0.004 / ((1 + Env->D) / 255.0));
				Env->CurvedOutput += DCurved;
			}
			break;
			
			case ENVSTATE_IDLE:
			case ENVSTATE_RELEASE:
			{
				int32_t DCurved = (Env->CurrentTarget - Env->CurvedOutput) * (0.004 / ((1 + Env->R) / 255.0));
				Env->CurvedOutput += DCurved;
			}
			break;
		}

		for (int i = 0; i < 13; i++)
		{
			Env->StateLeds[i] = 0;
		}

		if (Env->State != ENVSTATE_IDLE)
		{
			int32_t L1 = (Env->AttackProgress * 7 + Env->DecayProgress * 7) / 2 ;
			int idx = L1 >> FIXEDBITS;
			uint8_t frac = (uint8_t)((L1 & FRACMASK) >> (FIXEDBITS-8));
			
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
				idx = (L1 >> FIXEDBITS) + 9;
				frac = (uint8_t)((L1 & FRACMASK) >> (FIXEDBITS - 8));

				if (idx < 12) Env->StateLeds[(idx + 1)] = frac;
				if (idx < 13) Env->StateLeds[(idx)] = ~frac;
			}
		}

		Env->LinearOutput = (Env->Current * 4095) / (FIXED(1));
		Env->CurvedOutput = (Env->Current * 4095) / (FIXED(1));
		return Env->LinearOutput;
	}

#ifdef __cplusplus
}
#endif
