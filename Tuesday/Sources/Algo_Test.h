void Algo_Test_Init( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *Output)
{
	Output->Test.Mode = T->seed1 >> 7;
	Output->Test.Accent = T->seed2 >> 7;
	Output->Test.Velocity = (T->seed2 <<1 );
	Output->Test.Note = 0;
	Output->Test.SweepSpeed = (T->seed1 & (3<<1))>>1;
}

void Algo_Test_Gen( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *PS, int I,  Tuesday_Tick_t *Output)
{
	struct ScaledNote SN;
	DefaultTick(Output);

	switch (PS->Test.Mode)
	{
		case TESTMODE_OCTSWEEPS:
			NOTE((I % 5), 0);
		break;
		case TESTMODE_SCALEWALKER:
			NOTE(0, PS->Test.Note);
			PS->Test.Note = (PS->Test.Note + 1);
			break;
	}
	Output->slide = PS->Test.SweepSpeed;
	Output->note = ScaleToNote(&SN, T, P, S);
	Output->accent = PS->Test.Accent;
	Output->vel = PS->Test.Velocity;
}

void Algo_Random_Init( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *Output)
{
}

void Algo_Random_Gen( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *PS, int I,  Tuesday_Tick_t *Output)
{
	struct ScaledNote SN;
	DefaultTick(Output);
	RandomSlideAndLength(Output, R);
	NOTE(0, Tuesday_Rand(R) % (S->scales[S->scale[P->scale] & 0xf].count * 2));
	
	Output->note = ScaleToNote(&SN, T, P, S);
	Output->vel = Tuesday_Rand(R);
	Output->accent = Tuesday_BoolChance(R);
}
