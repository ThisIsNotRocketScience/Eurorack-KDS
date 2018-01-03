#include "Algo.h"

uint8_t Tuesday_RandByte( Tuesday_RandomGen *R)
{
	return (Tuesday_Rand(R) >> 7) & 0xff;
}

uint8_t NOINLINE Tuesday_BoolChance( Tuesday_RandomGen *R)
{
	int r = Tuesday_Rand(R);
	return ((((r >> 13)) & 1) == 1) ? 1 : 0;
}

uint8_t NOINLINE Tuesday_PercChance( Tuesday_RandomGen *R, uint8_t perc)
{
	int Res = (Tuesday_Rand(R) >> 6);
	if ((Res & 0xff) >= perc) return 1;
	return 0;
}

void Pattern_Reverse( Tuesday_PatternContainer *T, int first, int length)
{
	int last = first + length;
	if (last > T->Length) last = T->Length;
	for (int j = first; j < last - 1; j++)
	{
		Pattern_Rotate(T, j, last - j, 1);
	}
}

void Pattern_Rotate( Tuesday_PatternContainer *T, int first, int length, int rotate)
{
	int last = first + length;
	if (last > T->Length) last = T->Length;

	for (int i = 0; i < rotate; i++)
	{
		int V = T->Ticks[first].vel;
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

void NOINLINE Tuesday_RandomSeed( Tuesday_RandomGen *R, unsigned int seed)
{
	R->RandomMemory = (long)seed;
}

int NOINLINE Tuesday_Rand(Tuesday_RandomGen *R)
{
	return (((R->RandomMemory = R->RandomMemory * 214013L + 2531011L) >> 16) & 0x7fff);
}

int NOINLINE ScaleToNote( ScaledNote *SN,  Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S)
{
	if (SN->note == TUESDAY_NOTEOFF)
	{
		return TUESDAY_NOTEOFF;
	}
	int32_t octoffset = SN->oct;
	int32_t scaleidx = SN->note;
	//scaleidx &= 0xf;
	int32_t selectedscale = S->scale[  P->scale ]  & 0xf;
	int32_t selectedtranspose = S->scale[P->scale] >> 4;

	int32_t scalecount = S->scales[selectedscale].count;

	while (scaleidx < 0)
	{
		scaleidx += scalecount;
		octoffset--;
	};

	while (scaleidx >= scalecount)
	{
		scaleidx -= scalecount; octoffset++;
	}
	unsigned int transpose[4] = { 0,5,7,12 };
	octoffset++;
	return S->scales[selectedscale].notes[scaleidx] + (12 * (octoffset)) + transpose[selectedtranspose];
}
