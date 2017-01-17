
#include "../../EurorackShared/EURORACKSHARED.H"


void Algo_Wobble_Init(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *Output)
{
	Tuesday_RandomSeed(R, T->seed2 >> 2);
	Tuesday_RandomSeed(&Output->ExtraRandom, T->seed1 >> 2);
	Output->Wobble.Phase = 0;
	Output->Wobble.PhaseSpeed = 0xffffffff/T->CurrentPattern.Length;
	Output->Wobble.Phase2 = 0;
	Output->Wobble.LastWasHigh = 0;
	Output->Wobble.PhaseSpeed2 = 0xcfffffff / (T->CurrentPattern.Length/4);
}

void Algo_Wobble_Gen(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *PS, int I, struct Tuesday_Tick *Output)
{

	unsigned char accentoffs = 0;
	unsigned char veloffset = 0;

	struct ScaledNote SN;
	Output->slide = 0;
	PS->Wobble.Phase += PS->Wobble.PhaseSpeed;
	PS->Wobble.Phase2 += PS->Wobble.PhaseSpeed2;
	
	if (Tuesday_PercChance(R, T->seed2))
	{

		int32_t pos2 = BasicShapes(PS->Wobble.Phase2, T->seed1);
		NOTE(1, (pos2 >> 27));
		if (PS->Wobble.LastWasHigh == 0) 
		{
			if (Tuesday_PercChance(R, 200))
			{
				Output->slide = Tuesday_Rand(R) % 3;
			}
		}
		PS->Wobble.LastWasHigh = 1;
	}
	else
	{
		int32_t pos = BasicShapes(PS->Wobble.Phase, T->seed1);
		NOTE(0, (pos >> 27));
		if (PS->Wobble.LastWasHigh == 1)
		{
			if (Tuesday_PercChance(R, 200))
			{
				Output->slide = Tuesday_Rand(R) % 3;
			}
		}
		PS->Wobble.LastWasHigh = 0;
	}

	int32_t n = ScaleToNote(&SN, T, P, S);
	Output->slide = 0;
	Output->note = n;
	Output->vel = (Tuesday_Rand(&PS->ExtraRandom) / 4) + veloffset;
	Output->accent = Tuesday_PercChance(R, 50 + accentoffs);


}
