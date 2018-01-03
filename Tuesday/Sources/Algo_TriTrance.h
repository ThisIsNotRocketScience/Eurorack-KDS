void Algo_TriTrance_Init( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *Output)
{

	Tuesday_RandomSeed(R, T->seed1 >> 3);
	Tuesday_RandomSeed(&Output->ExtraRandom, T->seed2 >> 3);

	Output->GENERIC.b1 = (Tuesday_Rand(R) & 0x7);
	Output->GENERIC.b2 = (Tuesday_Rand(R) & 0x7);

	Output->GENERIC.b3 = (Tuesday_Rand(&Output->ExtraRandom) & 0x15);
	if (Output->GENERIC.b3 >= 7) Output->GENERIC.b3 -= 7; else Output->GENERIC.b3 = 0;
	Output->GENERIC.b3 -= 4;

	Output->GENERIC.b4 = 0;
}

void Algo_TriTrance_Gen( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *PS, int I,  Tuesday_Tick_t *Output)
{
	struct ScaledNote SN;
	DefaultTick(Output);
	int veloffs = 0;
	int accentoffs = 0;
	switch ((I + PS->GENERIC.b2) % 3)
	{
	case 0:
		if ((Tuesday_BoolChance(&PS->ExtraRandom) == 1) && (Tuesday_BoolChance(&PS->ExtraRandom) == 1))
		{
			PS->GENERIC.b3 = (Tuesday_Rand(&PS->ExtraRandom) & 0x15);
			if (PS->GENERIC.b3 >= 7) PS->GENERIC.b3 -= 7; else PS->GENERIC.b3 = 0;
			PS->GENERIC.b3 -= 4;
		}
		NOTE(0, PS->GENERIC.b3); break;
	case 1:NOTE(1, PS->GENERIC.b3);
		if (Tuesday_BoolChance(R) == 1)  PS->GENERIC.b2 = (Tuesday_Rand(R) & 0x7);
		break;
	case 2:
		NOTE(2, PS->GENERIC.b1);
		veloffs = 127;
		accentoffs = 127;
		if (Tuesday_BoolChance(R) == 1)
		{
			PS->GENERIC.b1 = ((Tuesday_Rand(R) >> 5) & 0x7);

		}; break;
	}

	int32_t n = ScaleToNote(&SN, T, P, S);
	Output->note = n;
	if (Tuesday_PercChance(R, 50 + accentoffs))
	{
		   Output->maxsubticklength = ((1 + ( Tuesday_Rand(R) % 4)) * TUESDAY_SUBTICKRES)  - 2;
	}

	Output->vel = (Tuesday_Rand(R) / 2) + veloffs;
	Output->accent = Tuesday_PercChance(R, 100 + accentoffs);
	if (Tuesday_PercChance(R,  ~(veloffs / 2))) Output->slide  = (Tuesday_Rand(R)%2);


}
