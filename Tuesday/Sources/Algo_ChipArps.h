#include "Algo.h"

void Algo_ChipArp_1_Init(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *Output)
{
	Output->GENERIC.b1 = (Tuesday_Rand(R) & 0x7);
	Output->GENERIC.b2 = (Tuesday_Rand(R) & 0x7);
	Output->GENERIC.b3 = (Tuesday_Rand(R) & 0x3) + 1;
	Output->GENERIC.b4 = (Tuesday_Rand(R) & 0x3) + 2;
}

void Algo_ChipArp_1_PatternInit(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_PatternContainer *PT)
{
	T->CurrentPattern.TPB *= 2;
}

void Algo_ChipArp_1_Gen(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *PS, int I, struct Tuesday_Tick *Output)
{
	struct ScaledNote SN;
	DefaultTick(Output);
	SN.note = ((I + PS->GENERIC.b1) % PS->GENERIC.b4) * 2;
	SN.oct = ((I + PS->GENERIC.b2) % PS->GENERIC.b3);

	Output->note = ScaleToNote(&SN, T, P, S);

	Output->vel = (Tuesday_RandByte(R) / 2) + (((I + PS->GENERIC.b2) == 0) ? 127 : 0);
	Output->accent = Tuesday_BoolChance(R);
}

void Algo_ChipArp_2_Init(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *Output)
{
	Output->GENERIC.b1 = (Tuesday_Rand(R) & 0x7);
	Output->GENERIC.b2 = (Tuesday_Rand(R) & 0x7);
	Output->GENERIC.b3 = (Tuesday_Rand(R) & 0x3) + 1;
	Output->GENERIC.b4 = (Tuesday_Rand(R) & 0x3) + 2;
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
