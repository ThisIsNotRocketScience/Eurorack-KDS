
#include "../../EurorackShared/EURORACKSHARED.H"


void Algo_SNH_Init(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *Output)
{
	Tuesday_RandomSeed(R, T->seed2 >> 2);
	Tuesday_RandomSeed(&Output->ExtraRandom, T->seed1 >> 2);
	ResetSVF(&Output->SNH.Filt);
	Output->SNH.Current =  Output->SNH.Target = Tuesday_Rand(R);
	
	Output->SNH.Phase = 0;
	Output->SNH.PhaseSpeed = 0xffffffff/T->CurrentPattern.Length;
	
}

void Algo_SNH_Gen(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *PS, int I, struct Tuesday_Tick *Output)
{

	unsigned char accentoffs = 0;
	unsigned char veloffset = 0;

	struct ScaledNote SN;
	Output->slide = 0;
	PS->SNH.Phase += PS->SNH.PhaseSpeed;
	int V = PS->SNH.Phase >> 30;
	if (V != PS->SNH.LastVal)
	{
		PS->SNH.LastVal = V;	
		PS->SNH.Target = Tuesday_Rand(R)<<16;
	}

	
	PS->SNH.Current += (PS->SNH.Target - PS->SNH.Current) / 2;
	NOTE(0, (PS->SNH.Current >> 27));

	int32_t n = ScaleToNote(&SN, T, P, S);
	Output->slide = 0;
	Output->note = n;
	Output->vel = (Tuesday_Rand(&PS->ExtraRandom) / 4) + veloffset;
	Output->accent = Tuesday_PercChance(R, 50 + accentoffs);


}
