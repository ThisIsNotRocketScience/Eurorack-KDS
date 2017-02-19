#include "Algo.h"

void Algo_ScaleWalk_Init(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *Output)
{
	Output->ScaleWalk.WalkLen = 1 + (T->seed1 >> 5);
	Output->ScaleWalk.Current = (Output->ScaleWalk.WalkLen * 255) / (T->seed2);

}

void Algo_ScaleWalk_Gen(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *PS, int I, struct Tuesday_Tick *Output)
{
	struct ScaledNote SN;
	DefaultTick(Output);	
	NOTE(PS->ScaleWalk.Current);
	PS->ScaleWalk.Current = (PS->ScaleWalk.Current + 1) % PS->ScaleWalk.WalkLen;

}
