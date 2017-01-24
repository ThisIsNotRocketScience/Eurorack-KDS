
#include "../../EurorackShared/EURORACKSHARED.H"


void Algo_SNH_Init(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *Output)
{
	Tuesday_RandomSeed(R, T->seed2 >> 4);
	Tuesday_RandomSeed(&Output->ExtraRandom, T->seed1 >> 4);
	ResetSVF(&Output->SNH.Filt);
	Output->SNH.Current =  Output->SNH.Target = Tuesday_Rand(R)<<10;
	Output->SNH.CurrentDelta = 0;
	Output->SNH.Phase = 0;
	Output->SNH.PhaseSpeed = 0xffffffff/(T->CurrentPattern.TPB* S->beatoptions[P->beatopt]);	
}

void Algo_SNH_Gen(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *PS, int I, struct Tuesday_Tick *Output)
{
	unsigned char accentoffs = 0;
	unsigned char veloffset = 0;

	struct ScaledNote SN;
	DefaultTick(Output);


	PS->SNH.Phase += PS->SNH.PhaseSpeed;
	int V = PS->SNH.Phase >> 30;

	if (V != PS->SNH.LastVal)
	{
		PS->SNH.LastVal = V;	
		PS->SNH.Target = Tuesday_Rand(R)<<10;
	}
	int newdelta = (PS->SNH.Target - PS->SNH.Current) / 100;
	PS->SNH.CurrentDelta += ((newdelta - PS->SNH.CurrentDelta) * 100) / (100 + T->seed1*4);
	PS->SNH.Current += PS->SNH.CurrentDelta * 100;
	NOTE(0, (abs(PS->SNH.Current )>> 22));

	int32_t n = ScaleToNote(&SN, T, P, S);
	Output->slide = 0;
	Output->note = n;
	Output->vel = (Tuesday_Rand(&PS->ExtraRandom) / 4) + veloffset;
	Output->accent = Tuesday_PercChance(R, 50 + accentoffs);
}
