#include "Algo.h"

void Algo_ChipArp_1_Init( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *Output)
{
	Output->Chip1.ChordSeed = Tuesday_Rand(R);
	Tuesday_RandomSeed(&Output->Chip1.R, Output->Chip1.ChordSeed);
	Output->Chip1.Base = Tuesday_Rand(R) % 3;
	Output->Chip1.Dir = Tuesday_RandByte(R) % 2;

}

void Algo_ChipArp_1_PatternInit( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_PatternContainer *PT)
{
	// this turned out to be not such a good idea.. just feed it faster tempo or use the TPB at a higher setting.
	//T->CurrentPattern.TPB *= 2;
}

void Algo_ChipArp_1_Gen(Tuesday_PatternGen *T, Tuesday_Params *P, Tuesday_Settings *S, Tuesday_RandomGen *R, Tuesday_PatternFuncSpecific *PS, int I, Tuesday_Tick_t *Output)
{
	struct ScaledNote SN;
	DefaultTick(Output);
	int chordpos = I % S->tpboptions[P->tpbopt];
	
	
	if (chordpos == 0)
	{
		Output->accent = 1;
		Tuesday_RandomSeed(&PS->Chip1.R, PS->Chip1.ChordSeed);
		if (Tuesday_PercChance(R, 0x20))
		{
			PS->Chip1.Base = Tuesday_Rand(R) % 3;
		}
		if (Tuesday_PercChance(R, 0xf0))
		{
			PS->Chip1.Dir = Tuesday_RandByte(R) % 2;
		}
	}
		
	if (PS->Chip1.Dir == 1)
	{
		chordpos = S->tpboptions[P->tpbopt] - chordpos - 1;
	}

	if (Tuesday_PercChance(&PS->Chip1.R, 0x20))
	{
		Output->accent = 1;
	}


	if (Tuesday_PercChance(&PS->Chip1.R, 0x80))
	{
		Output->slide = Tuesday_RandByte(&PS->Chip1.R) % 3;
	}

	if (Tuesday_PercChance(&PS->Chip1.R, 0xd0))
	{
		NOTEOFF();
	}
	else
	{
		SN.note = ((chordpos) * 2) + PS->Chip1.Base;
		SN.oct = 0;// ((I * PS->GENERIC.b2) % PS->GENERIC.b3);
		Output->maxsubticklength = 4 + 6 * (Tuesday_RandByte(&PS->Chip1.R) % 3);
	}
	Output->note = ScaleToNote(&SN, T, P, S);
	Output->vel = (Tuesday_RandByte(R) / 2) + (((I + PS->GENERIC.b2) == 0) ? 127 : 0);
	
}

void Algo_ChipArp_2_Init(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *Output)
{
	Output->Chip2.ChordSeed = Tuesday_Rand(R);
	Tuesday_RandomSeed(&Output->Chip2.R, Output->Chip2.ChordSeed);
	Output->Chip2.chordscaler = (Tuesday_Rand(R) % 3) + 2;

	Output->Chip2.offset = (Tuesday_Rand(R) % 5);
	Output->Chip2.len = ((Tuesday_Rand(R) & 0x3) + 1)*2;
	Output->Chip2.TimeMult =  Tuesday_BoolChance(R) ? Tuesday_BoolChance(R) : 0;
	Output->Chip2.DeadTime = 0;
	Output->Chip2.idx = 0;
	Output->Chip2.dir = Tuesday_BoolChance(R) ? Tuesday_BoolChance(R) : 0;
	Output->Chip2.ChordLen = 3 + (T->seed1>>6);
}

void Algo_ChipArp_2_PatternInit( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_PatternContainer *PT)
{
	//T->CurrentPattern.TPB *= 2;
}

void Algo_ChipArp_2_Gen(Tuesday_PatternGen *T, Tuesday_Params *P, Tuesday_Settings *S, Tuesday_RandomGen *R, Tuesday_PatternFuncSpecific *PS, int I, Tuesday_Tick_t *Output)
{
	struct ScaledNote SN;
	DefaultTick(Output);
	
	SN.oct = 0;
	if (PS->Chip2.DeadTime > 0)
	{
		PS->Chip2.DeadTime--;
		Output->accent = 0;
		NOTEOFF();
	}
	else
	{
		int DeadTimeAdd = 0;
		if (PS->Chip2.idx == PS->Chip2.ChordLen)
		{
			PS->Chip2.idx = 0;
			PS->Chip2.len--;
			Output->accent = 1;
			if (Tuesday_PercChance(R, 200)) DeadTimeAdd = 1 + (Tuesday_Rand(R) % 3);
			if (PS->Chip2.len == 0)
			{
				PS->Chip2.ChordSeed = Tuesday_Rand(R);

				PS->Chip2.chordscaler = (Tuesday_Rand(R) % 3) + 2;
				PS->Chip2.offset = (Tuesday_Rand(R) % 5);
				PS->Chip2.len = ((Tuesday_Rand(R) & 0x3) + 1)*2;

				if (Tuesday_BoolChance(R))
				{
					PS->Chip2.dir = Tuesday_BoolChance(R) ? Tuesday_BoolChance(R) : 0;
				}
			}
			Tuesday_RandomSeed(&PS->Chip2.R, PS->Chip2.ChordSeed);

		}

		int scaleidx = ((PS->Chip2.idx) % PS->Chip2.ChordLen);
		
		if (PS->Chip2.dir)
		{
			scaleidx = PS->Chip2.ChordLen - scaleidx - 1;
		}
		SN.oct = 0;
		SN.note = (scaleidx  * PS->Chip2.chordscaler) + PS->Chip2.offset;
		
		PS->Chip2.idx++;
		PS->Chip2.DeadTime = PS->Chip2.TimeMult;
		if (PS->Chip2.DeadTime > 0)
		{
			Output->maxsubticklength = ((1 + PS->Chip2.TimeMult) * TUESDAY_SUBTICKRES) - 2;
		}
		PS->Chip2.DeadTime += DeadTimeAdd;
	}
	Output->note = ScaleToNote(&SN, T, P, S);

	Output->vel = (Tuesday_RandByte(&PS->Chip2.R));
	
}
