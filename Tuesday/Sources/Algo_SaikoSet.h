#include "Algo.h"

void Algo_Saiko_Lead_Gen( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *PS, int I,  Tuesday_Tick_t *Output)
{
	int note = 0;
	struct ScaledNote SN ;
	SN.oct = 0;
	SN.note = 0;
	DefaultTick(Output);

	int RandNote = Tuesday_Rand(R) % 8;

	switch (RandNote)
	{
	case 0:
	case 2: NOTE(0, 0); break;
	case 1: NOTE(-1, 0); break; // -12
	case 3: NOTE(0, 1); break;
	case 4: NOTE(0, 2); break;
	case 5: NOTE(0, 4); break;
	case 6: NOTE(1, 0); break; // 12
	case 7: NOTE(1, 1); break; // 13
	}

	if (Output->accent)
	{
		switch (RandNote)
		{
		case 0:
		case 3:
		case 7:NOTE(1, 0); break;
		case 1:NOTE(0, 0); break;
		case 2:NOTE(1, -1); break;
		case 4:NOTE(1, 1); break;
		case 5:NOTE(0, -1); break;
		case 6:NOTE(1, 1); break;
		}
	}

	if (I < 7)
	{
		if (PS->GENERIC.b1 == 0) SN.note += 1;
		if (PS->GENERIC.b2 == 0) SN.note -= 2;
	}
	SN.oct++;
	Output->vel = Tuesday_RandByte(R);
	Output->accent = Tuesday_BoolChance(R);
	Output->note = ScaleToNote(&SN, T, P, S);
}

void Tuesday_Psych( Tuesday_PatternContainer *T,  Tuesday_RandomGen *R, int Length)
{
	if (Length > TUESDAY_MAXTICK) Length = TUESDAY_MAXTICK;

	T->Length = Length;
	T->TPB = 4;

	for (int i = 0; i < TUESDAY_MAXTICK; i++)
	{
		T->Ticks[i].vel = Tuesday_RandByte(R);

		uint64_t a = Tuesday_Rand(R) & 32767;
		a *= 0x57619F1ULL;
		a = (a >> (32 + 9)) + ((uint32_t)a >> 31);

		T->Ticks[i].accent = (a != 0);

		int r = (Tuesday_Rand(R) & 32767) >> 12;
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

void Tuesday_Flat( Tuesday_PatternContainer *T,  Tuesday_RandomGen *R, int Length)
{
	if (Length > TUESDAY_MAXTICK) Length = TUESDAY_MAXTICK;

	T->TPB = 4;
	T->Length = Length;
	for (int i = 0; i < TUESDAY_MAXTICK; i++)
	{


		T->Ticks[i].vel = Tuesday_RandByte(R);

		uint64_t a = Tuesday_Rand(R) & 32767;
		a *= 0x57619F1ULL;
		a = (a >> (32 + 9)) + ((uint32_t)a >> 31);

		T->Ticks[i].accent = (a != 0);

		int r = (Tuesday_Rand(R) & 32767) >> 12;
		switch (r)
		{
		case 0:
		case 2:
		case 4:
		case 5:
		case 6:
			T->Ticks[i].note = 0; break;
		case 1: T->Ticks[i].note = -12; break;
		case 3: T->Ticks[i].note = 0xD; break;
		case 7: T->Ticks[i].note = 0xC; break;
		default:
			// don't change note value!
			break;
		}

	}

};

void Tuesday_Goa( Tuesday_PatternContainer *T,  Tuesday_RandomGen *R, int Length)
{
	if (Length > TUESDAY_MAXTICK) Length = TUESDAY_MAXTICK;
	T->TPB = 4;
	T->Length = Length;
	for (int i = 0; i < TUESDAY_MAXTICK; i++)
	{
		T->Ticks[i].vel = Tuesday_RandByte(R);
		T->Ticks[i].accent = (Tuesday_BoolChance(R)) ? 1 : 0;

		int RandNote = Tuesday_Rand(R) % 8;
		switch (RandNote)
		{
		case 0:
		case 2:
			T->Ticks[i].note = 0; break;
		case 1: T->Ticks[i].note = (char)0xf4; break;
		case 3: T->Ticks[i].note = 1; break;
		case 4: T->Ticks[i].note = 3; break;
		case 5: T->Ticks[i].note = 7; break;
		case 6: T->Ticks[i].note = 0xc; break;
		case 7: T->Ticks[i].note = 0xd; break;
		}

		if (T->Ticks[i].accent)
		{
			switch (RandNote)
			{
			case 0:
			case 3:
			case 7: T->Ticks[i].note = 0; break;
			case 1:T->Ticks[i].note = (char)0xf4; break;
			case 2:T->Ticks[i].note = (char)0xfe; break;
			case 4:T->Ticks[i].note = 3; break;
			case 5:T->Ticks[i].note = (char)0xf2; break;
			case 6:T->Ticks[i].note = 1; break;
			}
		}

	}

	if (Tuesday_BoolChance(R))
	{
		Pattern_Transpose(T, 0, 7, 3);
	}

	if (Tuesday_BoolChance(R))
	{
		Pattern_Transpose(T, 0, 7, -5);
	}
}

void Tuesday_Zeph( Tuesday_PatternContainer *T, struct Tuesday_RandomGen *R, int stepsperbeat, int beats, int fullcycles)
{
	int totalticks = stepsperbeat * beats * fullcycles;
	int subpattern = stepsperbeat * beats;
	if (totalticks > TUESDAY_MAXTICK)
	{
		return;
	}
	Tuesday_Goa(T, R, subpattern);
	T->Length = totalticks;
	T->TPB = stepsperbeat;
	for (int i = 1; i < fullcycles; i++)
	{
		for (int j = 0; j < subpattern; j++)
		{
			T->Ticks[i*subpattern + j].note = T->Ticks[j].note;
			T->Ticks[i*subpattern + j].vel = T->Ticks[j].vel;
			T->Ticks[i*subpattern + j].accent = T->Ticks[j].accent;
		}
		if (Tuesday_PercChance(R, 128))
		{
			Pattern_Rotate(T, (fullcycles - 1)*subpattern, subpattern, 3);
		}
		if (Tuesday_PercChance(R, 128))
		{
			Pattern_Reverse(T, (fullcycles - 1)*subpattern + subpattern / 2, subpattern / 2);
		}
	}
	if (Tuesday_PercChance(R, 128)) Pattern_Transpose(T, (fullcycles - 1)*subpattern, subpattern, 3);
	if (Tuesday_PercChance(R, 128)) Pattern_Transpose(T, 2 * subpattern, 7, -5);
}




void Saiko_PsychTick( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *PS, int I,  Tuesday_Tick_t *Output)
{
	Output->vel = Tuesday_RandByte(R);

	uint64_t a = Tuesday_Rand(R) & 32767;
	a *= 0x57619F1ULL;
	a = (a >> (32 + 9)) + ((uint32_t)a >> 31);

	Output->accent = (a != 0);

	int r = (Tuesday_Rand(R) & 32767) >> 12;
	switch (r)
	{
	case 0:
	case 1:
	case 6:
		Output->note = 0;
		break;
	case 2:
		Output->note = -2;
		break;
	case 3:
		Output->note = 3;
		break;
	case 4:
		Output->note = 15;
		break;
	case 5:
		Output->note = 10;
		break;
	case 7:
		Output->note = 12;
		break;
	default:
		// don't change note value!
		break;
	}
	
}

void Saiko_FlatTick( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *PS, int I,  Tuesday_Tick_t *Output)
{

		Output->vel = Tuesday_RandByte(R);

		uint64_t a = Tuesday_Rand(R) & 32767;
		a *= 0x57619F1ULL;
		a = (a >> (32 + 9)) + ((uint32_t)a >> 31);

		Output->accent = (a != 0);

		int r = (Tuesday_Rand(R) & 32767) >> 12;
		switch (r)
		{
		case 0:
		case 2:
		case 4:
		case 5:
		case 6:
			Output->note = 0; break;
		case 1: Output->note = -12; break;
		case 3: Output->note = 0xD; break;
		case 7: Output->note = 0xC; break;
		default:
			// don't change note value!
			break;
		}


};

void Saiko_GoaTick( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *PS, int I,  Tuesday_Tick_t *Output)
{
		Output->vel = Tuesday_RandByte(R);
		Output->accent = (Tuesday_BoolChance(R)) ? 1 : 0;

		int RandNote = Tuesday_Rand(R) % 8;
		switch (RandNote)
		{
		case 0:
		case 2:
			Output->note = 0; break;
		case 1: Output->note = (char)0xf4; break;
		case 3: Output->note = 1; break;
		case 4: Output->note = 3; break;
		case 5: Output->note = 7; break;
		case 6: Output->note = 0xc; break;
		case 7: Output->note = 0xd; break;
		}

		if (Output->accent)
		{
			switch (RandNote)
			{
			case 0:
			case 3:
			case 7: Output->note = 0; break;
			case 1:Output->note = (char)0xf4; break;
			case 2:Output->note = (char)0xfe; break;
			case 4:Output->note = 3; break;
			case 5:Output->note = (char)0xf2; break;
			case 6:Output->note = 1; break;
			}
		}


	if (PS->GENERIC.b1 && I <= 7 )
	{
		Output->note += 3;
	}

	if (PS->GENERIC.b2 && I <= 7)
	{
		Output->note -= 5;
	}
	

}

void Saiko_ZephTick( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *PS, int I,  Tuesday_Tick_t *Output)
{
	Output->vel = Tuesday_RandByte(R);
	Output->accent = (Tuesday_BoolChance(R)) ? 1 : 0;

	int RandNote = Tuesday_Rand(R) % 8;
	switch (RandNote)
	{
	case 0:
		Output->note = 0; break;
	case 1: Output->note = (char)0xf4; break;
	case 3: Output->note = 1; break;
	case 4: Output->note = 3; break;
	case 5: Output->note = 7; break;
	case 6: Output->note = 0xc; break;
	case 7: Output->note = 0xd; break;
	case 2: Output->note = 12+3; break;
	}

	if (Output->accent)
	{
		switch (RandNote)
		{
		case 0:
		case 7: Output->note = 0; break;
		case 1:Output->note = (char)0xf4; break;
		case 2:Output->note = (char)0xfe; break;
		case 4:Output->note = 3; break;
		case 5:Output->note = (char)0xf2; break;
		case 6:Output->note = 1; break;
		case 3:Output->note = 12; break;
		}
	}


	if (PS->GENERIC.b1 && I <= 8)
	{
		Output->note += 7;
	}

	if (PS->GENERIC.b2 && I <= 5)
	{
		Output->note -= 3;
	}


}



void Algo_Saiko_Classic( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *PS, int I,  Tuesday_Tick_t *Output)
{
	DefaultTick(Output);

	switch (P->scale%4)
	{
	case 0:
		Saiko_GoaTick(T, P, S, R, PS, I, Output); break;
	case 1:
		Saiko_FlatTick(T, P, S, R, PS, I, Output); break;
	case 2:
		Saiko_PsychTick(T, P, S, R, PS, I, Output); break;
	case 3:
		Saiko_ZephTick(T, P, S, R, PS, I, Output); break;
	}

	Output->note += 24;
	Output->vel = (Tuesday_Rand(R) / 4);
	RandomSlideAndLength(Output, R);
}
