#include "Tuesday.h"
#include <stdint.h>

#include "Algo.h"
#include "AlgoImpl.h"

struct PatternFunctions PatternTypes[__ALGO_COUNT];

void SetPatternFunc(int i, GenFuncPtr Gen, InitFuncPtr Init, PatternInitFuncPtr PatternInit, uint8_t dither)
{
	struct PatternFunctions *PF = &PatternTypes[i];
	PF->Gen = Gen;
	PF->Init = Init;
	PF->PatternInit = PatternInit;		
	PF->Dither = dither;
}

void Tuesday_Init(struct Tuesday_PatternGen *P)
{
	for (int i = 0; i < __ALGO_COUNT; i++)
	{
		SetPatternFunc(i, NoPattern, NoInit, NoPatternInit,1);
	}

	SetPatternFunc(ALGO_SAIKO_LEAD, &Algo_Saiko_Lead_Gen, &Algo_Init_Generic_FourBool, &NoPatternInit,1);
	SetPatternFunc(ALGO_CHIPARP1, Algo_ChipArp_1_Gen, Algo_ChipArp_1_Init, Algo_ChipArp_1_PatternInit,1);	
	SetPatternFunc(ALGO_TRITRANCE, &Algo_TriTrance_Gen, &Algo_TriTrance_Init, &NoPatternInit,1);
	SetPatternFunc(ALGO_TESTS, &Algo_Test_Gen, &Algo_Test_Init, &NoPatternInit,1);
	SetPatternFunc(ALGO_MARKOV, &Algo_Markov_Gen, &Algo_Markov_Init, &NoPatternInit,1);
	SetPatternFunc(ALGO_STOMPER, &Algo_Stomper_Gen, &Algo_Stomper_Init, &NoPatternInit, 1);
	SetPatternFunc(ALGO_WOBBLE, &Algo_Wobble_Gen, &Algo_Wobble_Init, &NoPatternInit, 1);
	SetPatternFunc(ALGO_SNH, &Algo_SNH_Gen, &Algo_SNH_Init, &NoPatternInit, 1);

	P->ClockConnected = 0;
	P->lastnote = 0;
	P->CoolDown = 0;

	P->TickOut = 0;
	P->CVOut = 0;
	P->CVOutDelta = 0;
	P->CVOutCountDown = 0;
	P->CVOutTarget = 0;

	P->Tick = -1;
	P->Measure = 0;

	P->countdownTick = 1;
	P->countdownNote = 1;
	P->msecpertick = 10 ;

	P->clockup = 0;
	P->clockshad = 0;
	P->clockssincereset = 0;

	P->timesincelastclocktick = 0;
	P->clocktick = 0;

	P->directtick = 0;

	P->extclockssincereset[0] = 0;
	P->extclockssincereset[1] = 0;
	P->extclockssincereset[2] = 0;
	P->extclockssincereset[3] = 0;
	P->extclockssincereset[4] = 0;
	P->extclockssincereset[5] = 0;

	P->extclockssinceresetcounter[0] = 0;
	P->extclockssinceresetcounter[1] = 0;
	P->extclockssinceresetcounter[2] = 0;
	P->extclockssinceresetcounter[3] = 0;
	P->extclockssinceresetcounter[4] = 0;
	P->extclockssinceresetcounter[5] = 0;

	P->extclocksupsincereset = 0;
	P->extclocksdowsincereset = 0;

	P->lastclocksubdiv = -1;

	for (int i = 0; i < TUESDAY_GATES; i++)
	{
		P->Gates[i] = 0;
	}

	for (int i = 0; i < TUESDAY_LEDS; i++)
	{
		P->StateLedTargets[i] = 0;
		P->RStateLeds[i] = 0;
	}

	P->UIMode = UI_STARTUP;
}

void Tuesday_Reset(struct Tuesday_PatternGen *T)
{
	T->TickOut = 0;
	T->Tick = -1;
	T->CoolDown = 0;
	T->countdownTick = 0;
	T->directtick = 1;
	T->clockssincereset = 0;
	for (int i = 0;i<6;i++)
	{
		T->extclockssincereset[i] = 0;
		T->extclockssinceresetcounter[i] = 0;
	}
	T->extclocksupsincereset =0;
	T->extclocksdowsincereset = 0;
}

void Tuesday_Tick(struct Tuesday_PatternGen *T, struct Tuesday_Params *P)
{
	int CoolDownMax = T->intensity;
	if (T->CoolDown > 0)
	{
		T->CoolDown--;
		if (T->CoolDown > CoolDownMax  ) T->CoolDown = CoolDownMax;
		if (T->CoolDown < 0) T->CoolDown= 0;
	}

	struct Tuesday_Tick *Tick = &T->CurrentPattern.Ticks[T->Tick];

	if (Tick->vel >= T->CoolDown)
	{
		T->CoolDown = CoolDownMax;

		T->countdownNote =( T->msecpertick * 900) / 1000;

		if (T->countdownNote >= T->msecpertick) T->countdownNote = 0;

		T->TickOut = (T->CurrentPattern.Ticks[T->Tick].accent * 2048 + 2047);

		if (T->CurrentPattern.Ticks[T->Tick].note != TUESDAY_NOTEOFF)
		{
			T->CVOutTarget = (DAC_NOTE(T->CurrentPattern.Ticks[T->Tick].note))<<16;
			if (Tick->slide > 0)
			{
				T->CVOutDelta = (T->CVOutTarget - T->CVOut)/ (Tick->slide * 50);
				T->CVOutCountDown = Tick->slide *50 ;
			}
			else
			{
				T->CVOut = T->CVOutTarget;
			}
			T->lastnote = T->CurrentPattern.Ticks[T->Tick].note;
			T->Gates[GATE_GATE] = GATE_MINGATETIME;
		}
		if (T->CurrentPattern.Ticks[T->Tick].note == TUESDAY_NOTEOFF)
		{
			T->TickOut  =0;
			T->Gates[GATE_GATE] = 0;
			T->lastnote = T->CurrentPattern.Ticks[T->Tick].note;
		}
		if (T->CurrentPattern.Ticks[T->Tick].accent > 0) T->Gates[GATE_ACCENT] = GATE_MINGATETIME;
	}

	if (T->Tick == 0) T->Gates[GATE_LOOP] = GATE_MINGATETIME;
	if (T->Tick % T->CurrentPattern.TPB == 0) T->Gates[GATE_BEAT] = GATE_MINGATETIME;

	T->Gates[GATE_TICK] = GATE_MINGATETIME;
}

void Tuesday_TimerTick(struct Tuesday_PatternGen *T, struct Tuesday_Params *P)
{
	T->timesincelastclocktick++;
	int clockmode = 1;
	if (T->clockup == 0 && T->timesincelastclocktick > 2000)
	{
		T->timesincelastclocktick = 3000;
		clockmode = 0;
	}

	T->T++;

	if (T->T % 2 == 0)
	{
		if (T->countdownNote >= 0)
		{
			T->countdownNote--;
			if (T->countdownNote <= 0)
			{
				T->TickOut = 0;
				T->Gates[GATE_GATE] = 0;
			}
		}

		int bpm = 1 + (200 * T->tempo) / 256;
		int msecperbeat = (1000 * 60) / (96 * (bpm/4));


		if (clockmode == 0)
		{
			T->countdownTick--;

			if (T->countdownTick > msecperbeat)
				T->countdownTick = msecperbeat;
			if (T->countdownTick <= 0 || T->directtick == 1)
			{
				DoClock(1);

				T->directtick = 0;
				T->countdownTick = msecperbeat;
			}
			else
			{
				DoClock(0);
			}
		}

	}
}

void Tuesday_Clock(struct Tuesday_PatternGen *P, int ClockVal)
{
	if (ClockVal == 1)
	{
		P->Gates[GATE_CLOCK] = GATE_MINGATETIME;


		if (P->clockssincereset >= 96)
		{
			P->clockssincereset = 0;
			P->Measure++;
			if (P->Measure * P->CurrentPattern.TPB * 4 >= P->CurrentPattern.Length) P->Measure =0 ;
		}


		//if (clockshad >= 96 / (Pattern.TPB * 4) || directtick == 1)
		int NewTick = (P->Measure * (P->CurrentPattern.TPB * 4)  + ((P->clockssincereset * (P->CurrentPattern.TPB * 4))/96)) % P->CurrentPattern.Length;
		if (NewTick != P->Tick || P->directtick == 1)
		{
			//#define USE_SEMIHOST
			//printf("%d %d\n", Measure, NewTick);
			if (P->Tick == -1) P->Tick = 0;

			doTick();
			P->Tick = NewTick;
			P->directtick = 0;
			P->clockshad = 0;
		}

		P->clockshad++;
		P->clockssincereset++;

	}
	else
	{
		P->Gates[GATE_CLOCK] = 0 ;
	}
}


uint32_t KnobOpt(uint32_t val)
{
	int r = 0;
	if (val > (65536*1)/5) r++;
	if (val > (65536*2)/5) r++;
	if (val > (65536*3)/5) r++;
	if (val > (65536*4)/5) r++;
	return 1 + 4 - r;
}

void Tuesday_ExtClock(struct Tuesday_PatternGen *P,struct Tuesday_Params *Params, int state)
{
	P->clockup = state;

	int clocksubdiv = KnobOpt(P->tempo<<8);

	if (P->lastclocksubdiv != clocksubdiv)
	{
		P->clockssincereset = P->extclockssincereset[clocksubdiv];
		P->lastclocksubdiv = clocksubdiv;
	}

	if (state == 1 )
	{
		if ((P->extclocksupsincereset % clocksubdiv) == 0)
		{
			Tuesday_Clock(P, 1);
		}
		P->extclocksupsincereset = (P->extclocksupsincereset + 1)%clocksubdiv;
	}
	else
	{
		if ((P->extclocksdowsincereset % clocksubdiv) == 0)
		{
			Tuesday_Clock(P, 0);
		}
		P->extclocksdowsincereset  = (P->extclocksdowsincereset  + 1)%clocksubdiv;

	}

	if (state == 1)
	{
		for (int i = 1;i<6;i++)
		{

			P->extclockssinceresetcounter[i]++;
			if (P->extclockssinceresetcounter[i] == i)
			{
				P->extclockssinceresetcounter[i] = 0;
				P->extclockssincereset[i] = (P->extclockssincereset[i] + 1)%96;
			}
		}
		P->timesincelastclocktick = 0;
	}

}

void Tuesday_ValidateParams(struct Tuesday_Params *P)
{
	P->algo = P->algo % TUESDAY_MAXALGO;
	P->beatopt = P->beatopt % TUESDAY_MAXBEAT;
	P->tpbopt = P->tpbopt % TUESDAY_MAXTPB;
	P->scale = P->scale % TUESDAY_MAXSCALE;
}

void Tuesday_LoadDefaults(struct Tuesday_Settings *S, struct Tuesday_Params *P)
{
	P->algo = 2;
	P->beatopt = 0;
	P->scale = 1;
	P->tpbopt = 2;

	S->algooptions[0] = ALGO_SNH;
	S->algooptions[1] = ALGO_TRITRANCE;
	S->algooptions[2] = ALGO_STOMPER;
	S->algooptions[3] = ALGO_WOBBLE;

	S->tpboptions[0] = 2;
	S->tpboptions[1] = 3;
	S->tpboptions[2] = 4;
	S->tpboptions[3] = 5;

	S->beatoptions[0] = 4;
	S->beatoptions[1] = 8;
	S->beatoptions[2] = 16;
	S->beatoptions[3] = 32;

	for (int j = 0; j < __SCALE_COUNT; j++)
	{
		for (int i = 0; i < 12; i++)
		{
			S->scales[j].notes[i] = i;
		}
		S->scales[j].count = 1;
	}

	S->scale[0] = SCALE_MAJOR;
	S->scale[1] = SCALE_MINOR;
	S->scale[2] = SCALE_DORIAN;
	S->scale[3] = SCALE_BLUES;

	S->scales[SCALE_MAJOR].notes[0] = 0;
	S->scales[SCALE_MAJOR].notes[1] = 2;
	S->scales[SCALE_MAJOR].notes[2] = 4;
	S->scales[SCALE_MAJOR].notes[3] = 5;
	S->scales[SCALE_MAJOR].notes[4] = 7;
	S->scales[SCALE_MAJOR].notes[5] = 9;
	S->scales[SCALE_MAJOR].notes[6] = 11;
	S->scales[SCALE_MAJOR].count = 7; // Major scale

	S->scales[SCALE_MINOR].notes[0] = 0;
	S->scales[SCALE_MINOR].notes[1] = 2;
	S->scales[SCALE_MINOR].notes[2] = 3;
	S->scales[SCALE_MINOR].notes[3] = 5;
	S->scales[SCALE_MINOR].notes[4] = 7;
	S->scales[SCALE_MINOR].notes[5] = 8;
	S->scales[SCALE_MINOR].notes[6] = 10;
	S->scales[SCALE_MINOR].count = 7; // Minor scale

	S->scales[SCALE_DORIAN].notes[0] = 0;
	S->scales[SCALE_DORIAN].notes[1] = 2;
	S->scales[SCALE_DORIAN].notes[2] = 3;
	S->scales[SCALE_DORIAN].notes[3] = 6;
	S->scales[SCALE_DORIAN].notes[4] = 7;
	S->scales[SCALE_DORIAN].notes[5] = 9;
	S->scales[SCALE_DORIAN].notes[6] = 10;
	S->scales[SCALE_DORIAN].count = 7; // Dorian scale

	S->scales[SCALE_PENTA].notes[0] = 0;
	S->scales[SCALE_PENTA].notes[1] = 2;
	S->scales[SCALE_PENTA].notes[2] = 5;
	S->scales[SCALE_PENTA].notes[3] = 7;
	S->scales[SCALE_PENTA].notes[4] = 9;
	S->scales[SCALE_PENTA].count = 5; // Pentatonic


	S->scales[SCALE_BLUES].notes[0] = 0;
	S->scales[SCALE_BLUES].notes[1] = 3;
	S->scales[SCALE_BLUES].notes[2] = 5;
	S->scales[SCALE_BLUES].notes[3] = 6;
	S->scales[SCALE_BLUES].notes[4] = 7;
	S->scales[SCALE_BLUES].notes[5] = 10;
	S->scales[SCALE_BLUES].count = 6; // Blues



	S->scales[SCALE_12TONE].notes[0] = 0;
	S->scales[SCALE_12TONE].notes[1] = 1;
	S->scales[SCALE_12TONE].notes[2] = 2;
	S->scales[SCALE_12TONE].notes[3] = 3;
	S->scales[SCALE_12TONE].notes[4] = 4;
	S->scales[SCALE_12TONE].notes[5] = 5;
	S->scales[SCALE_12TONE].notes[6] = 6;
	S->scales[SCALE_12TONE].notes[7] = 7;
	S->scales[SCALE_12TONE].notes[8] = 8;
	S->scales[SCALE_12TONE].notes[9] = 9;
	S->scales[SCALE_12TONE].notes[10] = 10;
	S->scales[SCALE_12TONE].notes[11] = 11;
	S->scales[SCALE_12TONE].count = 12; // 12tone
}

void Tuesday_LoadSettings(struct Tuesday_Settings *S, struct Tuesday_Params *P)
{
	Tuesday_LoadDefaults(S, P);
}


const unsigned char dither[24 * 3] =
{
		0b0001, 0b0011, 0b0111,
		0b0001, 0b0011, 0b1011,
		0b0001, 0b0101, 0b0111,
		0b0001, 0b0101, 0b1101,
		0b0001, 0b1001, 0b1011,
		0b0001, 0b1001, 0b1101,
		0b0010, 0b0011, 0b0111,
		0b0010, 0b0011, 0b1011,
		0b0010, 0b0110, 0b0111,
		0b0010, 0b0110, 0b1110,
		0b0010, 0b1010, 0b1011,
		0b0010, 0b1010, 0b1110,
		0b0100, 0b0101, 0b0111,
		0b0100, 0b0101, 0b1101,
		0b0100, 0b0110, 0b0111,
		0b0100, 0b0110, 0b1110,
		0b0100, 0b1100, 0b1101,
		0b0100, 0b1100, 0b1110,
		0b1000, 0b1001, 0b1011,
		0b1000, 0b1001, 0b1101,
		0b1000, 0b1010, 0b1011,
		0b1000, 0b1010, 0b1110,
		0b1000, 0b1100, 0b1101,
		0b1000, 0b1100, 0b1110
};

struct Tuesday_PatternFuncSpecific FuncSpecific[4];

struct Tuesday_Tick Ticks[4];
struct Tuesday_Tick Top;
struct Tuesday_Tick Bot;
struct Tuesday_RandomGen Randoms[4];

void CopyTick(struct Tuesday_Tick *A, struct Tuesday_Tick *Out)
{
	Out->accent = A->accent;
	Out->note = A->note;
	Out->vel = A->vel;
	Out->slide = A->slide;
}

void ApplyDither(int tick, uint32_t ditherpattern, struct Tuesday_Tick *A, struct Tuesday_Tick *B, struct Tuesday_Tick *Out)
{
	if ((ditherpattern >> (tick & 0b11)) & 1 == 1)
	{
		CopyTick(A, Out);
	}
	else
	{
		CopyTick(B, Out);
	}
}

void Tuesday_Generate(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S)
{

	int len = S->tpboptions[P->tpbopt] * S->beatoptions[P->beatopt];
	T->CurrentPattern.Length = len;
	T->CurrentPattern.TPB = S->tpboptions[P->tpbopt];
	int X = T->seed1 >> 3;
	int Y = T->seed2 >> 3;

	int XFade = (T->seed1 & 0b110) >> 1;
	int YFade = (T->seed2 & 0b110) >> 1;

	unsigned char xbase = T->seed1 & 0b1111 + ((T->seed1) >> 6);
	unsigned char ybase = T->seed2 & 0b1111 + ((T->seed2) >> 6);

	unsigned char ditherx = 0;
	unsigned char dithery = 0;

	if (XFade > 0) ditherx = dither[xbase * 3 + XFade - 1];
	if (YFade > 0) dithery = dither[ybase * 3 + YFade - 1];


	Tuesday_RandomSeed(&Randoms[0], X + Y * 32);
	Tuesday_RandomSeed(&Randoms[1], X + Y * 32 + 1);
	Tuesday_RandomSeed(&Randoms[2], X + Y * 32 + 32);
	Tuesday_RandomSeed(&Randoms[3], X + Y * 32 + 33);

	int CurrentAlgo = S->algooptions[P->algo] % __ALGO_COUNT;

	struct PatternFunctions *Algo = &PatternTypes[CurrentAlgo];

	for (int j = 0; j < 4; j++)
	{
		Algo->Init(T,P, S, &Randoms[j], &FuncSpecific[j]);
	}

	Algo->PatternInit(T,P, S, &T->CurrentPattern);

	if (Algo->Dither == 1)
	{
		for (int i = 0; i < len; i++)
		{
			for (int j = 0; j < 4; j++)
			{

				Algo->Gen(T,P, S, &Randoms[j], &FuncSpecific[j], i, &Ticks[j]);
			}

			ApplyDither(i, ditherx, &Ticks[0], &Ticks[1], &Top);
			ApplyDither(i, ditherx, &Ticks[2], &Ticks[3], &Bot);
			ApplyDither(i, dithery, &Top, &Bot, &T->CurrentPattern.Ticks[i]);
		}
	}
	else
	{
		for (int i = 0; i < len; i++)
		{
			Algo->Gen(T, P, S, &Randoms[0], &FuncSpecific[0], i, &T->CurrentPattern.Ticks[i]);
		}
	}
}

