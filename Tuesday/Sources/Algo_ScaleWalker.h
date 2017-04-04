#include "Algo.h"

void Algo_ScaleWalk_Init(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *Output)
{
	Output->ScaleWalk.WalkLen = 1 + ((T->seed1 * S->scales[S->scale[P->scale ] & 0xf].count * 3)/255);
	Output->ScaleWalk.Current = (Output->ScaleWalk.WalkLen * T->seed2) / 255;

}

void Algo_ScaleWalk_Gen(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *PS, int I, struct Tuesday_Tick *Output)
{
	struct ScaledNote SN;
	DefaultTick(Output);	
	NOTE(0,PS->ScaleWalk.Current);
	
	Output->vel = (Tuesday_Rand(R));
	Output->accent = Tuesday_BoolChance(R);
	Output->note = ScaleToNote(&SN, T, P, S);
	RandomSlideAndLength(Output, R);

	PS->ScaleWalk.Current = (PS->ScaleWalk.Current + 1) % PS->ScaleWalk.WalkLen;
}
