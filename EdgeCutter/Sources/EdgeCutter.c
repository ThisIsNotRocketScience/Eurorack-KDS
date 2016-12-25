#include "EdgeCutter.h"
#include <math.h>
#ifdef __cplusplus
extern "C"
{
#endif

	void EdgeCutter_Init(struct EdgeCutter_Envelope *Env)
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

	void SwitchToState(struct EdgeCutter_Envelope *Env, int newstate)
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
			Env->AttackProgress = 1;
			Env->DecayProgress = 1;
			Env->ReleaseProgress = 0;
			Env->ReleaseStart = Env->Current;
			Env->Gates[GATE_RELEASESTART] = GATE_COUNTDOWN;
			break;
		case ENVSTATE_DECAY:
			Env->AttackProgress = 1;
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
			Env->AttackProgress = 1;
			Env->DecayProgress = 1;
			Env->ReleaseProgress = 0;
			Env->StateLeds[7] = 255;
			Env->StateLeds[8] = 255;
			break;
		}
	}


	void EdgeCutter_Trigger(struct EdgeCutter_Envelope *Env, unsigned char N, struct EdgeCutter_Params *Params)
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

	void EdgeCutter_LoadSettings(struct EdgeCutter_Settings *settings, struct EdgeCutter_Params *params)
	{

	}

	void EdgeCutter_ValidateParams(struct EdgeCutter_Params *params)
	{

	}

	unsigned long EnvelopeRange(float V, float SR)
	{
		return (unsigned long)(64 * pow((float)((SR * 6) / 64.0), pow((float)V, 0.54f)));
	}


	float EnvelopeLength(int inp, int speed)
	{
		//int I = EnvelopeRange(0, 2000) - 64 + 1;
	//int I2 = EnvelopeRange(1, 2000) - 64 + 1;
		if (speed == 1)
		{
			//inp *= 10;
			inp += 255;
		}
		return EnvelopeRange(inp / 255.0, 2000) - 64 + 1;
	}

	float SustainLevel(int sus)
	{
		return sus * 1.0f / 255.0f;
	}


	int EdgeCutter_GetEnv(struct EdgeCutter_Envelope *Env, struct EdgeCutter_Params *Params)
	{
		for (int i = 0; i < 4; i++)
		{
			if (Env->Gates[i]) Env->Gates[i]--;
		}

		switch (Env->State)
		{
		case ENVSTATE_ATTACK:
		{
			Env->CurrentTarget = 1.0;
			float Delta = 1.0f / EnvelopeLength(Env->A, Params->speed);
			Env->Current += Delta;
			Env->AttackProgress = Env->Current / (1.0f - Env->AttackStart);
			if (Env->Current >= 1.0f)
			{
				Env->Current = 1.0f;
				SwitchToState(Env, ENVSTATE_DECAY);
			}
		}
		break;

		case ENVSTATE_DECAY:
		{
			float SusLev = SustainLevel(Env->S);
			Env->CurrentTarget = SusLev;

			float Delta = -(1.0f - SusLev) / EnvelopeLength(Env->D, Params->speed);
			Env->Current += Delta;

			Env->DecayProgress = 1 - ((Env->Current - SusLev) / (Env->DecayStart - SusLev));



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
			float SusLev = SustainLevel(Env->S);
			Env->CurrentTarget = SusLev;

			float Delta = (SustainLevel(Env->S) - Env->Current)*0.2f;
			Env->Current += Delta;

		}
		break;

		case ENVSTATE_RELEASE:
		{
			Env->CurrentTarget = 0;

			float Delta = -Env->ReleaseStart / EnvelopeLength(Env->R, Params->speed);
			Env->Current += Delta;
			Env->ReleaseProgress = 1.0f - ((Env->Current) / (Env->ReleaseStart));

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
					float DCurved = (Env->CurrentTarget - Env->CurvedOutput) * 0.1f / 255.0f * (256 - Env->A);
					Env->CurvedOutput += DCurved;
				}
				break;
			case ENVSTATE_SUSTAIN:
			case ENVSTATE_DECAY:
			{
				float DCurved = (Env->CurrentTarget - Env->CurvedOutput) * 0.1f / 255.0f * (256 - Env->D);
				Env->CurvedOutput += DCurved;
			}
			break;
			case ENVSTATE_IDLE:
			case ENVSTATE_RELEASE:
			{
				float DCurved = (Env->CurrentTarget - Env->CurvedOutput) * 0.1f / 255.0f * (256 - Env->R);
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
			float L1 = (Env->AttackProgress + Env->DecayProgress) * (7.0f / 2.0f);

			int idx = (int)floor(L1);
			float frac = L1 - (float)idx;

			if (idx < 7)Env->StateLeds[(idx + 1)] = (int)((frac)*255.0f);
			if (idx < 7)Env->StateLeds[(idx)] = (int)((1.0f - frac)*255.0f);

			if (Env->State == ENVSTATE_SUSTAIN)
			{
				Env->StateLeds[7] = 255;
				Env->StateLeds[8] = 255;
			}

			if (Env->State == ENVSTATE_RELEASE)
			{
				L1 = (Env->ReleaseProgress) * (5.0f) + 8;
				idx = (int)floor(L1);
				frac = L1 - (float)idx;

				if (idx < 12)Env->StateLeds[(idx + 1)] = (int)((frac)*255.0f);
				if (idx < 13)Env->StateLeds[(idx)] = (int)((1.0f - frac)*255.0f);

			}
		}
		return (int)(Env->Current * 2047.0f);
	}

#ifdef __cplusplus
}
#endif
