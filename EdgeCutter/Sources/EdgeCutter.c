#include "EdgeCutter.h"

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

}

void EdgeCutter_Trigger(struct EdgeCutter_Envelope *Env, unsigned char N, struct EdgeCutter_Params *Params )
{
	if (N > 0)
	{
		if (Env->TriggerState == 0)
		{
			Env->State = ENVSTATE_ATTACK;
			Env->TriggerState = 1;
		}
	}
	else
	{
		if (Params->mode != ENVMODE_TRIGGER &&  Env->State != ENVSTATE_IDLE && Env->State != ENVSTATE_RELEASE)
		{
			if (Env->State == ENVSTATE_ATTACK)
			{
				
			}
			Env->State = ENVSTATE_RELEASE;
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

float EnvelopeLength(int inp, int speed)
{
	if (speed == 1) inp *= 10;
	return 1 + inp * 1.0f;
}

float SustainLevel(int sus)
{
	return sus * 1.0f / 255.0f;
}

int EdgeCutter_GetEnv(struct EdgeCutter_Envelope *Env, struct EdgeCutter_Params *Params)
{

	switch (Env->State)
	{
	case ENVSTATE_ATTACK: 
		{
			Env->CurrentTarget = 1.0;
			float Delta = 1.0f / EnvelopeLength(Env->A, Params->speed);
			Env->Current += Delta;
			if (Env->Current >= 1.0f)
			{
				Env->Current = 1.0f;
				Env->State = ENVSTATE_DECAY;
			}
		}
		break;

	case ENVSTATE_DECAY:
		{
		float SusLev = SustainLevel(Env->S);
		Env->CurrentTarget = SusLev;

			float Delta = - (1.0f - SusLev) / EnvelopeLength(Env->D, Params->speed);
			Env->Current += Delta;
			if (Env->Current <= SusLev)
			{
				Env->Current = SusLev;
				if (Params->mode == ENVMODE_TRIGGER || Params->mode == ENVMODE_LOOP)
				{
					Env->State = ENVSTATE_RELEASE;
				}
				else
				{
					Env->State = ENVSTATE_SUSTAIN;					
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
 
			float Delta = -SustainLevel(Env->S) / EnvelopeLength(Env->R, Params->speed);
			Env->Current += Delta;

			if (Env->Current <= 0)
			{
				Env->Current = 0;

				if (Params->mode == ENVMODE_LOOP && Env->TriggerState)
				{
					Env->State = ENVSTATE_ATTACK;
					
				}
				else
				{
					Env->State = ENVSTATE_IDLE;								
				}
			}
		}
		break;
	}

	float DCurved = (Env->CurrentTarget - Env->CurvedOutput) * 0.1/255.0 * (255-Env->A);
	Env->CurvedOutput += DCurved;

	return (int)(Env->Current * 2047.0f);
}

#ifdef __cplusplus
}
#endif
