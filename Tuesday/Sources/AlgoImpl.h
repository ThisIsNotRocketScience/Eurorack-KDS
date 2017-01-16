#include "Algo.h"

#include "Algo_ChipArps.h"
#include "Algo_Markov.h"
#include "Algo_SaikoSet.h"
#include "Algo_Stomper.h"
#include "Algo_Test.h"
#include "Algo_TriTrance.h"

void Algo_Init_Generic_FourBool(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *Output)
{
	Output->b1 = Tuesday_BoolChance(R);
	Output->b2 = Tuesday_BoolChance(R);
	Output->b3 = Tuesday_BoolChance(R);
	Output->b4 = Tuesday_BoolChance(R);
}

void NoInit(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *Output) 
{
}

void NoPattern(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *PS, int I, struct Tuesday_Tick *Output) 
{
}

void NoPatternInit(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_PatternContainer *PT) 
{
}

int ScaleToNote(struct ScaledNote *SN, struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S)
{
	int32_t octoffset = SN->oct;
	int32_t scaleidx = SN->note;
	//scaleidx &= 0xf;
	int32_t selectedscale = P->scale;
	int32_t scalecount = S->scalecount[selectedscale];

	while (scaleidx < 0)
	{
		scaleidx += scalecount;
		octoffset--;
	};

	while (scaleidx >= scalecount)
	{
		scaleidx -= scalecount; octoffset++;
	}

	octoffset++;
	return S->scale[selectedscale][scaleidx] + (12 * (octoffset));
}
