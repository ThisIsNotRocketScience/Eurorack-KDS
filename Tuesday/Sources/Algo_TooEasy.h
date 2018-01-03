#include "Algo.h"

#pragma GCC push_options
#pragma GCC optimize ("Os")

#define swap(x,y) {int t = x;x = y;y = t;};
void generate(int n, unsigned char *A, int stop)
{
	if (stop == 0) return;


	unsigned char c[8];

	for (int i = 0; i < n; i++)
	{
		c[i] = 0;
	}


	int i = 0;
	int cnt = 0;
	while (i < n) {
		if (c[i] < i)
		{
			if ((i & 1) == 0)
			{
				swap(A[0], A[i]);
			}
			else
			{
				swap(A[c[i]], A[i]);
			}
			
			if (cnt == stop) return;
			cnt++;
			c[i] += 1;
			i = 0;
		}
		else
		{
			c[i] = 0;
			i++;

		}
	}

}

int fact(int i)
{
	if (i <= 2) return 2;
	return i * fact(i - 1);
}

void Algo_TooEasy_Init( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *Output)
{
	Output->TooEasy.WalkLen = T->CurrentPattern.TPB;
	if (Output->TooEasy.WalkLen > 5) Output->TooEasy.WalkLen = 5;
	Tuesday_RandomSeed(&Output->ExtraRandom, T->seed2 >> 2);
	
	for (int i = 0; i < Output->TooEasy.WalkLen; i++)
	{
		int V = ((i * 200) / Output->TooEasy.WalkLen);
		Output->TooEasy.Pattern[i] = 200 - V;
	}
	int perm = (T->seed1 * fact(Output->TooEasy.WalkLen)) / 255;
	int v = 0;
	generate(Output->TooEasy.WalkLen, Output->TooEasy.Pattern, perm);

	Output->TooEasy.Current = 0;
}

#pragma GCC pop_options
#pragma GCC optimize ("O3")

void Algo_TooEasy_Gen( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *PS, int I,  Tuesday_Tick_t *Output)
{
	struct ScaledNote SN;
	DefaultTick(Output);

	PS->TooEasy.Current = (PS->TooEasy.Current + 1) % (PS->TooEasy.WalkLen);
	if (PS->TooEasy.Current >= T->CurrentPattern.TPB) PS->TooEasy.Current = 0;
	int oct = 0;
	if (Tuesday_PercChance(&PS->ExtraRandom, 220))
	{

		oct++;
	}

	NOTE(oct, 0);
	Output->vel = 50 + PS->TooEasy.Pattern[PS->TooEasy.Current];
	Output->accent = Tuesday_BoolChance(R);

	RandomSlideAndLength(Output, R);

	Output->note = ScaleToNote(&SN, T, P, S);

}
