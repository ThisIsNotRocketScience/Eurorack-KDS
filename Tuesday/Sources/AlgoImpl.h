#include "Algo.h"


#include "Algo_ChipArps.h"
#include "Algo_Markov.h"
#include "Algo_SaikoSet.h"
#include "Algo_Stomper.h"
#include "Algo_Test.h"
#include "Algo_TriTrance.h"
#include "Algo_Wobble.h"
#include "Algo_SNH.h"
#include "Algo_ScaleWalker.h"
#include "Algo_TooEasy.h"

void Algo_Init_Generic_FourBool( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *Output)
{
	Output->GENERIC.b1 = Tuesday_BoolChance(R);
	Output->GENERIC.b2 = Tuesday_BoolChance(R);
	Output->GENERIC.b3 = Tuesday_BoolChance(R);
	Output->GENERIC.b4 = Tuesday_BoolChance(R);
}

void NoInit( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *Output)
{
}

void NoPattern( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *PS, int I,  Tuesday_Tick_t *Output)
{
}

void NoPatternInit( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_PatternContainer *PT)
{
}

