void Algo_Test_Gen(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *PS, int I, struct Tuesday_Tick *Output)
{
	struct ScaledNote SN;

	NOTE((I % 5), 0);

	Output->note = ScaleToNote(&SN, T, P, S);
	Output->accent = Tuesday_BoolChance(R);
	Output->vel = (I * 40) % 255;
}
