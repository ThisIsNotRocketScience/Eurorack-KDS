void Algo_TriTrance_Init(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *Output)
{

	Tuesday_RandomSeed(R, T->seed1 >> 3);
	Tuesday_RandomSeed(&Output->ExtraRandom, T->seed2 >> 3);

	Output->b1 = (Tuesday_Rand(R) & 0x7);
	Output->b2 = (Tuesday_Rand(R) & 0x7);

	Output->b3 = (Tuesday_Rand(&Output->ExtraRandom) & 0x15);
	if (Output->b3 >= 7) Output->b3 -= 7; else Output->b3 = 0;
	Output->b3 -= 4;

	Output->b4 = 0;
}

void Algo_TriTrance_Gen(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *PS, int I, struct Tuesday_Tick *Output)
{
	struct ScaledNote SN;
	int veloffs = 0;
	int accentoffs = 0;
	switch ((I + PS->b2) % 3)
	{
	case 0:
		if ((Tuesday_BoolChance(&PS->ExtraRandom) == 1) && (Tuesday_BoolChance(&PS->ExtraRandom) == 1))
		{
			PS->b3 = (Tuesday_Rand(&PS->ExtraRandom) & 0x15);
			if (PS->b3 >= 7) PS->b3 -= 7; else PS->b3 = 0;
			PS->b3 -= 4;
		}
		NOTE(0, PS->b3); break;
	case 1:NOTE(1, PS->b3);
		if (Tuesday_BoolChance(R) == 1)  PS->b2 = (Tuesday_Rand(R) & 0x7);
		break;
	case 2:
		NOTE(2, PS->b1);
		veloffs = 127;
		accentoffs = 127;
		if (Tuesday_BoolChance(R) == 1)
		{
			PS->b1 = ((Tuesday_Rand(R) >> 5) & 0x7);

		}; break;
	}

	int32_t n = ScaleToNote(&SN, T, P, S);
	Output->note = n;

	Output->vel = (Tuesday_Rand(R) / 2) + veloffs;
	Output->accent = Tuesday_PercChance(R, 100 + accentoffs);


}
