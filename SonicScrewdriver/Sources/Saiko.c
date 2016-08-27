#include "Saiko.h"
#include <math.h>
#include <stdlib.h>     /* srand, rand */

long holdrand = 0x1235;

void  szrand (unsigned int seed)
{

        holdrand = (long)seed;

}

int  zrand (void)
{

    return(((holdrand = holdrand * 214013L + 2531011L) >> 16) & 0x7fff);

}


float RAND1()
{

	return zrand() / (float)(0x7fff);
}

void PatternGen_Goa(struct PatternTarget *T)
{
	for (int i = 0;i < 256;i++)
	{
		T->Ticks[i].vel = RAND1();
		T->Ticks[i].accent = (RAND1()> 0.5) ? 1 : 0;
		
		int RandNote = zrand() % 8;
		switch (RandNote)
		{
		case 0:
		case 2:T->Ticks[i].note = 0;break;
		case 1:T->Ticks[i].note = (char)0xf4;break;
		case 3:T->Ticks[i].note = 1;break;
		case 4:T->Ticks[i].note = 3;break;
		case 5:T->Ticks[i].note = 7;break;
		case 6:T->Ticks[i].note = 0xc;break;
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
