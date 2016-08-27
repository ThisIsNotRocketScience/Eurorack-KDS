#include "Saiko.h"
#include <stdint.h>

long RandomMemory= 0x1235;

void  szrand (unsigned int seed)
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

void PatternGen_Psych(struct PatternTarget *T)
{
	for (int i = 0; i < SAIKO_MAXTICK; i++)
	{
		T->Ticks[i].vel = (RAND1() - 0.5) * 0.1;

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

void PatternGen_Flat(struct PatternTarget *T)
{


	for (int i = 0; i < SAIKO_MAXTICK; i++)
	{


		T->Ticks[i].vel = RAND1() * 0.00025;

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

void PatternGen_Goa(struct PatternTarget *T)
{
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
		for (int i = 0;i < 7;i++)
		{
			T->Ticks[i].note += 3;
		}
	}

	if(RAND1() > 0.5)
	{
		for (int i = 0;i < 7;i++)
		{
			T->Ticks[i].note += -5;
		}
	}			
}
