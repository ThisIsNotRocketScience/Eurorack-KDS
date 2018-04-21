
#include "EdgeCutter2.h"



#include <math.h>
int lowpassenabled = 0;

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
		Env->CurrentCurved = 0;

		Env->AttackStart = 0;
		Env->DecayStart = 0;
		Env->AttackProgress = 0;
		Env->ReleaseProgress = 0;

		Env->CurvedReleaseStart = 0;
		Env->CurvedAttackStart = 0;
		Env->CurvedDecayStart = 0;
		Env->AttackProgress = 0;
		Env->DecayProgress = 0;
		Env->ReleaseProgress = 0;
		Env->ReleaseTime = 0;
		Env->AttackTime = 0;
		Env->DecayTime = 0;
		Env->runninglowpass = 0;
		Env->SampledVelocity = 0;
		Env->PreviouslySampledVelocity = 0;

		for (int i = 0; i < EDGECUTTER_VELOCITYSAMPLES; i++) Env->VelocitySample[i] = 0;


		for (int i = 0; i < 4; i++)
		{
			Env->Gates[i] = 0;
		}
	}

	int32_t EdgeCutter2_EnvTransferFunc(int32_t S)
	{
		return fix16_mul(S, fix16_mul(S, S));
	};

#define FIX(x)((int32_t)((x)*65536.0f))

	uint32_t DoCurve(int32_t from, int32_t to, uint32_t prog, uint16_t Curve, SteppedResult_t *curB, int32_t Linear)
	{


		prog = prog % 0x8000;
		int32_t delta = to - from;
		int32_t preres1;
		int32_t preres2;

		{
			int32_t lowerbound = FIX(0.8);
			int32_t upperbound = FIX(0.002);
			int32_t highval = EdgeCutter2_EnvTransferFunc(lowerbound);
			int32_t lowval = EdgeCutter2_EnvTransferFunc(upperbound);
			int32_t baseline = -lowval;
			int32_t range = fix16_sdiv(FIX(1.0), fix16_ssub(highval, lowval));
			int32_t scaled = fix16_add(fix16_mul(fix16_ssub(FIX(1.0), prog * 2), fix16_ssub(upperbound, lowerbound)), lowerbound);
			preres1 = fix16_mul(fix16_add(EdgeCutter2_EnvTransferFunc(scaled), baseline), range);
		}
		{
			int32_t upperbound = FIX(0.99);
			int32_t lowerbound = FIX(-0.000);
			int32_t highval = EdgeCutter2_EnvTransferFunc(lowerbound);
			int32_t lowval = EdgeCutter2_EnvTransferFunc(upperbound);
			int32_t baseline = -lowval;
			int32_t range = fix16_sdiv(FIX(1.0), fix16_ssub(highval, lowval));
			int32_t scaled = fix16_add(fix16_mul(fix16_ssub(FIX(1.0), prog * 2), fix16_ssub(upperbound, lowerbound)), lowerbound);
			preres2 = fix16_mul(fix16_add(EdgeCutter2_EnvTransferFunc(scaled), baseline), range);
		}
		int32_t Va[2] = { preres1,prog * 2 };
		int32_t Vb[2] = { prog * 2,preres2 };
		int32_t preres = preres1;
		if (curB->index == 1)
		{
			preres = LERP(Va, 1, curB->fractional);
		}
		else
		{
			if (curB->index == 2)
			{
				preres = LERP(Vb, 1, curB->fractional);
			}
			else
			{
				if (curB->index > 2)
				{
					preres = preres2;
				}
			}
		}

		uint32_t premul = (delta * preres / 2);
		return from + premul / FIXED(1);
	}

	uint32_t DoCurveAttack(int32_t from, int32_t to, uint32_t prog, uint16_t Curve, SteppedResult_t *cur, int32_t Linear)
	{
		uint32_t curve1 = FIXED(1) - DoCurve(from, to, prog, Curve, cur, Linear);

		if (cur->index == 0)
		{
			uint32_t prog2 = prog;

			int32_t mult = ((0xff - cur->fractional));
			prog2 = prog2 * mult;
			prog2 /= PEAKCOUNTSCALER;
			//	prog2 = fix16_mul(prog2, FIX(2.0/(6*4096.0)));
			uint32_t curve2 = FIXED(1) - DoCurve(from, to, prog2, Curve, cur, Linear);
			curve2 *= curve1;
			curve2 /= 32768;
			uint32_t L[2] = { curve1,curve2 };

			uint32_t res = ULERP(L, 1, 255 - cur->fractional);
			return res;
		}
		else
		{
			return curve1;
		}
	}

	uint32_t DoCurveDecay(int32_t from, int32_t to, uint32_t prog, uint16_t Curve, SteppedResult_t *cur, int32_t Linear)
	{
		uint32_t curve1 = DoCurve(from, to, prog, Curve, cur, Linear);
		uint32_t curveref = DoCurve(from, to, ~prog, Curve, cur, Linear);

		if (cur->index == 0)
		{
			uint32_t prog2 = prog;
			prog2 = 0x8000 - prog2;
			int32_t mult = ((0xff - cur->fractional));
			prog2 = prog2 * mult;
			prog2 /= PEAKCOUNTSCALER;
			prog2 = 0x8000 - (prog2 % 0x8000);
			//	prog2 = fix16_mul(prog2, FIX(2.0/(6*4096.0)));
			uint32_t curve2 = DoCurve(0, FIXED(1), prog2, Curve, cur, Linear);
			uint32_t peak = FIXED(1) - curve1;
			curve2 *= peak;
			curve2 /= FIXED(1);
			//curve2 = 0;
			curve2 += curve1;
			uint32_t L[2] = { curve1,curve2 };

			uint32_t res = ULERP(L, 1, 255 - cur->fractional);
			return res;
		}
		else
		{
			return curve1;
		}
	}

	uint32_t DoCurveRelease(int32_t from, int32_t to, uint32_t prog, uint16_t Curve, SteppedResult_t *cur, int32_t Linear)
	{
		uint32_t curve1 = DoCurve(from, to, prog, Curve, cur, Linear);
		uint32_t curveref = DoCurve(0, FIXED(0.5), prog, Curve, cur, Linear);

		if (cur->index >= 3)
		{
			uint32_t prog2 = prog;

			int32_t mult = (cur->fractional);
			if (cur->index > 3)mult = 256;
			prog2 = 0x8000 - prog2;
			prog2 = prog2 * mult;
			prog2 /= PEAKCOUNTSCALER;
			prog2 = 0x8000 - (prog2 % 0x8000);
			//	prog2 = fix16_mul(prog2, FIX(2.0/(6*4096.0)));
			uint32_t curve2 = DoCurve(from, to, prog2, Curve, cur, Linear);
			curve2 *= curveref;
			curve2 /= 32768 / 2;
			uint32_t L[2] = { curve1,curve2 };
			if (cur->index == 3)
			{
				uint32_t res = ULERP(L, 1, cur->fractional);
				return res;
			}
			else
			{
				return curve2;
			}
		}
		else
		{
			return curve1;
		}
	}

	static void SwitchToState(EdgeCutter2_Envelope *Env, int newstate)
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
			Env->CurvedReleaseStart = Env->CurrentCurved;
			Env->Gates[GATE_RELEASESTART] = GATE_COUNTDOWN;
			Env->ReleaseTime = 0;
			//if (Env->Current == 0)
			//{
		//		Env->CurrentCurved = 0;
		//		SwitchToState(Env,ENVSTATE_IDLE);
		//	}
			break;

		case ENVSTATE_DECAY:
			Env->AttackProgress = FIXED(1);
			Env->DecayProgress = 0;
			Env->ReleaseProgress = 0;
			Env->DecayStart = Env->Current;
			Env->CurvedDecayStart = Env->CurrentCurved;
			Env->DecayTime = 0;
			break;

		case ENVSTATE_ATTACK:
			Env->AttackProgress = 0;
			Env->DecayProgress = 0;
			Env->ReleaseProgress = 0;
			Env->AttackStart = Env->Current;
			Env->CurvedAttackStart = Env->CurrentCurved;
			Env->AttackTime = 0;
			Env->PreviouslySampledVelocity = Env->SampledVelocity;

			if (Env->AttackStart == FIXED(1))
			{
				SwitchToState(Env, ENVSTATE_DECAY);
			}

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

	void EdgeCutter2_Retrigger(struct EdgeCutter2_Envelope *Env)
	{
		if (Env->TriggerState == 1)
		{
			SwitchToState(Env, ENVSTATE_ATTACK);
			Env->VelocitySampleCountdown = 7;
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
				Env->VelocitySampleCountdown = 7;

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
		params->GatedMode = 0;

	}

	void EdgeCutter2_ValidateParams(struct EdgeCutter2_Params *params)
	{

	}

	static unsigned long EnvelopeRange(uint32_t V, int speed)
	{
		return 1 + (((speed ? 1 : 10) * V) >> 8);
	}

	static int32_t EnvelopeLength(int inp, int speed)
	{
		return 1 + (((speed ? 200 : 2000) * (inp >> 8)) >> 8);
	}

	static int32_t SustainLevel(int sus)
	{
		return ((sus >> 8)* FIXED(1)) >> 8;
	}

	int EdgeCutter2_GetEnv(EdgeCutter2_Envelope *Env, EdgeCutter2_Params *Params, EdgeCutter2_Calibration *Calibration)
	{
		SteppedResult_t CurveSteps;
		GetSteppedResult(Env->Curvature, 4, &CurveSteps);
		if (Env->VelocitySampleCountdown > 0)
		{
			Env->VelocitySampleCountdown--;
			Env->VelocitySample[Env->VelocitySampleCountdown] = Env->Velocity;

			for (int i = 0; i < EDGECUTTER_VELOCITYSAMPLES; i++) Env->SampledVelocity += Env->VelocitySample[i];
			Env->SampledVelocity /= EDGECUTTER_VELOCITYSAMPLES;
		}
		for (int i = 0; i < 4; i++)
		{
			if (Env->Gates[i]) Env->Gates[i]--;
		}
		if (Params->GatedMode > 0)
		{
			for (int i = 0; i < 4; i++)
			{
				Env->Gates[i] = 0;
			}
			switch (Env->State)
			{
			case ENVSTATE_ATTACK: Env->Gates[GATE_ATTACKEND] = 1; break;
			case ENVSTATE_DECAY:  Env->Gates[GATE_DECAYEND] = 1; break;
			case ENVSTATE_SUSTAIN: Env->Gates[GATE_RELEASESTART] = 1; break;
			case ENVSTATE_RELEASE: Env->Gates[GATE_RELEASEEND] = 1; break;
			}
		}

		switch (Env->State)
		{
		case ENVSTATE_ATTACK:
		{
			Env->CurrentTarget = FIXED(1);
			int L = EnvelopeLength(Env->A, Params->speed);;
			int32_t Delta = FIXED(1) / L;
			Env->Current += Delta;
			Env->AttackProgress = ((Env->Current - Env->AttackStart) * FIXED(1)) / (FIXED(1) - Env->AttackStart);

			Env->CurrentCurved = Env->CurvedAttackStart + DoCurveAttack(Env->CurvedAttackStart, FIXED(1), FIXED(1) - Env->AttackProgress, Env->Curvature, &CurveSteps, Env->Current);
			Env->VelocityCurrent = (Env->AttackProgress * (Env->SampledVelocity - Env->PreviouslySampledVelocity)) / FIXED(1) + Env->PreviouslySampledVelocity;
			if (Env->Current >= FIXED(1))
			{
				SwitchToState(Env, ENVSTATE_DECAY);
				Env->Current = FIXED(1);
				Env->CurrentCurved = FIXED(1);
			}
		}
		break;

		case ENVSTATE_DECAY:
		{
			int32_t SusLev = SustainLevel(Env->S);
			Env->CurrentTarget = SusLev;
			int32_t decaylength = EnvelopeLength(Env->D, Params->speed);
			int32_t Delta = -(FIXED(1) - SusLev) / decaylength;
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
			Env->DecayTime++;
			Env->DecayProgress = (Env->DecayTime * FIXED(1)) / (decaylength);

			Env->CurrentCurved = DoCurveDecay(SusLev, FIXED(1), FIXED(1) - Env->DecayProgress, Env->Curvature, &CurveSteps, Env->Current);
			if (Env->Current <= SusLev || Env->DecayTime >= decaylength)
			{
				Env->Current = SusLev;
				Env->CurrentCurved = SusLev;
				if (Params->mode == ENVMODE_TRIGGER || Params->mode == ENVMODE_LOOP)
				{
					SwitchToState(Env, ENVSTATE_RELEASE);
					if (Env->State == ENVSTATE_IDLE)
					{
						SwitchToState(Env, ENVSTATE_ATTACK);
					}
				}
				else
				{
					if (Env->Current == 0)
					{

						SwitchToState(Env, ENVSTATE_IDLE);
					}
					else
					{
						SwitchToState(Env, ENVSTATE_SUSTAIN);
					}
				}
			}


		}
		break;

		case ENVSTATE_SUSTAIN:
		{
			int32_t SusLev = SustainLevel(Env->S);
			Env->CurrentTarget = SusLev;
			int32_t Delta = (SustainLevel(Env->S) - Env->Current) / 5;
			Env->Current += Delta;
			Env->CurrentCurved = Env->Current;
		}
		break;

		case ENVSTATE_RELEASE:
		{
			Env->CurrentTarget = 0;
			int32_t releasetime = EnvelopeLength(Env->R, Params->speed);
			int32_t Delta = -Env->ReleaseStart / releasetime;
			Env->Current += Delta;
			Env->ReleaseTime++;
			if (Env->Current < 0) Env->Current = 0;
			Env->ReleaseProgress = (Env->ReleaseTime * FIXED(1)) / (releasetime);


			if (Env->CurvedReleaseStart > 0)
			{
				Env->CurrentCurved = DoCurveRelease(0, Env->CurvedReleaseStart, FIXED(1) - Env->ReleaseProgress, Env->Curvature, &CurveSteps, Env->Current);
			}
			else
			{
				Env->CurrentCurved = 0;
			}

			if (Env->ReleaseTime >= releasetime)
			{
				Env->Current = 0;
				Env->CurrentCurved = 0;

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


		for (int i = 0; i < 13; i++)
		{
			Env->StateLeds[i] = 0;
		}

		if (Env->State != ENVSTATE_IDLE)
		{
			// 12 leds, 8 and 9 are sustain.
			int32_t Led = 0;
			switch(Env->State)
			{
			case ENVSTATE_ATTACK: Led = (Env->AttackProgress * 3) >> (FIXEDBITS - 8); break;
			case ENVSTATE_DECAY: Led = (3 << 8) + ((Env->DecayProgress * 3) >> (FIXEDBITS - 8)); break;
			case ENVSTATE_SUSTAIN: Led = (6 << 8); break;			
			case ENVSTATE_RELEASE: Led = ((9 << 8) + 0xff) + ((Env->ReleaseProgress * 3) >> (FIXEDBITS - 8)); break;
			}
			

			if (0) {
				if (Led < Env->LastLed)
				{

					int idx1 = (Env->LastLed >> 8);
					int idx2 = 13;
					for (int i = idx1; i < idx2; i++)
					{
						Env->StateLeds[i] = 255;
					}
					Env->StateLeds[idx1] = (Env->LastLed & 0xff);
					Env->StateLeds[idx2] = (Led & 0xff);

					// dont slide back, since the release phase has not actually been performed in this case.
					//Env->LastLed = 0;
				}
			}
			//	printf("%4x %4x %d %d\n", Led, Env->LastLed, Led>>8, Env->LastLed>>8);

	//			if (Led >= Env->LastLed) this is always true now, so just make the led segment light up.
			{
				int idx1 = (Env->LastLed >> 8);
				int idx2 = Led >> 8;
				if (idx1!=idx2)
				for (int i = idx1; i <= idx2; i++)
				{
						Env->StateLeds[i] = 255;
				}
				Env->StateLeds[idx1] = (Env->LastLed & 0xff);
				Env->StateLeds[idx1] = 255 - Env->StateLeds[idx1];
				if (idx2 != idx1)
				{
					Env->StateLeds[idx2+1] = (Led & 0xff);
				}
				else
				{
					Env->StateLeds[idx1 + 1] = ~Env->StateLeds[idx1];
				}
			}
			Env->StateLeds[7] = Env->StateLeds[6];
			Env->StateLeds[8] = Env->StateLeds[6];
			Env->StateLeds[9] = Env->StateLeds[6];
			Env->LastLed = Led;
		}
		else
		{
			Env->LastLed = 0;
		}


		if (lowpassenabled)
		{
			Env->runninglowpass = (Env->runninglowpass * 5 + Env->CurrentCurved) / 6;
		}
		else
		{
			Env->runninglowpass = Env->CurrentCurved;
		}


		int VelocityPeak = (Env->VelocityCurrent * 4095) / FIXED(2);
		Env->LinearOutput = (Env->Current * (VelocityPeak - Calibration->CalibNormal)) / (FIXED(1));
		Env->CurvedOutput = (Env->runninglowpass * (VelocityPeak - Calibration->CalibCurved)) / (FIXED(1));

		Env->LinearOutput += Calibration->CalibNormal;
		Env->CurvedOutput += Calibration->CalibCurved;
		if (Env->LinearOutput < 0) Env->LinearOutput = 0; else if (Env->LinearOutput > 4095) Env->LinearOutput = 4095;
		if (Env->CurvedOutput < 0) Env->CurvedOutput = 0; else if (Env->CurvedOutput > 4095) Env->CurvedOutput = 4095;

		return Env->LinearOutput;
	}

#ifdef __cplusplus
}
#endif
