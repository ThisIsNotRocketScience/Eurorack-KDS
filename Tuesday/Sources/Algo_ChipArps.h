#include "Algo.h"

void Algo_ChipArp_1_Init(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *Output)
{
	Output->Chip1.ChordSeed = Tuesday_Rand(R);
	Tuesday_RandomSeed(&Output->Chip1.R, Output->Chip1.ChordSeed);
	Output->Chip1.Base = Tuesday_Rand(R) % 3;
	Output->Chip1.Dir = Tuesday_RandByte(R) % 2;

}

void Algo_ChipArp_1_PatternInit(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_PatternContainer *PT)
{
	T->CurrentPattern.TPB *= 2;
}

void Algo_ChipArp_1_Gen(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *PS, int I, struct Tuesday_Tick *Output)
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
	Output->GENERIC.b1 = (Tuesday_Rand(R) & 0x7);
	
}


void Algo_ChipArp_2_PatternInit(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_PatternContainer *PT)
{
	T->CurrentPattern.TPB *= 2;
}







void Algo_ChipArp_2_Gen(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *PS, int I, struct Tuesday_Tick *Output)
{
	struct ScaledNote SN;
	DefaultTick(Output);
	SN.note = ((I + PS->GENERIC.b1) % PS->GENERIC.b4) * 2;
	SN.oct = ((I + PS->GENERIC.b2) % PS->GENERIC.b3);

	Output->note = ScaleToNote(&SN, T, P, S);

	Output->vel = (Tuesday_RandByte(R) / 2) + (((I + PS->GENERIC.b2) == 0) ? 127 : 0);
	Output->accent = Tuesday_BoolChance(R);
}
