#include "Algo.h"

void Algo_ChipArp_1_Init(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *Output)
{
	Output->b1 = (Tuesday_Rand(R) & 0x7);
	Output->b2 = (Tuesday_Rand(R) & 0x7);
	Output->b3 = (Tuesday_Rand(R) & 0x3) + 1;
	Output->b4 = (Tuesday_Rand(R) & 0x3) + 2;
}

void Algo_ChipArp_1_PatternInit(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_PatternContainer *PT)
{
	T->CurrentPattern.TPB *= 2;
}

void Algo_ChipArp_1_Gen(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *PS, int I, struct Tuesday_Tick *Output)
{
	struct ScaledNote SN;

	SN.note = ((I + PS->b1) % PS->b4) * 2;
	SN.oct = ((I + PS->b2) % PS->b3);

	Output->note = ScaleToNote(&SN, T, P, S);

	Output->vel = (Tuesday_RandByte(R) / 2) + (((I + PS->b2) == 0) ? 127 : 0);
	Output->accent = Tuesday_BoolChance(R);
}
