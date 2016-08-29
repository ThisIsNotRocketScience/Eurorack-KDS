#include "Saiko.h"
#include <stdint.h>

long RandomMemory= 0x1235;

void  ZRANDOMSEED (unsigned int seed)
{
	RandomMemory = (long)seed;
}

int  zrand (void)
{
	return(((RandomMemory = RandomMemory * 214013L + 2531011L) >> 16) & 0x7fff);
}

float RAND1()
{
	return zrand() / (float)(0x7fff);
}

void Reverse(struct PatternTarget *T, int first, int length)
{
	int last = first + length;
	if (last > T->Length) last = T->Length;
	for(int j = first;j<last-1;j++)
	{
		Rotate(T, j, last-j,1);
	}
}

void Rotate(struct PatternTarget *T, int first, int length, int rotate)
{
	int last = first + length;
	if (last > T->Length) last = T->Length;

	for (int i =0 ;i<rotate;i++)
	{
		float V = T->Ticks[first].vel;
		int N = T->Ticks[first].note;
		int A = T->Ticks[first].accent;
		for (int j= first;j<last-1;j++)
		{
			T->Ticks[j].vel = T->Ticks[j+1].vel;
			T->Ticks[j].note= T->Ticks[j+1].note;
			T->Ticks[j].accent = T->Ticks[j+1].accent;
		}
		T->Ticks[last-1].note =N;
		T->Ticks[last-1].vel =V;
		T->Ticks[last-1].accent =A;
	}
}

void Transpose(struct PatternTarget *T, int first, int length, int transpose)
{
	int last = first + length;
	if (last > T->Length) last = T->Length;
	for(int i = first;i<last;i++)
	{
		T->Ticks[i].note += transpose;
	}
}

void PatternGen_Psych(struct PatternTarget *T, int Length)
{
	if (Length> SAIKO_MAXTICK) Length = SAIKO_MAXTICK;

	T->Length = Length;
	T->TPB = 4;
	for (int i = 0; i < SAIKO_MAXTICK; i++)
	{
		T->Ticks[i].vel = RAND1() ;

		uint64_t a = zrand() & 32767;
		a *= 0x57619F1ULL;
		a = (a>>(32+9)) + ((uint32_t)a >> 31);

		T->Ticks[i].accent = (a != 0);

		int r = (zrand() & 32767) >> 12;
		switch (r)
		{
		case 0:
		case 1:
		case 6:
			T->Ticks[i].note = 0;
			break;
		case 2:
			T->Ticks[i].note = -2;
			break;
		case 3:
			T->Ticks[i].note = 3;
			break;
		case 4:
			T->Ticks[i].note = 15;
			break;
		case 5:
			T->Ticks[i].note = 10;
			break;
		case 7:
			T->Ticks[i].note = 12;
			break;
		default:
			// don't change note value!
			break;
		}

	}
}

void PatternGen_Flat(struct PatternTarget *T, int Length)
{
	if (Length> SAIKO_MAXTICK) Length = SAIKO_MAXTICK;

	T->TPB = 4;
	T->Length = Length;
	for (int i = 0; i < SAIKO_MAXTICK; i++)
	{


		T->Ticks[i].vel = RAND1() ;

		uint64_t a = zrand() & 32767;
		a *= 0x57619F1ULL;
		a = (a>>(32+9)) + ((uint32_t)a >> 31);

		T->Ticks[i].accent = (a != 0);

		int r = (zrand() & 32767) >> 12;
		switch (r)
		{
		case 0:
		case 2:
		case 4:
		case 5:
		case 6:
			T->Ticks[i].note = 0;break;
		case 1: T->Ticks[i].note = -12;break;
		case 3: T->Ticks[i].note = 0xD;break;
		case 7: T->Ticks[i].note = 0xC;break;
		default:
			// don't change note value!
			break;
		}

	}

};

void PatternGen_Zeph(struct PatternTarget *T, int stepsperbeat, int beats, int fullcycles)
{
	int totalticks = stepsperbeat * beats * fullcycles;
	int subpattern = stepsperbeat * beats;
		if (totalticks > SAIKO_MAXTICK)
	{
		return;
	}
	PatternGen_Goa(T, subpattern  );
	T->Length = totalticks;
	T->TPB = stepsperbeat;
	for (int i =1;i<fullcycles;i++)
	{
		for(int j = 0;j<subpattern;j++)
		{
			T->Ticks[i*subpattern + j].note = T->Ticks[j].note;
			T->Ticks[i*subpattern + j].vel= T->Ticks[j].vel;
			T->Ticks[i*subpattern + j].accent = T->Ticks[j].accent;
		}
		if (RAND1() > 0.5)
		{
			Rotate(T, (fullcycles-1)*subpattern,subpattern,3);
		}
		if (RAND1() > 0.5)
				{
					Reverse(T, (fullcycles-1)*subpattern + subpattern/2,subpattern/2);
				}
	}
	if (RAND1() > 0.5) Transpose(T, (fullcycles-1)*subpattern,subpattern,3);
	if (RAND1() > 0.5) Transpose(T, 2*subpattern,7,-5);
}


void PatternGen_Goa(struct PatternTarget *T, int Length)
{
	if (Length> SAIKO_MAXTICK) Length = SAIKO_MAXTICK;
	T->TPB = 4;
	T->Length = Length;
	for (int i = 0;i < SAIKO_MAXTICK;i++)
	{
		T->Ticks[i].vel = RAND1();
		T->Ticks[i].accent = (RAND1()> 0.5) ? 1 : 0;

		int RandNote = zrand() % 8;
		switch (RandNote)
		{
		case 0:
		case 2:
			T->Ticks[i].note = 0;break;
		case 1: T->Ticks[i].note = (char)0xf4;break;
		case 3: T->Ticks[i].note = 1;break;
		case 4: T->Ticks[i].note = 3;break;
		case 5: T->Ticks[i].note = 7;break;
		case 6: T->Ticks[i].note = 0xc;break;
		case 7: T->Ticks[i].note = 0xd;break;
		}

		if (T->Ticks[i].accent)
		{
			switch (RandNote)
			{
			case 0:
			case 3:
			case 7: T->Ticks[i].note = 0;break;
			case 1:T->Ticks[i].note = (char)0xf4;break;
			case 2:T->Ticks[i].note = (char)0xfe;break;
			case 4:T->Ticks[i].note = 3;break;
			case 5:T->Ticks[i].note = (char)0xf2;break;
			case 6:T->Ticks[i].note = 1;break;
			}
		}

	}


	if (RAND1() > 0.5)
	{
		Transpose(T, 0,7,3);
	}

	if(RAND1() > 0.5)
	{
		Transpose(T, 0,7,-5);
	}			
}
