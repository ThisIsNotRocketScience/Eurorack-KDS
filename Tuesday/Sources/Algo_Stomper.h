
void Algo_Stomper_Init( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *Output)
{
	Tuesday_RandomSeed(R, T->seed2 >> 2);
	Tuesday_RandomSeed(&Output->ExtraRandom, T->seed1 >> 2);

	Output->Stomper.LastMode = (Tuesday_Rand(&Output->ExtraRandom) % 7) * 2;
	Output->Stomper.CountDown = 0;
	Output->Stomper.LowNote = Tuesday_Rand(R) % 3;
	Output->Stomper.LastNote = Output->Stomper.LowNote;
	Output->Stomper.LastOctave = 0;
	Output->Stomper.HighNote[0] = Tuesday_Rand(R) % 7;
	Output->Stomper.HighNote[1] = Tuesday_Rand(R) % 5;
}

void Algo_Stomper_Gen( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *PS, int I,  Tuesday_Tick_t *Output)
{


	//low high
	//high low
	//slide low
	//slide up

	unsigned char accentoffs = 0;
	unsigned char veloffset = 0;

	struct ScaledNote SN;
	DefaultTick(Output);

	if (PS->Stomper.CountDown > 0)
	{
		veloffset = 100- PS->Stomper.CountDown*20;
		NOTEOFF();
		PS->Stomper.CountDown--;
	}
	else
	{
		if (PS->Stomper.LastMode == STOMPER_MAKENEW)
		{
			PS->Stomper.LastMode = (Tuesday_Rand(&PS->ExtraRandom) % 7) * 2;
		}

		if (Tuesday_PercChance(R, 100))
		{
			PS->Stomper.LowNote = Tuesday_Rand(R) % 3;

		}
		if (Tuesday_PercChance(R, 100))
		{
			PS->Stomper.HighNote[0] = Tuesday_Rand(R) % 7;

		}
		if (Tuesday_PercChance(R, 100))
		{
			PS->Stomper.HighNote[1] = Tuesday_Rand(R) % 5;
		}
		veloffset = 100;
		int maxticklen = 2;
		switch (PS->Stomper.LastMode)
		{
		case STOMPER_SLIDEDOWN1:
			NOTE(1, PS->Stomper.HighNote[Tuesday_Rand(R) % 2]);
			PS->Stomper.LastMode++;
			break;
		case STOMPER_SLIDEDOWN2:
			NOTE(0, PS->Stomper.LowNote);
			Output->slide = (Tuesday_Rand(R) % 3) + 1;;
			if (Tuesday_BoolChance(&PS->ExtraRandom)) PS->Stomper.CountDown = Tuesday_Rand(&PS->ExtraRandom) % maxticklen;
			PS->Stomper.LastMode = STOMPER_MAKENEW;
			break;
		case STOMPER_SLIDEUP1:
			NOTE(0, PS->Stomper.LowNote);
			PS->Stomper.LastMode++;
			break;
		case STOMPER_SLIDEUP2:
			NOTE(1, PS->Stomper.HighNote[Tuesday_Rand(R) % 2]);
			Output->slide = (Tuesday_Rand(R) % 3) + 1;
			if (Tuesday_BoolChance(&PS->ExtraRandom)) PS->Stomper.CountDown = Tuesday_Rand(&PS->ExtraRandom) % maxticklen;
			PS->Stomper.LastMode = STOMPER_MAKENEW;
			break;
		case STOMPER_LOW1:
			accentoffs += 100;
		case STOMPER_LOW2:
			NOTE(0, PS->Stomper.LowNote);
			if (Tuesday_BoolChance(&PS->ExtraRandom)) PS->Stomper.CountDown = Tuesday_Rand(&PS->ExtraRandom) % maxticklen;
			PS->Stomper.LastMode = STOMPER_MAKENEW;
			break;
		case STOMPER_PAUSE1:
		case STOMPER_PAUSE2:
			NOTE(PS->Stomper.LastOctave, PS->Stomper.LastNote);
			veloffset = 0;
			if (Tuesday_BoolChance(&PS->ExtraRandom)) PS->Stomper.CountDown = Tuesday_Rand(&PS->ExtraRandom) % maxticklen;
			PS->Stomper.LastMode = STOMPER_MAKENEW;
			break;
		case STOMPER_HIGH1:
			accentoffs += 100;
		case STOMPER_HIGH2:
			if (Tuesday_BoolChance(&PS->ExtraRandom)) PS->Stomper.CountDown = Tuesday_Rand(&PS->ExtraRandom) % maxticklen;
			PS->Stomper.LastMode = STOMPER_MAKENEW;
			NOTE(0, PS->Stomper.LowNote);
			break;
		case STOMPER_HILOW1:
			NOTE(1, PS->Stomper.HighNote[Tuesday_Rand(R) % 2]);
			PS->Stomper.LastMode++;
			break;
		case STOMPER_HILOW2:
			NOTE(0, PS->Stomper.LowNote);
			accentoffs += 100;
			if (Tuesday_BoolChance(&PS->ExtraRandom)) PS->Stomper.CountDown = Tuesday_Rand(&PS->ExtraRandom) % maxticklen;
			PS->Stomper.LastMode = STOMPER_MAKENEW;
			break;
		case STOMPER_LOWHI1:
			NOTE(0, PS->Stomper.LowNote);
			accentoffs += 100;
			PS->Stomper.LastMode++;
			break;
		case STOMPER_LOWHI2:
			NOTE(1, PS->Stomper.HighNote[Tuesday_Rand(R) % 2]);
			if (Tuesday_BoolChance(&PS->ExtraRandom)) PS->Stomper.CountDown = Tuesday_Rand(&PS->ExtraRandom) % maxticklen;
			PS->Stomper.LastMode = STOMPER_MAKENEW;
			break;
		default:
			NOTE(PS->Stomper.LastOctave, PS->Stomper.LastNote);
		}
	}
	PS->Stomper.LastNote = SN.note;
	PS->Stomper.LastOctave = SN.oct;

	int32_t n = ScaleToNote(&SN, T, P, S);
	Output->note = n;
	if (PS->Stomper.CountDown > 0)   Output->maxsubticklength = (PS->Stomper.CountDown * TUESDAY_SUBTICKRES)  - 2;
	if (Tuesday_PercChance(R, 50 + accentoffs))
	{
		   Output->maxsubticklength = ((1 + ( Tuesday_Rand(R) % 4)) * TUESDAY_SUBTICKRES)  - 2;
	}
	Output->vel = (Tuesday_Rand(&PS->ExtraRandom) / 4) + veloffset;
	Output->accent = Tuesday_PercChance(R, 50 + accentoffs);


}
