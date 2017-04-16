#include "Wobbler.h"
#include <math.h>
#include "../../EurorackShared/EURORACKSHARED.H"

#define __max(a,b) ((a)>(b)?(a):(b))
#define __min(a,b) ((a)<(b)?(a):(b))

#ifdef __cplusplus
extern "C"
{
#endif

	void Wobbler_RandomSeed(struct Wobbler_RandomGen *R, unsigned int seed)
	{
		R->RandomMemory = (long)seed;
	}

	int Wobbler_Rand(struct Wobbler_RandomGen *R)
	{
		return (((R->RandomMemory = R->RandomMemory * 214013L + 2531011L) >> 16) & 0x7fff);
	}

	void Wobbler_Init(struct Wobbler_LFO *LFO)
	{
		LFO->Output = 0;
		LFO->OutputPhased = 0;
		LFO->Phase1 = 0;
		LFO->Gate[0] = 0;
		LFO->Gate[1] = 0;
		LFO->EnvelopeVal = 0;
		LFO->PhasedCountdown = 0;
		LFO->EnvelopeState = WOBBLER_IDLE;

	}

	void Wobbler_Trigger(struct Wobbler_LFO *LFO, unsigned char N, struct Wobbler_Params *Params)
	{
		if (N == 0)
		{
			LFO->Phase1 = 0;
			Wobbler_StartTwang(LFO);
		}
	}

	void Wobbler_LoadSettings(struct Wobbler_Settings *settings, struct Wobbler_Params *params)
	{

	}

	void Wobbler_ValidateParams(struct Wobbler_Params *params)
	{

	}

	unsigned long LFORange(int32_t V, int32_t SR)
	{
		return  1 + V*SR* 64;
		//	return (unsigned long)(64 * pow((int32_t)((SR * 6) / 64.0), pow((int32_t)V, 0.54f)));
	}

	unsigned long LFORange2(int32_t V)
		{
			return  1 + V*V * 256 ;
			//	return (unsigned long)(64 * pow((int32_t)((SR * 6) / 64.0), pow((int32_t)V, 0.54f)));
		}





	void Wobbler_StartTwang(struct Wobbler_LFO *LFO)
	{
		//LFO->EnvelopeVal = 0;
		LFO->EnvelopeState = WOBBLER_ATTACK;
	}

	int SampleHold(struct Wobbler_LFO_SNH *sh, struct Wobbler_LFO *lfo, uint32_t phase, uint16_t mod)
	{
		int newseg = (phase >> 29);
		SetSVF(&sh->filt, 0x10 + mod/4,  0x150 );

		if (newseg != sh->lastseg)
		{
			if (newseg == 0)
			{
				Wobbler_RandomSeed(&sh->random, lfo->Phasing);
			}
			sh->lastseg = newseg;
			sh->lastval = (Wobbler_Rand(&sh->random)<<14) - (1<<28);
		}
		ProcessSVF(&sh->filt, sh->lastval>>16);
		return sh->filt.lo;
	}

	int Twang(struct Wobbler_LFO *LFO, uint32_t phase)
	{
		return (Sine(phase) >> 16) * (LFO->EnvelopeVal >> 8);
	}

	int Wobbler_Get(struct Wobbler_LFO *LFO, struct Wobbler_Params *Params)
	{
		if (LFO->Gate[0] > 0)
		{
			LFO->Gate[0]--;
		}
		if (LFO->Gate[1] > 0) {
			LFO->Gate[1]--;
		}

		if (LFO->EnvelopeState != WOBBLER_IDLE)
		{
			uint32_t A = 0;
			uint32_t R = LFORange(128, 2000) >> 12;
			if (LFO->Mod < 128)
			{
				R = 1 + (LFORange(LFO->Mod, 2000) >> 12);
			}
			else
			{
				A = 1 + (LFORange(LFO->Mod - 128, 2000) >> 12);
			}
			if (LFO->EnvelopeState == WOBBLER_ATTACK)
			{
				if (A == 0)
				{
					LFO->EnvelopeState = WOBBLER_RELEASE;
					LFO->EnvelopeVal = 1 << 24;
				}
				else
				{
					LFO->EnvelopeVal += ((1 << 24) - 1) / A;
					if (LFO->EnvelopeVal >= 1 << 24)
					{
						LFO->EnvelopeVal = 1 << 24;
						LFO->EnvelopeState = WOBBLER_RELEASE;
					}
				}
			}
			else
			{
				LFO->EnvelopeVal -= ((1 << 24) - 1) / R;
				if (LFO->EnvelopeVal <= 0)
				{
					LFO->EnvelopeState = WOBBLER_IDLE;
					LFO->EnvelopeVal = 0;
				}
			}
		}


		uint32_t DP = LFORange2(LFO->Speed);;
		LFO->Phase1 += DP;

		uint32_t DP2 = LFO->Phasing * 0x1000000;
		//DP2 <<= 24;
		LFO->Phase2 = LFO->Phase1 + DP2;

		for (int i = 0; i < 12; i++)
		{
			LFO->Led[i] = 0;
		}

		if (LFO->Phase1 < LFO->OldPhase1)
		{
			LFO->Gate[1] = WOBBLER_GATECOUNTDOWN;
			if (LFO->PhasedCountdown > 0)
			{
				LFO->Gate[0] = WOBBLER_GATECOUNTDOWN;
			}
			LFO->PhasedCountdown = LFO->Phasing << 24;
		}

		uint32_t last = LFO->PhasedCountdown;
		LFO->PhasedCountdown -= __min(DP, LFO->PhasedCountdown);
		if (LFO->PhasedCountdown == 0 && last != 0)
		{
			LFO->Gate[0] = WOBBLER_GATECOUNTDOWN;
		}

		LFO->OldPhase1 = LFO->Phase1;
		LFO->OldPhase2 = LFO->Phase2;

		int32_t O[4];
		int32_t P[4];


		O[0] = BasicShapes(LFO->Phase1, LFO->Mod);
		O[1] = Twang(LFO, LFO->Phase1);
		O[2] = SampleHold(&LFO->SNH[0],LFO, LFO->Phase1, LFO->Mod);
		O[3] = -BasicShapes(LFO->Phase1, LFO->Mod);

		P[0] = BasicShapes(LFO->Phase2, LFO->Mod);
		P[1] = Twang(LFO, LFO->Phase2);
		P[2] = SampleHold(&LFO->SNH[1], LFO, LFO->Phase2, LFO->Mod);
		P[3] = -BasicShapes(LFO->Phase2, LFO->Mod);



		LFO->Output = LERP(O, 3, LFO->Shape) / (0xffff * 4);
		LFO->OutputPhased = LERP(P, 3, LFO->Shape) / (0xffff * 4);

		LFO->Output += 2048;// + (2540 - 2048);
		LFO->OutputPhased += 2048;// +(2540 - 2048);

		if (LFO->Output > 4095) LFO->Output = 4095; else if (LFO->Output < 0) LFO->Output = 0;
		if (LFO->OutputPhased > 4095) LFO->OutputPhased = 4095; else if (LFO->OutputPhased < 0) LFO->OutputPhased = 0;
		//if (LFO->Output > 1) LFO->Output = 1; else if (LFO->Output < -1) LFO->Output = -1;

		int32_t LedIdxB = (LFO->Output * 11);
		int iLedIdxB = LedIdxB >> 12;
		int IdxB = ((LedIdxB - (iLedIdxB << 12))) >> 4;

		LFO->Led[(iLedIdxB + 12) % 12] = 255 - IdxB;
		LFO->Led[(iLedIdxB + 13) % 12] = IdxB;

		int32_t LedIdxA = (LFO->OutputPhased * 11);
		int iLedIdxA = LedIdxA >> 12;
		int IdxA = ((LedIdxA - (iLedIdxA << 12))) >> 4;

		LFO->Led[(iLedIdxA + 12) % 12] = __max(LFO->Led[(iLedIdxA + 12) % 12], 255 - IdxA);
		LFO->Led[(iLedIdxA + 13) % 12] = __max(LFO->Led[(iLedIdxA + 13) % 12], IdxA);


		return LFO->Output;
	}



#ifdef __cplusplus
}
#endif
