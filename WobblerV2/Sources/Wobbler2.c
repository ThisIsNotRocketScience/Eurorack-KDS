#include "Wobbler2.h"
#include <Math.h>
#include "../../EurorackShared/EURORACKSHARED.H"

#define __max(a,b) ((a)>(b)?(a):(b))
#define __min(a,b) ((a)<(b)?(a):(b))

#define PI  (float)(3.1415926535897932384626433832795)
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
		LFO->syncindex = 0;
		LFO->SNH.F1.high = LFO->SNH.F1.mid = LFO->SNH.F1.low = 0;
		LFO->SNH.F2.high = LFO->SNH.F2.mid = LFO->SNH.F2.low = 0;
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
		settings->SlowSpeedMult = 0;
		
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
	//	LFO->EnvelopeVal = 0;
		LFO->EnvelopeState = Wobbler2_ATTACK;
	}

#define SNHTABCURVECOUNT 128
#define VAL(x) (uint32_t)((x*1024))
	uint32_t SNHTAB[SNHTABCURVECOUNT] = { VAL(0), VAL(0.25), VAL(0.5), VAL(0.75), VAL(1) };
	const uint32_t Cmapping[] = {
		0x03F0D370, 0x04113BF3, 0x0432AF01, 0x04553521, 0x0478D733, 0x049D9E4A, 0x04C393DB, 0x04EAC196, 0x05133184, 0x053CEE01, 0x056801B9, 0x059477BD, 0x05C25B5F, 0x05F1B862, 0x06229AEB, 0x06550F7A,
		0x068922FE, 0x06BEE2C9, 0x06F65C99, 0x072F9E9E, 0x076AB787, 0x07A7B67C, 0x07E6AB09, 0x0827A55A, 0x086AB609, 0x08AFEE3C, 0x08F75FB8, 0x09411CBC, 0x098D3826, 0x09DBC57E, 0x0A2CD8D7, 0x0A8086EF,
		0x0AD6E539, 0x0B3009CF, 0x0B8C0B83, 0x0BEB01D8, 0x0C4D052E, 0x0CB22E85, 0x0D1A97CD, 0x0D865BD1, 0x0DF595FE, 0x0E6862FF, 0x0EDEE00D, 0x0F592B8A, 0x0FD764CC, 0x1059AC2A, 0x10E022D7, 0x116AEB71,
		0x11FA2945, 0x128E0135, 0x132698FD, 0x13C417AB, 0x1466A591, 0x150E6C51, 0x15BB96BC, 0x166E5159, 0x1726C9F1, 0x17E52F8A, 0x18A9B2E6, 0x19748653, 0x1A45DDDA, 0x1B1DEEE9, 0x1BFCF0FA, 0x1CE31D14,
		0x1DD0AE1F, 0x1EC5E0BA, 0x1FC2F404, 0x20C8285F, 0x21D5C0FA, 0x22EC02BB, 0x240B34E4, 0x2533A0E4, 0x266592AA, 0x27A1587E, 0x28E74328, 0x2A37A668, 0x2B92D7DC, 0x2CF93096, 0x2E6B0C74, 0x2FE8C9D7,
		0x3172CA8D, 0x330973D6, 0x34AD2D72, 0x365E6291, 0x381D82C2, 0x39EAFFC4, 0x3BC75008, 0x3DB2EDBC, 0x3FAE5631, 0x41BA0BB5, 0x43D69409, 0x4604799B, 0x48444B8B, 0x4A969D09, 0x4CFC05F3, 0x4F75237A,
		0x5202981D, 0x54A50A1A, 0x575D26E2, 0x5A2BA094, 0x5D112EEE, 0x600E8FF1, 0x6324873B, 0x6653DF4B, 0x699D68E2, 0x6D01FA5D, 0x7082737D, 0x741FB99F, 0x77DAB864, 0x7BB4656B, 0x7FADBC91, 0x83C7C1D7,
		0x8803829B, 0x8C62145D, 0x90E4955A, 0x958C2C93, 0x9A5A0B05, 0x9F4F6C4F, 0xA46D94D1, 0xA9B5D389, 0xAF298355, 0xB4CA09B5, 0xBA98D626, 0xC09765E7, 0xC6C742B7, 0xCD2A0055, 0xD3C142BD, 0xDA8EB7F0
	};


	uint16_t Wobbler2_CalcSampleHoldFilter(uint16_t Mod, uint32_t DP)
	{
		uint32_t freq = 0xffffffff / DP;
		Mod = 255-Mod;
		Mod = ((Mod*245)/255);
		//Mod = (Mod * 20) / 33;

		uint32_t m = LERP(Cmapping, SNHTABCURVECOUNT - 1, Mod);

		//m *= freq*freq;
		//m /= 10000;
		uint16_t res = ((m >> 16));
		//res = ~res;
		return res;
	}
#define HI16(x) (x>>16)
#define LO16(x) (x&65535)

	void Wobbler2_SVF(Wobbler2_SVFstruct *f, int32_t coeff, uint32_t inp)
	{

		//	cutoff = 0 to 0xffff

		//	f = 2 sin(0 tot kwartpi ) //[approximately]

		signed short const tMid = HI16(f->mid);
		
		uint16_t sC = coeff;


		unsigned short Max = (0xf200 - sC);
		unsigned long fR = 0 * Max;
		//	sR=0xffff-HI16(fR);
		uint16_t sR = ~HI16(fR);

		uint16_t scaler = 0xffff;
		//if (coeff > 30000)
//		{
			scaler = 0xffff/20 + HI16(coeff * (0xd000));
	//	}

		f->high = (inp<<10 ) - f->low - sR * tMid;
		f->mid += (sC * HI16(f->high));
		f->low += ((HI16(sC * scaler))* tMid);

//uint32_t N = H + L;
//f->mid = B;
//if (f->mid < 0) f->mid = 0;
//if (f->mid > 0xffff) f->mid = 0xffff;
//f->low = L;

//if (f->low < 0) f->low = 0;
//if (f->low > 0xffff) f->low = 0xffff;

//f->mid = ((coeff * (inp - f->low))>>16) + f->mid;
//f->low = f->low + ((coeff * f->mid)>>16);
	}

	void Wobbler2_SampleHold(Wobbler2_LFO_SNH_t *sh, Wobbler2_LFO_t *lfo, uint32_t phase, uint32_t coeff, uint32_t phase2)
	{
		int newseg = ((phase >> 29)) & 7;
		
		if (newseg != sh->lastseg)
		{
			sh->lastseg = newseg;
			sh->segbuffer[newseg] = ((Wobbler2_Rand(&sh->random)) >> 1);
			sh->lastval1 = sh->segbuffer[newseg];
		}

		int newseg2 = ((((phase2) >> 29))) & 7;
		sh->lastval2 = sh->segbuffer[newseg2];



		Wobbler2_SVF(&sh->F1, coeff, sh->lastval1);
		Wobbler2_SVF(&sh->F2, coeff, sh->lastval2);
		sh->store1 = sh->F1.low>>10;
		sh->store2 = sh->F2.low>>10;
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
			Wobbler2_GetSteppedResult(LFO->SpeedOrig, 11, &SpeedGrade);
#define S(x,y) ((x*65536)/y)
			int32_t Speeds[11] = { S(1,8),S(1,6),S(1,4),S(1,3), S(1,2),
									S(1,1),
									S(2,1),S(3,1),S(4,1),S(6,1),S(8,1) };
			uint32_t DPorig = Wobbler2_MakeFreq(LFO->Speed);// Wobbler2_LFORange2(LFO->Speed << 2, 0);;
			DP = ((LFO->SyncDP>>16) * GetInterpolatedResultInt(Speeds, &SpeedGrade)) ;
			uint32_t DPdiff = DPorig - DP;
		}
		else
		{
			DP = Wobbler2_MakeFreq(LFO->Speed);// Wobbler2_LFORange2(LFO->Speed << 2, 0);;
		}
		LFO->Phase1 += DP;

		uint32_t DP2 = LFO->Phasing * 0x100000;
		uint32_t DP3 = Wobbler2_MakeFreq(LFO->Phasing) ;
		//DP2 <<= 24;
		LFO->Phase2 = LFO->Phase1 + DP2;

		LFO->PhasedShift += DP;
		LFO->PhasedShift += DP3;



		if (LFO->Phase1 < LFO->OldPhase1)
		{
			LFO->Gate[1] = Wobbler2_GATECOUNTDOWN;
		//		if (LFO->PhasedCountdown > 0)
		//		{
		//			LFO->Gate[0] = Wobbler2_GATECOUNTDOWN;
		//		}
		//		LFO->PhasedCountdown = LFO->Phasing << 24;
		}

		//	uint32_t last = LFO->PhasedCountdown;
		//	LFO->PhasedCountdown -= __min(DP, LFO->PhasedCountdown);
		//	if (LFO->PhasedCountdown == 0 && last != 0)
		//	{
		//		LFO->Gate[0] = Wobbler2_GATECOUNTDOWN;
		//	}

		LFO->OldPhase1 = LFO->Phase1;
		
		//	LFO->OldPhase2 = LFO->Phase2;

		Wobbler2_SampleHold(&LFO->SNH, LFO, LFO->Phase1, Wobbler2_CalcSampleHoldFilter(LFO->Mod>>8, DP), LFO->Phase1 - DP2);

		CalculateCompensation(&LFO->CompensationVals, LFO->Mod >> 8);
		//Shapes_t BSO, &LFO->BasicShapesB;
		LFO->OutputsNormal[0] = FillBasicShapes(LFO->Phase1, LFO->Mod >> 8, &LFO->BasicShapesA, &LFO->CompensationVals);

#ifdef INTPENDULUM
		Wobbler2_DoublePendulumInt(&LFO->Pendulum, LFO->BasicShapesA.Sine);
#else
		Wobbler2_DoublePendulum(&LFO->Pendulum, 0.05f, LFO->BasicShapesA.Sine );
#endif

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
