#include "Algo.h"

uint8_t Tuesday_RandByte(struct Tuesday_RandomGen *R)
{
	return (Tuesday_Rand(R) >> 7) & 0xff;
}

uint8_t Tuesday_BoolChance(struct Tuesday_RandomGen *R)
{
	int r = Tuesday_Rand(R);
	return ((((r >> 13)) & 1) == 1) ? 1 : 0;
}

uint8_t Tuesday_PercChance(struct Tuesday_RandomGen *R, uint8_t perc)
{
	int Res = (Tuesday_Rand(R) >> 6);
	if ((Res & 0xff) >= perc) return 1;
	return 0;
}

void Pattern_Reverse(struct Tuesday_PatternContainer *T, int first, int length)
{
	int last = first + length;
	if (last > T->Length) last = T->Length;
	for (int j = first; j < last - 1; j++)
	{
		Pattern_Rotate(T, j, last - j, 1);
	}
}

void Pattern_Rotate(struct Tuesday_PatternContainer *T, int first, int length, int rotate)
{
	int last = first + length;
	if (last > T->Length) last = T->Length;

	for (int i = 0; i < rotate; i++)
	{
		float V = T->Ticks[first].vel;
		int N = T->Ticks[first].note;
		int A = T->Ticks[first].accent;
		for (int j = first; j < last - 1; j++)
		{
			T->Ticks[j].vel = T->Ticks[j + 1].vel;
			T->Ticks[j].note = T->Ticks[j + 1].note;
			T->Ticks[j].accent = T->Ticks[j + 1].accent;
		}
		T->Ticks[last - 1].note = N;
		T->Ticks[last - 1].vel = V;
		T->Ticks[last - 1].accent = A;
	}
}

void Pattern_Transpose(struct Tuesday_PatternContainer *T, int first, int length, int transpose)
{
	int last = first + length;
	if (last > T->Length) last = T->Length;
	for (int i = first; i < last; i++)
	{
		T->Ticks[i].note += transpose;
	}
}

