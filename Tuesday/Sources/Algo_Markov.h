#include "Algo.h"

void Algo_Markov_Init( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *Output)
{
	Output->Markov.NoteHistory1 = (Tuesday_Rand(R) & 0x7);
	Output->Markov.NoteHistory3 = (Tuesday_Rand(R) & 0x7);
	

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Output->Markov.matrix[i][j][0] = (Tuesday_Rand(R) % 8);
			Output->Markov.matrix[i][j][1] = (Tuesday_Rand(R) % 8);
		}
	}
}

void Algo_Markov_Gen( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *PS, int I,  Tuesday_Tick_t *Output)
{

	struct ScaledNote SN;
	DefaultTick(Output);

	int idx = Tuesday_BoolChance(R) == 1 ? 1 : 0;
	SN.note = PS->Markov.matrix[PS->Markov.NoteHistory1][PS->Markov.NoteHistory3][idx];
	PS->Markov.NoteHistory1 = PS->Markov.NoteHistory3;
	PS->Markov.NoteHistory3 = SN.note;

	SN.oct = Tuesday_BoolChance(R);
	RandomSlideAndLength(Output, R);
	Output->note = ScaleToNote(&SN, T, P, S);
	Output->accent = Tuesday_PercChance(R, 100);
	Output->vel = (Tuesday_Rand(R) / 2) + 40;
}
