#include "PatternGen.h"
#include <stdint.h>

void Rotate(struct PatternGen_Target *T, int first, int length, int rotate);
void Reverse(struct PatternGen_Target *T, int first, int length);

void PatternGen_RandomSeed (struct PatternGen_Random *R, unsigned int seed)
{
	R->RandomMemory = (long)seed;
}

int PatternGen_Rand(struct PatternGen_Random *R)
{
	return (((R->RandomMemory = R->RandomMemory * 214013L + 2531011L) >> 16) & 0x7fff);
}

uint8_t PatternGen_RandByte(struct PatternGen_Random *R)
{
	return (PatternGen_Rand(R) >> 7) & 0xff;
}

uint8_t PatternGen_BoolChance(struct PatternGen_Random *R)
{
	int r = PatternGen_Rand(R);
	return ((((r >> 13 )) &1) == 1 ) ?1:0;
}

uint8_t PatternGen_PercChance(struct PatternGen_Random *R, uint8_t perc)
{
	int Res =(PatternGen_Rand(R)>>6);
	if ((Res &0xff) >= perc) return 1;
	return 0;
}

void PatternGen_LoadDefaults(struct PatternGen_Settings *S, struct PatternGen_Params *P)
{
	P->algo = 2;
	P->beatopt = 0;
	P->scale = 1;
	P->tpbopt = 2;

	for (int i =0 ;i<PATTERNGEN_MAXALGO;i++) S->algooptions[i] = i;
	S->algooptions[3] = 7;

	S->tpboptions[0] = 2;
	S->tpboptions[1] = 3;
	S->tpboptions[2] = 4;
	S->tpboptions[3] = 5;

	S->beatoptions[0] = 4;
	S->beatoptions[1] = 8;
	S->beatoptions[2] = 16;
	S->beatoptions[3] = 32;

	for (int j =0 ;j<PATTERNGEN_MAXSCALE;j++)
	{
		for(int i =0 ;i<12;i++)
		{
			S->scale[j][i] = i;
		}
		S->scalecount[j] = 1;
	}

	S->scale[0][0] = 0;
	S->scale[0][1] = 2;
	S->scale[0][2] = 4;
	S->scale[0][3] = 5;
	S->scale[0][4] = 7;
	S->scale[0][5] = 9;
	S->scale[0][6] = 11;
	S->scalecount[0] = 7; // Major scale

	S->scale[1][0] = 0;
	S->scale[1][1] = 2;
	S->scale[1][2] = 3;
	S->scale[1][3] = 5;
	S->scale[1][4] = 7;
	S->scale[1][5] = 8;
	S->scale[1][6] = 10;
	S->scalecount[1] = 7; // Minor scale

	S->scale[2][0] = 0;
	S->scale[2][1] = 2;
	S->scale[2][2] = 3;
	S->scale[2][3] = 6;
	S->scale[2][4] = 7;
	S->scale[2][5] = 9;
	S->scale[2][6] = 10;
	S->scalecount[2] = 7; // Dorian scale

	S->scale[3][0] = 0;
	S->scale[3][1] = 2;
	S->scale[3][2] = 5;
	S->scale[3][3] = 7;
	S->scale[3][4] = 9;
	S->scalecount[3] = 5; // Penta
}

void __attribute__ ((weak)) PatternGen_LoadSettings(struct PatternGen_Settings *S, struct PatternGen_Params *P)
{
	PatternGen_LoadDefaults(S,P);
}

void Reverse(struct PatternGen_Target *T, int first, int length)
{
	int last = first + length;
	if (last > T->Length) last = T->Length;
	for(int j = first;j<last-1;j++)
	{
		Rotate(T, j, last-j,1);
	}
}

void Rotate(struct PatternGen_Target *T, int first, int length, int rotate)
{
	int last = first + length;
	if (last > T->Length) last = T->Length;

	for (int i = 0 ;i < rotate;i++)
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

void Transpose(struct PatternGen_Target *T, int first, int length, int transpose)
{
	int last = first + length;
	if (last > T->Length) last = T->Length;
	for(int i = first;i<last;i++)
	{
		T->Ticks[i].note += transpose;
	}
}

void PatternGen_Psych(struct PatternGen_Target *T, struct PatternGen_Random *R, int Length)
{
	if (Length> PATTERNGEN_MAXTICK) Length = PATTERNGEN_MAXTICK;

	T->Length = Length;
	T->TPB = 4;

	for (int i = 0; i < PATTERNGEN_MAXTICK; i++)
	{
		T->Ticks[i].vel = PatternGen_RandByte(R) ;

		uint64_t a = PatternGen_Rand(R) & 32767;
		a *= 0x57619F1ULL;
		a = (a>>(32+9)) + ((uint32_t)a >> 31);

		T->Ticks[i].accent = (a != 0);

		int r = (PatternGen_Rand(R) & 32767) >> 12;
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

void PatternGen_Flat(struct PatternGen_Target *T, struct PatternGen_Random *R, int Length)
{
	if (Length> PATTERNGEN_MAXTICK) Length = PATTERNGEN_MAXTICK;

	T->TPB = 4;
	T->Length = Length;
	for (int i = 0; i < PATTERNGEN_MAXTICK; i++)
	{


		T->Ticks[i].vel = PatternGen_RandByte(R) ;

		uint64_t a = PatternGen_Rand(R) & 32767;
		a *= 0x57619F1ULL;
		a = (a>>(32+9)) + ((uint32_t)a >> 31);

		T->Ticks[i].accent = (a != 0);

		int r = (PatternGen_Rand(R) & 32767) >> 12;
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

void PatternGen_Goa(struct PatternGen_Target *T, struct PatternGen_Random *R, int Length)
{
	if (Length> PATTERNGEN_MAXTICK) Length = PATTERNGEN_MAXTICK;
	T->TPB = 4;
	T->Length = Length;
	for (int i = 0;i < PATTERNGEN_MAXTICK;i++)
	{
		T->Ticks[i].vel = PatternGen_RandByte(R);
		T->Ticks[i].accent =  (PatternGen_BoolChance(R)) ? 1 : 0;

		int RandNote = PatternGen_Rand(R) % 8;
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

	if (PatternGen_BoolChance(R))
	{
		Transpose(T, 0,7,3);
	}

	if (PatternGen_BoolChance(R))
	{
		Transpose(T, 0,7,-5);
	}			
}

void PatternGen_Zeph(struct PatternGen_Target *T, struct PatternGen_Random *R, int stepsperbeat, int beats, int fullcycles)
{
	int totalticks = stepsperbeat * beats * fullcycles;
	int subpattern = stepsperbeat * beats;
	if (totalticks > PATTERNGEN_MAXTICK)
	{
		return;
	}
	PatternGen_Goa(T, R, subpattern  );
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
		if (PatternGen_PercChance(R, 128))
		{
			Rotate(T, (fullcycles-1)*subpattern,subpattern,3);
		}
		if (PatternGen_PercChance(R, 128))
		{
			Reverse(T, (fullcycles-1)*subpattern + subpattern/2,subpattern/2);
		}
	}
	if (PatternGen_PercChance(R, 128)) Transpose(T, (fullcycles-1)*subpattern,subpattern,3);
	if (PatternGen_PercChance(R, 128)) Transpose(T, 2*subpattern,7,-5);
}

unsigned char dither[24*3] =
{
		0b0001, 0b0011, 0b0111,
		0b0001, 0b0011, 0b1011,
		0b0001, 0b0101, 0b0111,
		0b0001, 0b0101, 0b1101,
		0b0001, 0b1001, 0b1011,
		0b0001, 0b1001, 0b1101,
		0b0010, 0b0011, 0b0111,
		0b0010, 0b0011, 0b1011,
		0b0010, 0b0110, 0b0111,
		0b0010, 0b0110, 0b1110,
		0b0010, 0b1010, 0b1011,
		0b0010, 0b1010, 0b1110,
		0b0100, 0b0101, 0b0111,
		0b0100, 0b0101, 0b1101,
		0b0100, 0b0110, 0b0111,
		0b0100, 0b0110, 0b1110,
		0b0100, 0b1100, 0b1101,
		0b0100, 0b1100, 0b1110,
		0b1000, 0b1001, 0b1011,
		0b1000, 0b1001, 0b1101,
		0b1000, 0b1010, 0b1011,
		0b1000, 0b1010, 0b1110,
		0b1000, 0b1100, 0b1101,
		0b1000, 0b1100, 0b1110
};

struct ScaledNote
{
	int32_t oct;
	int32_t note;
};

int ScaleToNote(struct ScaledNote *SN, struct PatternGen_Params *P, struct PatternGen_Settings *S)
{
	int32_t octoffset = SN->oct;
	int32_t scaleidx = SN->note;
	//scaleidx &= 0xf;
	int32_t selectedscale = P->scale;
	int32_t scalecount = S->scalecount[selectedscale];

	while (scaleidx<0)
	{
		scaleidx+= scalecount;
		octoffset --;
	};

	while(scaleidx >= scalecount)
	{
		scaleidx-= scalecount;octoffset ++;
	}

	octoffset++;
	return S->scale[selectedscale][scaleidx] + (12 * (octoffset));
}

#define NOTE(aoct, anote) { SN.note = anote;SN.oct = aoct;};

void ClassicPattern(struct PatternGen_Params *P, struct PatternGen_Settings *S, struct PatternGen_Random *R, struct PatternGen_PatternFuncSpecific *PS, int I, struct PatternGen_Tick *Output)
{

	int note = 0;
	struct ScaledNote SN;

	int RandNote = PatternGen_Rand(R) % 8;

	switch (RandNote)
	{
	case 0:
	case 2: NOTE( 0, 0);break;
	case 1: NOTE(-1, 0);break; // -12
	case 3: NOTE( 0, 1);break;
	case 4: NOTE( 0, 2);break;
	case 5: NOTE( 0, 4);break;
	case 6: NOTE( 1, 0);break; // 12
	case 7: NOTE( 1, 1);break; // 13
	}

	if (Output->accent)
	{
		switch (RandNote)
		{
		case 0:
		case 3:
		case 7:NOTE( 0, 0);break;
		case 1:NOTE(-1, 0);break;
		case 2:NOTE( 0,-1);break;
		case 4:NOTE( 0, 1);break;
		case 5:NOTE(-1,-1);break;
		case 6:NOTE( 0, 1);break;
		}
	}

	if (I<7)
	{
		if (PS->b1 == 0) SN.note += 1;
		if (PS->b2 == 0) SN.note -= 2;
	}

	Output->vel = PatternGen_RandByte(R);
	Output->accent = PatternGen_BoolChance(R);
	Output->note = ScaleToNote(&SN, P,S);

}

void Chip1(struct PatternGen_Params *P, struct PatternGen_Settings *S, struct PatternGen_Random *R, struct PatternGen_PatternFuncSpecific *PS, int I, struct PatternGen_Tick *Output)
{
	struct ScaledNote SN;

	SN.note = ((I+PS->b1) % PS->b4)*2;
	SN.oct = ((I+PS->b2) % PS->b3);

	Output->note = ScaleToNote(&SN, P,S);

	Output->vel = (PatternGen_RandByte(R)/2) + (((I+PS->b2)==0)?127:0);
	Output->accent = PatternGen_BoolChance(R);

}

void TranceThing(struct PatternGen_Params *P, struct PatternGen_Settings *S, struct PatternGen_Random *R, struct PatternGen_PatternFuncSpecific *PS, int I, struct PatternGen_Tick *Output)
{
	struct ScaledNote SN;
	int veloffs = 0;
	int accentoffs = 0;
	switch((I + PS->b2) % 3)
	{
		case 0:
			if ((PatternGen_BoolChance(&PS->ExtraRandom)==1) &&(PatternGen_BoolChance(&PS->ExtraRandom)==1))
			{
				PS->b3 = (PatternGen_Rand(&PS->ExtraRandom) &0x15);
				if (PS->b3 >= 7) PS->b3 -= 7;else PS->b3 = 0;
				PS->b3 -= 4;
			}
			NOTE(0,PS->b3);break;
		case 1:NOTE(1,PS->b3);
				if (PatternGen_BoolChance(R)==1)  PS->b2 = (PatternGen_Rand(R) &0x7) ;
		break;
		case 2:
			NOTE(2,PS->b1);
			veloffs = 127;
			accentoffs = 127;
			if (PatternGen_BoolChance(R)==1)
			{
				PS->b1 = ((PatternGen_Rand(R)>>5)&0x7);

			};break;
	}

	int32_t n = ScaleToNote(&SN, P,S);
	Output->note = n;

	Output->vel = (PatternGen_Rand(R)/2) + veloffs;
	Output->accent = PatternGen_PercChance(R, 100 + accentoffs);


}


void Pattern4(struct PatternGen_Params *P, struct PatternGen_Settings *S, struct PatternGen_Random *R, struct PatternGen_PatternFuncSpecific *PS, int I, struct PatternGen_Tick *Output)
{

	struct ScaledNote SN;
	int idx = PatternGen_BoolChance(R)==1?1:0;
	SN.note = PS->matrix[PS->b1][PS->b3][idx];
	PS->b1 = PS->b3;
	PS->b3 = SN.note;

	SN.oct = PatternGen_BoolChance(R);



	Output->note = ScaleToNote(&SN, P,S);
	Output->accent = 0;
	Output->vel = 255;
}
void Pattern5(struct PatternGen_Params *P, struct PatternGen_Settings *S, struct PatternGen_Random *R, struct PatternGen_PatternFuncSpecific *PS, int I, struct PatternGen_Tick *Output){}
void Pattern6(struct PatternGen_Params *P, struct PatternGen_Settings *S, struct PatternGen_Random *R, struct PatternGen_PatternFuncSpecific *PS, int I, struct PatternGen_Tick *Output){}
void Pattern7(struct PatternGen_Params *P, struct PatternGen_Settings *S, struct PatternGen_Random *R, struct PatternGen_PatternFuncSpecific *PS, int I, struct PatternGen_Tick *Output){}
void TestPattern(struct PatternGen_Params *P, struct PatternGen_Settings *S, struct PatternGen_Random *R, struct PatternGen_PatternFuncSpecific *PS, int I, struct PatternGen_Tick *Output)
{
	struct ScaledNote SN;

	NOTE((I%5),0);

	Output->note = ScaleToNote(&SN, P,S);
	Output->accent = PatternGen_BoolChance(R);
	Output->vel = PatternGen_Rand(R);
}

typedef void (*GenFuncPtr)(struct PatternGen_Params *P, struct PatternGen_Settings *S, struct PatternGen_Random *R, struct PatternGen_PatternFuncSpecific *PS, int I, struct PatternGen_Tick *Output);
typedef void (*InitFuncPtr)(struct PatternGen_Params *P, struct PatternGen_Settings *S,struct PatternGen_Random *R, struct PatternGen_PatternFuncSpecific *Output);
typedef void (*PatternInitFuncPtr)(struct PatternGen_Params *P, struct PatternGen_Settings *S,struct PatternGen_Target *T);

void NoInit(struct PatternGen_Params *P, struct PatternGen_Settings *S, struct PatternGen_Random *R, struct PatternGen_PatternFuncSpecific *Output)
{

}

void MarkovInit(struct PatternGen_Params *P, struct PatternGen_Settings *S, struct PatternGen_Random *R, struct PatternGen_PatternFuncSpecific *Output)
{
	Output->b1 = (PatternGen_Rand(R) &0x7);
	Output->b2 = (PatternGen_Rand(R) &0x7);
	Output->b3 = (PatternGen_Rand(R) &0x7);
	Output->b4 = (PatternGen_Rand(R) &0x7);

	for (int i = 0;i<8;i++)
	{
		for (int j = 0;j<8;j++)
		{
			Output->matrix[i][j][0] = (PatternGen_Rand(R) %8);
			Output->matrix[i][j][1] = (PatternGen_Rand(R) %8);
		}
	}
}

void TranceThingInit(struct PatternGen_Params *P, struct PatternGen_Settings *S, struct PatternGen_Random *R, struct PatternGen_PatternFuncSpecific *Output)
{

	PatternGen_RandomSeed(R, P->seed1>>3);
	PatternGen_RandomSeed(&Output->ExtraRandom, P->seed2>>3);

	Output->b1 = (PatternGen_Rand(R) &0x7) ;
	Output->b2 = (PatternGen_Rand(R) &0x7) ;

	Output->b3 = (PatternGen_Rand(&Output->ExtraRandom) &0x15);
	if (Output->b3 >= 7) Output->b3 -= 7;else Output->b3 = 0;
	Output->b3 -= 4;

	Output->b4 = 0;
}

void ChipInit(struct PatternGen_Params *P, struct PatternGen_Settings *S, struct PatternGen_Random *R, struct PatternGen_PatternFuncSpecific *Output)
{
	Output->b1 = (PatternGen_Rand(R) &0x7) ;
	Output->b2 = (PatternGen_Rand(R) &0x7) ;
	Output->b3 = (PatternGen_Rand(R) &0x3) + 1;
	Output->b4 = (PatternGen_Rand(R) &0x3) + 2;
}

void FourBool(struct PatternGen_Params *P, struct PatternGen_Settings *S, struct PatternGen_Random *R, struct PatternGen_PatternFuncSpecific *Output)
{
	Output->b1 = PatternGen_BoolChance(R);
	Output->b2 = PatternGen_BoolChance(R);
	Output->b3 = PatternGen_BoolChance(R);
	Output->b4 = PatternGen_BoolChance(R);
}

void ChipPatternInit(struct PatternGen_Params *P, struct PatternGen_Settings *S,struct PatternGen_Target *T)
{
	T->TPB *= 2;
}

void NoPatternInit(struct PatternGen_Params *P, struct PatternGen_Settings *S,struct PatternGen_Target *T)
{

}



GenFuncPtr Funcs[8] = {&ClassicPattern,&Chip1,&TranceThing,&Pattern4,&Pattern5,&Pattern6,&Pattern7,&TestPattern};
InitFuncPtr InitFuncs[8] = {&FourBool,&ChipInit,&TranceThingInit,&MarkovInit,&NoInit,&NoInit,&NoInit,&NoInit};
PatternInitFuncPtr PatternInit[8] = {&NoPatternInit,&ChipPatternInit,&NoPatternInit,&NoPatternInit,&NoPatternInit,&NoPatternInit,&NoPatternInit,&NoPatternInit};

uint8_t FuncDither[8] = {1,1,1,1,1,1,1,1};

struct PatternGen_PatternFuncSpecific FuncSpecific[4];

struct PatternGen_Tick Ticks[4];
struct PatternGen_Tick Top;
struct PatternGen_Tick Bot;
struct PatternGen_Random Randoms[4];

void CopyTick(struct PatternGen_Tick *A, struct PatternGen_Tick *Out )
{
	Out->accent = A->accent;
	Out->note = A->note;
	Out->vel = A->vel;
}

void ApplyDither(int tick, uint32_t ditherpattern, struct PatternGen_Tick *A, struct PatternGen_Tick *B, struct PatternGen_Tick *Out )
{
	if ((ditherpattern >> (tick & 0b11)) & 1 == 1)
	{
		CopyTick(A, Out);
	}
	else
	{
		CopyTick(B, Out);
	}
}

void PatternGen_Generate(struct PatternGen_Target *T, struct PatternGen_Params *P, struct PatternGen_Settings *S)
{

	int len = S->tpboptions[P->tpbopt] * S->beatoptions[P->beatopt];
	T->Length = len;
	T->TPB = S->tpboptions[P->tpbopt];
	int X = P->seed1 >> 3;
	int Y = P->seed2 >> 3;

	int XFade = (P->seed1 & 0b110) >> 1;
	int YFade = (P->seed2 & 0b110) >> 1;

	unsigned char xbase = P->seed1 & 0b1111 + ((P->seed1)>>6) ;
	unsigned char ybase = P->seed2 & 0b1111 + ((P->seed2)>>6);

	unsigned char ditherx = 0;
	unsigned char dithery = 0;

	if (XFade > 0) ditherx = dither[xbase*3 + XFade-1];
	if (YFade > 0) dithery = dither[ybase*3 + YFade-1];


	PatternGen_RandomSeed(&Randoms[0], X + Y  * 32);
	PatternGen_RandomSeed(&Randoms[1], X + Y  * 32 + 1) ;
	PatternGen_RandomSeed(&Randoms[2], X + Y  * 32 + 32);
	PatternGen_RandomSeed(&Randoms[3], X + Y  * 32 + 33);


	int CurrentAlgo = S->algooptions[P->algo];
	GenFuncPtr TheFunc = Funcs[CurrentAlgo];
	InitFuncPtr InitFunc = InitFuncs[CurrentAlgo];

	for (int j =0 ;j<4;j++)
	{
		InitFunc(P, S, &Randoms[j], &FuncSpecific[j]);
	}

	PatternInit[CurrentAlgo](P,S,T);

	if (FuncDither[CurrentAlgo] == 1)
	{
		for (int i =0;i<len;i++)
		{
			for (int j =0 ;j<4;j++)
			{

				TheFunc(P, S, &Randoms[j], &FuncSpecific[j], i, &Ticks[j]);
			}

			ApplyDither(i, ditherx, &Ticks[0], &Ticks[1], &Top);
			ApplyDither(i, ditherx, &Ticks[2], &Ticks[3], &Bot);
			ApplyDither(i, dithery, &Top, &Bot, &T->Ticks[i]);
		}
	}
	else
	{
		for (int i =0;i<len;i++)
		{
			TheFunc(P, S, &Randoms[0], &FuncSpecific[0], i, &T->Ticks[i]);
		}
	}
}

