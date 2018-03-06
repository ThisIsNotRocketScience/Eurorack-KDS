
#include "../../EurorackShared/EURORACKSHARED.H"


void Algo_Wobble_Init( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *Output)
{
	Tuesday_RandomSeed(R, T->seed2 >> 2);
	Tuesday_RandomSeed(&Output->ExtraRandom, T->seed1 >> 2);
	Output->Wobble.Phase = 0;
	Output->Wobble.PhaseSpeed = 0xffffffff/T->CurrentPattern.Length;
	Output->Wobble.Phase2 = 0;
	Output->Wobble.LastWasHigh = 0;
	Output->Wobble.PhaseSpeed2 = 0xcfffffff / (__max(4, T->CurrentPattern.Length)/4);
}



void Algo_Wobble_Gen( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *PS, int I,  Tuesday_Tick_t *Output)
{

	unsigned char accentoffs = 0;
	unsigned char veloffset = 0;

	struct ScaledNote SN;
	DefaultTick(Output);
	

	PS->Wobble.Phase += PS->Wobble.PhaseSpeed;
	PS->Wobble.Phase2 += PS->Wobble.PhaseSpeed2;
	
	if (Tuesday_PercChance(R, T->seed2))
	{

		int32_t pos2 = UncompensatedBasicShapes(PS->Wobble.Phase2, T->seed1);
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
		int32_t pos = UncompensatedBasicShapes(PS->Wobble.Phase, T->seed1);
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
//	Output->slide = 0;
	Output->note = n;
	Output->vel = (Tuesday_Rand(&PS->ExtraRandom) / 4) + veloffset;
	Output->accent = Tuesday_PercChance(R, 50 + accentoffs);


}
