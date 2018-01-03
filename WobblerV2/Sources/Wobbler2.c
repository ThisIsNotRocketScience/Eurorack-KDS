#include "Wobbler2.h"
#include <Math.h>
#include "../../EurorackShared/EURORACKSHARED.H"

#define __max(a,b) ((a)>(b)?(a):(b))
#define __min(a,b) ((a)<(b)?(a):(b))

#ifdef __cplusplus
extern "C"
{
#endif


int32_t imul(int32_t a, int32_t b)
	{
		int64_t R = (int64_t)a * (int64_t)b;


		return R >> (WOBBLER_FIXBITS);
	}

	int32_t idiv(int32_t a, int32_t b)
	{
		int64_t R = (int64_t)a;
		R <<= WOBBLER_FIXBITS;
		R/= (int64_t)b;
		return R ;
	}

	void Wobbler2_RandomSeed( Wobbler2_RandomGen *R, unsigned int seed)
	{
		R->RandomMemory = (long)seed;
	}

	void Wobbler2_SyncPulse( Wobbler2_LFO_t *LFO)
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

			LFO->SyncedPeriodTime = (LFO->synctime[0] + LFO->synctime[1] + LFO->synctime[2])/3;
			LFO->SyncDP = 0xffffffff/ LFO->SyncedPeriodTime;
		}
		LFO->synctime[LFO->syncindex] = Delta;

	}

	int Wobbler2_Rand( Wobbler2_RandomGen *R)
	{
		return (((R->RandomMemory = R->RandomMemory * 214013L + 2531011L) >> 16) & 0x7fff);
	}

	void Wobbler2_Init( Wobbler2_LFO_t *LFO)
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
		Wobbler2_InitPendulum(&LFO->Pendulum, LFO);
		Wobbler2_RandomSeed(&LFO->SNH.random, 0);
		for (int i = 0; i < 32; i++)
		{
			LFO->SNH.segbuffer[i] = 0;
		}
		//Wobbler2_InitIntPendulum(&LFO->Pendulum);
	}

	void Wobbler2_Trigger( Wobbler2_LFO_t *LFO, unsigned char N, struct Wobbler2_Params *Params)
	{
		if (N == 0)
		{
			LFO->TriggerLed = 255;
			LFO->Phase1 = 0;
			LFO->PhasedShift = 0;
			Wobbler2_StartTwang(LFO);
#ifdef INTPENDULUM
			Wobbler2_InitIntPendulum(&LFO->Pendulum);
#else
			Wobbler2_InitPendulum(&LFO->Pendulum, LFO);

#endif
		}
	}

	void Wobbler2_LoadSettings( Wobbler2_Settings *settings,  Wobbler2_Params *params)
	{

	}

	void Wobbler2_ValidateParams( Wobbler2_Params *params)
	{

	}

	unsigned long Wobbler2_LFORange(int32_t V, int32_t SR)
	{
		return  1 + ((V*SR * 64)>>8);
		//	return (unsigned long)(64 * pow((int32_t)((SR * 6) / 64.0), pow((int32_t)V, 0.54f)));
	}

	unsigned long Wobbler2_LFORange2(int32_t V, int32_t SR)
	{
		return  1 + V*V *8;
		//	return (unsigned long)(64 * pow((int32_t)((SR * 6) / 64.0), pow((int32_t)V, 0.54f)));
	}

	unsigned long Wobbler2_LFORange3(int32_t V, int32_t SR)
	{
		return  1 + ((V*SR )>>13);
		//	return (unsigned long)(64 * pow((int32_t)((SR * 6) / 64.0), pow((int32_t)V, 0.54f)));
	}
	

	void Wobbler2_InitPendulum(Wobbler2_Pendulum_t *P, Wobbler2_LFO_t *W)
	{
		P->A = 0;
		P->B = 0;
		P->Theta1 = -3.1415 / 2.0f;
		P->Theta2 = 3.1415 / 2.0f;
		P->d2Theta1 = 0;
		P->d2Theta2 = 0;
		P->dTheta1 = 1;
		P->dTheta2 = 0;
		P->l1 = 1;
		P->l2 = 1;
		P->g = .2981;
		Wobbler2_UpdateSettings(P, W);
		
	}

	void Wobbler2_UpdateSettings(Wobbler2_Pendulum_t *P, Wobbler2_LFO_t *W)
	{
		P->Damping = 0.999 + (W->Speed / (float)512)*0.00099999999;
		float p1 = W->Phasing / (float)0xffff;
		float M = 1.0f + 2.0f * (W->Mod / (float)0xffff);
		P->m1 = 2.1f + p1 * M;
		P->m2 = 2.1f + (1-p1) * M;
		P->mu = 1.0f + P->m1 / P->m2;
	}

	float fsin(float P)
	{
	//	P *= (float)(1 << 32);
		P /= (float)6.283f;;
		P *= (float)0xffffffff;
		int32_t phase = (int)P;
		int32_t R = Sine(phase);
		return (float)R / (float)(1 << 30);
	}


	//P->d2Theta1 = (P->g*(st2*c1sub2 - P->mu*st1) - (P->l2*P->dTheta2*P->dTheta2 + P->l1*P->dTheta1*P->dTheta1*c1sub2)*s1sub2) / (P->l1*(P->mu - c1sub2*c1sub2));
	//P->d2Theta2 = (P->mu*P->g*(st1*c1sub2 - st2) + (P->mu*P->l1*P->dTheta1*P->dTheta1 + P->l2*P->dTheta2*P->dTheta2*c1sub2)*s1sub2) / (P->l2*(P->mu - c1sub2*c1sub2));

	void Wobbler2_DoublePendulum( Wobbler2_Pendulum_t *P, float const DT)
	{
		//while (P->Theta1 < 0) { P->Theta1 += 6.283f; P->Theta2 += 6.283f; }
		P->_2sub1 = P->Theta2 - P->Theta1;
		P->_1sub2 = P->Theta1 - P->Theta2;
		P->st1 = sin(P->Theta1);
		P->st2 = sin(P->Theta2);
		P->c1sub2 = cos(P->_1sub2);
		P->c1sub2SQUARED = P->c1sub2 * P->c1sub2;
		P->s1sub2 = sin(P->_1sub2);
		P->dTheta2SQUARED = P->dTheta2 * P->dTheta2;
		P->dTheta1SQUARED = P->dTheta1 * P->dTheta1;
		P->l1_x_dTheta1SQUARED = P->l1 * P->dTheta1SQUARED;
		P->l2_x_dTheta2SQUARED = P->l2 * P->dTheta2SQUARED;
		float const T1a1 = P->st2 * P->c1sub2;
		float const T1a2 = P->mu * P->st1;
		float const T1a = T1a1 - T1a2;
		float const T1 = P->g * T1a;
		float const T2b1 = P->l1_x_dTheta1SQUARED * P->c1sub2;
		float const T2b = P->l2_x_dTheta2SQUARED + T2b1;
		float const T2 = T2b * P->s1sub2;
		float const T3b = P->mu - P->c1sub2SQUARED;
		float const T3 = P->l1*T3b;
		float const T4 = P->mu * P->g * (P->st1 * P->c1sub2 - P->st2);
		float const T5a2 = P->l2_x_dTheta2SQUARED * P->c1sub2;
		float const T5a1 = P->l1_x_dTheta1SQUARED * P->mu;
		float const T5a = T5a1 + T5a2;
		float const T5 = T5a * P->s1sub2;
		float const T6b = P->mu - P->c1sub2SQUARED;
		float const T6 = P->l2 * T6b;

		P->d2Theta1 = (T1 - T2) / (T3);
		P->d2Theta2 = (T4 + T5) / T6;
		P->dTheta1 *= P->Damping;
		P->dTheta2 *= P->Damping;
		P->dTheta1 += P->d2Theta1*DT;
		P->dTheta2 += P->d2Theta2*DT;
		P->Theta1 += P->dTheta1*DT;
		P->Theta2 += P->dTheta2*DT;
		P->A = P->Theta1 * 0xffff;
		P->B = P->Theta2 * 0xffff;
		P->As = P->Theta1 * ((float)(0xffff)/6.283f);
		P->Bs = P->Theta2 *  ((float)(0xffff) / 6.283f);
		while (P->Theta1 > 3.1415f) P->Theta1 -= 6.283f;
		while (P->Theta2 > 3.1415f) P->Theta2 -= 6.283f;
		while (P->Theta1 < -3.1415f) P->Theta1 += 6.283f;
		while (P->Theta2 < -3.1415f) P->Theta2 += 6.283f;

		P->As *= 0xffff;
		P->Bs *= 0xffff;
	}






	void Wobbler2_StartTwang( Wobbler2_LFO_t *LFO)
	{
		//LFO->EnvelopeVal = 0;
		LFO->EnvelopeState = Wobbler2_ATTACK;
	}

	void Wobbler2_SampleHold( Wobbler2_LFO_SNH_t *sh,  Wobbler2_LFO_t *lfo, uint32_t phase, uint16_t modin, uint32_t phase2)
	{
		int newseg = ((phase >> 29))&7;
		//SetSVF(&sh->filt, 0x10 + (mod / (256)), 0x150+(mod/1024));
		modin = ((modin)/16);
		modin = ~modin;
		
		uint16_t mod = (modin*modin)/(1<<14);
		mod = (~mod)*2;
		//mod = (mod * mod)/65536;
		//mod = mod / 2 ;

		if (newseg != sh->lastseg)
		{
			sh->lastseg = newseg;
			sh->segbuffer[newseg] =( (Wobbler2_Rand(&sh->random))>>1);
			sh->lastval1 = sh->segbuffer[newseg];
		}

		int newseg2 = ((((  phase2) >> 29)) ) &7  ;
		sh->lastval2 = sh->segbuffer[newseg2];

		uint32_t r1 = sh->store1 * mod;
		uint16_t m2 = (~mod);
		uint32_t r2 = (sh->lastval1) * m2;
		sh->store1 = (r1/65536)  + (r2 / 65536);

		uint32_t r1a = sh->store2 * mod;
		uint32_t r2a = (sh->lastval2) * m2;
		sh->store2 = (r1a/65536) + (r2a/ 65536);


	}

	int Wobbler2_Twang( Wobbler2_LFO_t *LFO, uint32_t phase)
	{
		return (Sine(phase) >> 16) * (LFO->EnvelopeVal >> 8);
	}

	int Wobbler2_Get( Wobbler2_LFO_t *LFO,  Wobbler2_Params *Params)
	{
		LFO->timesincesync++;
		
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
		if (LFO->TriggerLed > 0) LFO->TriggerLed --;
		if (LFO->EnvelopeState != Wobbler2_IDLE)
		{
			uint32_t A = 0;
			uint32_t R = Wobbler2_LFORange3(128<<8, 2000) ;
			if ((LFO->Mod>>8) < 128)
			{
				R = 1 + (Wobbler2_LFORange3(LFO->Mod, 2000));
			}
			else
			{
				A = 1 + (Wobbler2_LFORange3(LFO->Mod - (128<<8), 2000) );
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
			
			DP = (LFO->SyncDP * GetInterpolatedResultInt(Speeds, &SpeedGrade))>>16;
		}
		else
		{
			DP = Wobbler2_LFORange2(LFO->Speed << 2, 0);;
		}
		LFO->Phase1 += DP;

		uint32_t DP2 = LFO->Phasing * 0x100000;
		//DP2 <<= 24;
		LFO->Phase2 = LFO->Phase1 + DP2;

		LFO->PhasedShift += DP2>>8;
		


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

		int32_t O[6] = {0};
		int32_t P[6] = {0};
#ifdef INTPENDULUM
		Wobbler2_DoublePendulumInt(&LFO->Pendulum);
#else
		Wobbler2_DoublePendulum(&LFO->Pendulum,0.05f);
#endif
		Wobbler2_SampleHold(&LFO->SNH, LFO, LFO->Phase1, LFO->Mod, LFO->Phase1 - DP2);

		Shapes_t BSO, BSP;
		O[0] = FillBasicShapes(LFO->Phase1, LFO->Mod>>8,&BSO);
		O[1] = (BasicShapes(LFO->Phase1 + LFO->PhasedShift, LFO->Mod>>8) + O[0])/2;
		O[2] = Wobbler2_Twang(LFO, LFO->Phase1);
		O[3] = LFO->Pendulum.As;// + 0x80000000;
		O[4] = (int32_t)(LFO->SNH.store1<<16) - (1<<29);

		P[0] = FillBasicShapes(LFO->Phase2, LFO->Mod>>8, &BSP);
		P[1] = (BasicShapes(LFO->Phase2 +LFO->PhasedShift, LFO->Mod>>8) + P[0])/2;
		P[2] = Wobbler2_Twang(LFO, LFO->Phase2);
		P[3] = LFO->Pendulum.Bs;// + 0x80000000;
		P[4] = (int32_t)(LFO->SNH.store2<<16) - (1<<29);


		if ((BSP.Sine > 0) && (LFO->OldPhase2 <0))
		{
			LFO->Gate[0] = Wobbler2_GATECOUNTDOWN;
		}

		LFO->OldPhase2 = BSP.Sine;

		Wobbler2_GetSteppedResult(LFO->Shape, 4, &LFO->ShapeStepped);

		LFO->Output = GetInterpolatedResultInt(O, &LFO->ShapeStepped) /(0xffff*4);
		LFO->OutputPhased = GetInterpolatedResultInt(P, &LFO->ShapeStepped) /(0xffff*4);

		LFO->Output = (LFO->Output *LFO->Amount1) / (int)(1<<14);
		LFO->OutputPhased = (LFO->OutputPhased *LFO->Amount2) / (int)(1<<14);


		LFO->Output += 2048;// + (2540 - 2048);
		LFO->OutputPhased += 2048;// +(2540 - 2048);

		if (LFO->Output > 4095) LFO->Output = 4095; else if (LFO->Output < 0) LFO->Output = 0;
		if (LFO->OutputPhased > 4095) LFO->OutputPhased = 4095; else if (LFO->OutputPhased < 0) LFO->OutputPhased = 0;
		//if (LFO->Output > 1) LFO->Output = 1; else if (LFO->Output < -1) LFO->Output = -1;

		Wobbler2_DoLeds(LFO);

		return LFO->Output;
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
		unsigned char IF =  ~inp->fractional;
		return ((table[inp->index]/256) * IF)  + ((table[inp->index + 1]/256) * F);
	}

	void Wobbler2_GetSteppedResult(uint16_t param, uint8_t steps, SteppedResult_t *out)
	{
		//max(floor(x + 0.25), (x + 0.25 - floor(x + 0.25)) * 2 + floor(x + 0.25) - 1)

		uint32_t X = (param * steps)/256;
		X += 64;

		int FloorX = X &0xffffff00;
		int Aside = FloorX;
		int Bside = (X - (FloorX)) * 2 + (FloorX)-256;
		int M = (Aside > Bside) ? Aside : Bside;

		out->index = M >> 8;
		out->fractional = M & 0xff;

	}


#ifdef __cplusplus
}
#endif
