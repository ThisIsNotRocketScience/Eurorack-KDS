#include "Tuesday.h"
#include <stdint.h>

#include "Algo.h"
#include "AlgoImpl.h"

void Tuesday_ToggleSlope(Tuesday_Settings *Settings) 
{

	int ClockDownSlope = ((Settings->ClockMode & CLOCKMODE_DOWNSLOPE) > 0)?1:0;
	if (ClockDownSlope > 0)
	{
		Settings->ClockMode &= ~CLOCKMODE_DOWNSLOPE;
	}

	else
	{
		Settings->ClockMode |= CLOCKMODE_DOWNSLOPE;
	}
};

void Tuesday_ToggleReset(Tuesday_Settings *Settings) 
{
	int ClockResetModeInverted = (Settings->ClockMode & CLOCKMODE_RESETINVERTED) > 0?1:0;

	if (ClockResetModeInverted > 0)
	{
		Settings->ClockMode &= ~CLOCKMODE_RESETINVERTED;
	}

	else
	{
		Settings->ClockMode |= CLOCKMODE_RESETINVERTED;
	}
};
void Tuesday_ToggleResetPattern(Tuesday_Settings *Settings) 
{
	int ClockResetHaltsPulses = (Settings->ClockMode & CLOCKMODE_RESET_BLOCKS_TICKS) > 0?1:0;

	if (ClockResetHaltsPulses > 0)
	{
		Settings->ClockMode &= ~CLOCKMODE_RESET_BLOCKS_TICKS;
	}

	else
	{
		Settings->ClockMode |= CLOCKMODE_RESET_BLOCKS_TICKS;
	}
};

PatternFunctions PatternTypes[ALGO_COUNT];
long oldseed = -1;
struct denoise_state_t algosw_state = { 0 };
struct denoise_state_t scalesw_state = { 0 };
struct denoise_state_t beatsw_state = { 0 };
struct denoise_state_t tpbsw_state = { 0 };
int tickssincecommit = 0;


extern void SaveEeprom();
extern void SaveSettingsEeprom();
extern void 	SaveCalibrationEeprom();

void SetLedNumber(Tuesday_PatternGen *T,  int offset, int number)
{
	switch (number % 13)
	{
	case 0:
		T->StateLedTargets[offset + 0] = 255;
		T->StateLedTargets[offset + 1] = 0;
		T->StateLedTargets[offset + 2] = 0;
		T->StateLedTargets[offset + 3] = 0;
		break;
	case 1:
		T->StateLedTargets[offset + 0] = 0;
		T->StateLedTargets[offset + 1] = 255;
		T->StateLedTargets[offset + 2] = 0;
		T->StateLedTargets[offset + 3] = 0;
		break;
	case 2:
		T->StateLedTargets[offset + 0] = 0;
		T->StateLedTargets[offset + 1] = 0;
		T->StateLedTargets[offset + 2] = 255;
		T->StateLedTargets[offset + 3] = 0;
		break;
	case 3:
		T->StateLedTargets[offset + 0] = 0;
		T->StateLedTargets[offset + 1] = 0;
		T->StateLedTargets[offset + 2] = 0;
		T->StateLedTargets[offset + 3] = 255;
		break;
	case 4:
		T->StateLedTargets[offset + 0] = 255;
		T->StateLedTargets[offset + 1] = 0;
		T->StateLedTargets[offset + 2] = 0;
		T->StateLedTargets[offset + 3] = 255;
		break;
	case 5:
		T->StateLedTargets[offset + 0] = 255;
		T->StateLedTargets[offset + 1] = 255;
		T->StateLedTargets[offset + 2] = 0;
		T->StateLedTargets[offset + 3] = 0;
		break;
	case 6:
		T->StateLedTargets[offset + 0] = 0;
		T->StateLedTargets[offset + 1] = 255;
		T->StateLedTargets[offset + 2] = 255;
		T->StateLedTargets[offset + 3] = 0;
		break;
	case 7:
		T->StateLedTargets[offset + 0] = 0;
		T->StateLedTargets[offset + 1] = 0;
		T->StateLedTargets[offset + 2] = 255;
		T->StateLedTargets[offset + 3] = 255;
		break;
	case 8:
		T->StateLedTargets[offset + 0] = 255;
		T->StateLedTargets[offset + 1] = 0;
		T->StateLedTargets[offset + 2] = 255;
		T->StateLedTargets[offset + 3] = 255;
		break;
	case 9:
		T->StateLedTargets[offset + 0] = 255;
		T->StateLedTargets[offset + 1] = 255;
		T->StateLedTargets[offset + 2] = 0;
		T->StateLedTargets[offset + 3] = 255;
		break;
	case 10:
		T->StateLedTargets[offset + 0] = 255;
		T->StateLedTargets[offset + 1] = 255;
		T->StateLedTargets[offset + 2] = 255;
		T->StateLedTargets[offset + 3] = 0;
		break;
	case 11:
		T->StateLedTargets[offset + 0] = 0;
		T->StateLedTargets[offset + 1] = 255;
		T->StateLedTargets[offset + 2] = 255;
		T->StateLedTargets[offset + 3] = 255;
		break;
	case 12:
		T->StateLedTargets[offset + 0] = 255;
		T->StateLedTargets[offset + 1] = 255;
		T->StateLedTargets[offset + 2] = 255;
		T->StateLedTargets[offset + 3] = 255;
		break;
	}
}


void NOINLINE ShowSets(Tuesday_PatternGen*T, int algogroup, int scalegroup, int ticksgroup, int beatsgroup)
{
	if (ticksgroup > -1) SetLedNumber(T,0, ticksgroup);
	if (beatsgroup > -1) SetLedNumber(T,4, beatsgroup);
	if (scalegroup > -1) SetLedNumber(T,8, scalegroup);
	if (algogroup > -1) SetLedNumber(T,12, algogroup);
}


void NOINLINE Tuesday_SetupLeds(Tuesday_PatternGen *T, Tuesday_Settings *settings, Tuesday_Params *params)
{
	switch (T->UIMode)
	{
	case UI_STARTUP:
		// give the opening animation some breathing space
		break;

	case UI_NORMAL:
		SetLedNumber(T,0, params->tpbopt);
		SetLedNumber(T,4, params->beatopt);
		SetLedNumber(T,8, params->scale);
		SetLedNumber(T,12, params->algo);
		break;

	case UI_SELECTOPTION:
	{
		for (int i = 0; i < TUESDAY_LEDS; i++) T->StateLedTargets[i] = 0;
		int D = -1;
		if ((tickssincecommit >> 8) & 0x1) D = T->OptionIndex;

		switch (T->OptionSelect)
		{
		case OPTION_ALGO:
		{
			int S = settings->algooptions[T->OptionIndex];
			ShowSets(T,D, (S >> 4) & 0x07, S & 0x3, (S >> 2) & 0x03);
		}
		break;
		case OPTION_SCALE:
		{
			int S = settings->scale[T->OptionIndex];
			ShowSets(T,(S >> 4) & 0x03, D, S & 0x3, (S >> 2) & 0x03);
		}
		break;
		case OPTION_BEATS:
		{
			int S = settings->beatoptions[T->OptionIndex] - 1;
			ShowSets(T,S & 0x3, (S >> 2) & 0x03, (S >> 4) & 0x03, D);
		}
		break;
		case OPTION_TPB:
		{
			int S = settings->tpboptions[T->OptionIndex] - 1;
			ShowSets(T,S & 0x3, (S >> 2) & 0x03, D, (S >> 4) & 0x03);
		}
		break;
		}
	}break;

	case UI_GLOBALSETTINGS:
	{
		for (int i = 0; i < TUESDAY_LEDS; i++) T->StateLedTargets[i] = 0;

		int S = settings->ClockSubDivMode;
		ShowSets(T,-1,settings->OctaveLimiter, S, -1);

		T->StateLedTargets[12+0] =( settings->ClockMode & CLOCKMODE_RESETINVERTED) >0?255:0;
		T->StateLedTargets[12+1] = (settings->ClockMode & CLOCKMODE_RESET_BLOCKS_TICKS) > 0 ? 255 : 0;
		T->StateLedTargets[12+2] = (settings->ClockMode & CLOCKMODE_DOWNSLOPE)> 0 ? 255 : 0;
		T->StateLedTargets[12+3] = 0;


		T->StateLedTargets[4] = 255;
		T->StateLedTargets[5] = 255;
		T->StateLedTargets[6] = 255;
		T->StateLedTargets[7] = 255;

	}
	break;
	case UI_CALIBRATION:
		for (int i = 0; i < TUESDAY_LEDS; i++) T->StateLedTargets[i] = 0;
		switch (T->CalibTarget)
		{
		case CALIBRATION_VEL:
			T->Gates[GATE_ACCENT] = 1;
			T->Gates[GATE_GATE] = 0;
			T->StateLedTargets[0] = 255;
			T->StateLedTargets[1] = 255;
			T->StateLedTargets[2] = 255;
			T->StateLedTargets[3] = 255;
			break;

		case CALIBRATION_NOTE:
			T->Gates[GATE_ACCENT] = 0;
			T->Gates[GATE_GATE] = 1;
			T->StateLedTargets[4] = 255;
			T->StateLedTargets[5] = 255;
			T->StateLedTargets[6] = 255;
			T->StateLedTargets[7] = 255;
			break;

		case CALIBRATION_NOTARGET:
		{
			T->Gates[GATE_ACCENT] = 0;
			T->Gates[GATE_GATE] = 0;
			unsigned char tT = Triangle(tickssincecommit << 23) >> 24;
			for (int i = 0; i < 4; i++)
			{
				T->StateLedTargets[i] = tT;
				T->StateLedTargets[i + 4] = ~tT;
			}
		}
		break;
		}
		break;
	}

	for (int i = 0; i < TUESDAY_LEDS; i++)
	{
		if (T->StateLedTargets[i] > T->RStateLeds[i])
		{
			T->RStateLeds[i]++;
		}
		else
		{
			if (T->StateLedTargets[i] < T->RStateLeds[i])
			{
				T->RStateLeds[i]--;
			}
			if (T->StateLedTargets[i] < T->RStateLeds[i])
			{
				T->RStateLeds[i]--;
			}
		}
	}
}



void NOINLINE SetPatternFunc(int i, GenFuncPtr Gen, InitFuncPtr Init, PatternInitFuncPtr PatternInit, uint8_t dither)
{
	PatternFunctions *PF = &PatternTypes[i];
	PF->Gen = Gen;
	PF->Init = Init;
	PF->PatternInit = PatternInit;
	PF->Dither = dither;
}

void Tuesday_SetupClockSubdivision( Tuesday_PatternGen *P,  Tuesday_Settings *S,  Tuesday_Params *Par)
{
	switch (S->ClockSubDivMode % 4)
	{
	case CLOCKSUBDIV_4: P->TicksPerMeasure = 4 * S->tpboptions[Par->tpbopt]; break;  // adaptive
	case CLOCKSUBDIV_8: P->TicksPerMeasure = 8 * S->tpboptions[Par->tpbopt]; break;  // adaptive
	case CLOCKSUBDIV_16: P->TicksPerMeasure = 16 * 4; break; // 64
	case CLOCKSUBDIV_24PPQN: P->TicksPerMeasure = 24 * 4; break; // 96
	}
}

void Tuesday_Init( Tuesday_PatternGen *P)
{
	for (int i = 0; i < ALGO_COUNT; i++)
	{
		SetPatternFunc(i, NoPattern, NoInit, NoPatternInit, 1);
	}

	SetPatternFunc(ALGO_SAIKO_CLASSIC, &Algo_Saiko_Classic, &Algo_Init_Generic_FourBool, &NoPatternInit, 1);
	SetPatternFunc(ALGO_SAIKO_LEAD, &Algo_Saiko_Lead_Gen, &Algo_Init_Generic_FourBool, &NoPatternInit, 1);
	SetPatternFunc(ALGO_CHIPARP1, Algo_ChipArp_1_Gen, Algo_ChipArp_1_Init, Algo_ChipArp_1_PatternInit, 1);
	SetPatternFunc(ALGO_CHIPARP2, Algo_ChipArp_2_Gen, Algo_ChipArp_2_Init, Algo_ChipArp_2_PatternInit, 1);
	SetPatternFunc(ALGO_TRITRANCE, &Algo_TriTrance_Gen, &Algo_TriTrance_Init, &NoPatternInit, 1);
	SetPatternFunc(ALGO_TESTS, &Algo_Test_Gen, &Algo_Test_Init, &NoPatternInit, 1);
	SetPatternFunc(ALGO_MARKOV, &Algo_Markov_Gen, &Algo_Markov_Init, &NoPatternInit, 1);
	SetPatternFunc(ALGO_STOMPER, &Algo_Stomper_Gen, &Algo_Stomper_Init, &NoPatternInit, 1);
	SetPatternFunc(ALGO_WOBBLE, &Algo_Wobble_Gen, &Algo_Wobble_Init, &NoPatternInit, 1);

	SetPatternFunc(ALGO_SNH, &Algo_SNH_Gen, &Algo_SNH_Init, &NoPatternInit, 1);
	SetPatternFunc(ALGO_SCALEWALKER, &Algo_ScaleWalk_Gen, &Algo_ScaleWalk_Init, &NoPatternInit, 1);
	SetPatternFunc(ALGO_TOOEASY, &Algo_TooEasy_Gen, &Algo_TooEasy_Init, &NoPatternInit, 1);

	SetPatternFunc(ALGO_RANDOM, &Algo_Random_Gen, &Algo_Random_Init, &NoPatternInit, 1);

	P->TicksPerMeasure = 96;
	P->ClockConnected = 0;
	P->lastnote = 0;
	P->CoolDown = 0;
	P->DoReset = 1;
	P->TickOut = 0;
	P->CVOut = 0;
	P->CVOutDelta = 0;
	P->CVOutCountDown = 0;
	P->CVOutTarget = 0;

	P->Tick = -1;
	P->Measure = 0;

	P->countdownTick = 1;
	//P->countdownNote = 1;
	P->msecpertick = 10;
	P->msecsincelasttick = 0;
	P->clockup = 0;
	P->clockshad = 0;
	P->clockssincereset = 0;

	P->timesincelastclocktick = 0;
	P->clocktick = 0;

	P->directtick = 0;

	P->extclockssincereset[0] = 0;
	P->extclockssincereset[1] = 0;
	P->extclockssincereset[2] = 0;
	P->extclockssincereset[3] = 0;
	P->extclockssincereset[4] = 0;
	P->extclockssincereset[5] = 0;

	P->extclockssinceresetcounter[0] = 0;
	P->extclockssinceresetcounter[1] = 0;
	P->extclockssinceresetcounter[2] = 0;
	P->extclockssinceresetcounter[3] = 0;
	P->extclockssinceresetcounter[4] = 0;
	P->extclockssinceresetcounter[5] = 0;

	P->extclocksupsincereset = 0;
	P->extclocksdowsincereset = 0;

	P->lastclocksubdiv = -1;

	for (int i = 0; i < TUESDAY_GATES; i++)
	{
		P->Gates[i] = 0;
		P->GatesGap[i] = 0;
	}

	for (int i = 0; i < TUESDAY_LEDS; i++)
	{
		P->StateLedTargets[i] = 0;
		P->RStateLeds[i] = 0;
	}
	P->switchmode = 1;
	P->commitchange = 0;
	P->UIMode = UI_STARTUP;
	P->ticklengthscaler = 9;
}

int GetResetEq(Tuesday_Settings *s)
{
	if (s->ClockMode & CLOCKMODE_RESETINVERTED) return 0;
	return 1;
}
void Tuesday_Reset( Tuesday_PatternGen *T,Tuesday_Settings *s, int val)
{
	T->LastResetVal = val;
	if (val != GetResetEq(s)) return;

	T->TickOut = 0;
	//T->Tick = -1;
	T->DoReset = 1;
	T->Measure = 0;
	T->CoolDown = 0;
	T->countdownTick = 0;
	T->directtick = 1;
	T->clockssincereset = 0;
	for (int i = 0; i < 6; i++)
	{
		T->extclockssincereset[i] = 0;
		T->extclockssinceresetcounter[i] = 0;
	}
	T->extclocksupsincereset = 0;
	T->extclocksdowsincereset = 0;
}





void UI_SelectOption( Tuesday_PatternGen *T,Tuesday_Settings *settings, Tuesday_Params *params)
{
	int butconf, but1, but2, but3;
	int butconflong = 0;;
	int S = 0;
	int MaxS = 64;
	switch (T->OptionSelect)
	{
	case OPTION_ALGO:
	{
		butconf = pressed(&algosw_state);
		butconflong = islongpress(&algosw_state);
		S = settings->algooptions[T->OptionIndex];
		//MaxS = __ALGO_COUNT;

		but1 = pressed(&tpbsw_state);
		but2 = pressed(&beatsw_state);
		but3 = pressed(&scalesw_state);
	};
	break;

	case OPTION_SCALE:
	{
		butconf = pressed(&scalesw_state);
		butconflong = islongpress(&scalesw_state);
		S = settings->scale[T->OptionIndex];
		//		MaxS = __SCALE_COUNT;

		but1 = pressed(&tpbsw_state);
		but2 = pressed(&beatsw_state);
		but3 = pressed(&algosw_state);

	};
	break;

	case OPTION_BEATS:
	{
		butconf = pressed(&beatsw_state);
		butconflong = islongpress(&beatsw_state);
		//Settings.beatoptions[]
		but1 = pressed(&algosw_state);
		but2 = pressed(&scalesw_state);
		but3 = pressed(&tpbsw_state);
	};
	break;

	case OPTION_TPB:
	{
		butconf = pressed(&tpbsw_state);
		butconflong = islongpress(&tpbsw_state);

		but1 = pressed(&algosw_state);
		but2 = pressed(&scalesw_state);
		but3 = pressed(&beatsw_state);
	};
	break;
	}

	int NewS = S;

	if (but1) NewS = (S & (0xff - (3 << 0))) + (((((S >> 0) & 3) + 1) & 3) << 0);
	if (but2) NewS = (S & (0xff - (3 << 2))) + (((((S >> 2) & 3) + 1) & 3) << 2);
	if (T->OptionSelect == OPTION_ALGO)
	{
		if (but3) NewS = (S & (0xff - (7 << 4))) + (((((S >> 4) & 7) + 1) & 7) << 4);
	}
	else
	{
		if (but3) NewS = (S & (0xff - (3 << 4))) + (((((S >> 4) & 3) + 1) & 3) << 4);
	}

	if (NewS != S)
	{
		S = NewS;

		switch (T->OptionSelect)
		{
		case OPTION_ALGO:  settings->algooptions[T->OptionIndex] = S; break;
		case OPTION_BEATS: settings->beatoptions[T->OptionIndex] = (S + 1); break;
		case OPTION_TPB:   settings->tpboptions[T->OptionIndex] = (S + 1); break;
		case OPTION_SCALE: settings->scale[T->OptionIndex] = S; break;
		}
		T->switchmode = 1;
		SaveSettingsEeprom();

	}
	if (butconf == 1)
	{
		T->OptionIndex = (T->OptionIndex + 1) % 4;

		switch (T->OptionSelect)
		{
		case OPTION_ALGO:  params->algo = T->OptionIndex; break;
		case OPTION_BEATS: params->beatopt = T->OptionIndex; break;
		case OPTION_TPB:   params->tpbopt = T->OptionIndex; break;
		case OPTION_SCALE: params->scale = T->OptionIndex; break;
		}

		T->switchmode = 1;

	}
	if (butconflong == 1)
	{
		T->UIMode = UI_NORMAL;

		switch (T->OptionSelect)
		{


		case OPTION_ALGO:  params->algo = (params->algo + TUESDAY_MAXALGO - 1) % TUESDAY_MAXALGO; break;
		case OPTION_SCALE: params->scale = (params->scale + TUESDAY_MAXSCALE - 1) % TUESDAY_MAXSCALE; break;

		}
	}
}


void UI_GlobalSettings(Tuesday_PatternGen* T,Tuesday_Settings *settings, Tuesday_Params *params)
{
	if (pressed(&beatsw_state))
	{
		T->UIMode = UI_NORMAL;
		Tuesday_SetupClockSubdivision(T, settings, params);
		SaveSettingsEeprom();
		return;
	}

	if (pressed(&tpbsw_state))
	{
		settings->ClockSubDivMode = (settings->ClockSubDivMode + 1) % 4;
		Tuesday_SetupClockSubdivision(T, settings, params);

	}

	if (pressed(&algosw_state))
	{
		settings->ClockMode = (settings->ClockMode + 1) % 8;
	}

	if (pressed(&scalesw_state))
	{
		settings->OctaveLimiter = (settings->OctaveLimiter + 1) % 4;
	}
}

void UI_Calibration(Tuesday_PatternGen *T)
{
	if (pressed(&algosw_state))
	{
		T->UIMode = UI_NORMAL;
		return;
	}

	switch (T->CalibTarget)
	{
	case CALIBRATION_NOTE: ChangeDACCalibration(0, T->seed1, T->seed2); break;
	case CALIBRATION_VEL: ChangeDACCalibration(1, T->seed1, T->seed2); break;
	}


	if (pressed(&beatsw_state))
	{
		T->CalibTarget = CALIBRATION_NOTE;
	}
	else
	{
		if (islongpress(&beatsw_state))
		{
			if (T->CalibTarget == CALIBRATION_NOTE)
			{
				// Write Velocity output calibration value!
				T->CalibTarget = CALIBRATION_NOTARGET;
				SaveCalibrationEeprom();
			}
		}
	}

	if (pressed(&tpbsw_state))
	{
		T->CalibTarget = CALIBRATION_VEL;
	}
	else
	{
		if (islongpress(&tpbsw_state))
		{
			if (T->CalibTarget == CALIBRATION_VEL)
			{
				// Write Note output calibration value!
				T->CalibTarget = CALIBRATION_NOTARGET;
				SaveCalibrationEeprom();
			}
		}
	}

}

void UI_Normal(Tuesday_PatternGen*T,Tuesday_Settings *settings, Tuesday_Params *params)
{

	if (pressed(&algosw_state))
	{
		T->switchmode = 1;
		params->algo = (params->algo + 1) % TUESDAY_MAXALGO;
		T->commitchange = 1;
	}
	else
	{
		if (islongpress(&algosw_state)) // longpress!
		{
			params->algo = (params->algo + TUESDAY_MAXALGO - 1) % TUESDAY_MAXALGO;
			T->commitchange = 1;
			Tuesday_SwitchToOptionMode(T,OPTION_ALGO, params->algo);
		}
	}

	if (pressed(&scalesw_state))
	{
		T->switchmode = 1;
		params->scale = (params->scale + 1) % TUESDAY_MAXSCALE;
		T->commitchange = 1;
	}
	else
	{
		if (islongpress(&scalesw_state)) // longpress!
		{
			params->scale = (params->scale + TUESDAY_MAXSCALE - 1) % TUESDAY_MAXSCALE;
			T->commitchange = 1;
			Tuesday_SwitchToOptionMode(T,OPTION_SCALE, params->scale);
		}
	}

	if (pressed(&beatsw_state))
	{
		T->switchmode = 1;
		params->beatopt = (params->beatopt + 1) % TUESDAY_MAXBEAT;

		T->commitchange = 1;
	}
	else
	{
		if (islongpress(&beatsw_state)) // longpress!
		{
			//Params.beatopt = (Params.beatopt + TUESDAY_MAXBEAT -1) % TUESDAY_MAXBEAT;

			//SwitchToOptionMode(OPTION_BEATS, Params.beatopt);
		}
	}

	if (pressed(&tpbsw_state))
	{
		T->switchmode = 1;

		params->tpbopt = (params->tpbopt + 1) % TUESDAY_MAXTPB;
		T->commitchange = 1;
	}
	else
	{
		if (islongpress(&tpbsw_state)) // longpress!
		{

			params->tpbopt = (params->tpbopt + TUESDAY_MAXTPB - 1) % TUESDAY_MAXTPB;
			T->commitchange = 1;
			T->UIMode = UI_GLOBALSETTINGS;
			//Params.tpbopt = (Params.tpbopt + TUESDAY_MAXTPB - 1) % TUESDAY_MAXTPB;
			//SwitchToOptionMode(OPTION_TPB, Params.tpbopt);
		}
	}
}

struct Tuesday_RandomGen MainRandom;

void Tuesday_SwitchToOptionMode(Tuesday_PatternGen *T,int mode, int startoption)
{
	T->OptionSelect = mode;
	T->OptionIndex = startoption;
	T->UIMode = UI_SELECTOPTION;
}


void Tuesday_MainLoop(Tuesday_PatternGen *T,Tuesday_Settings *settings, Tuesday_Params *params)
{
	switch (T->UIMode)
	{

	case UI_SELECTOPTION: UI_SelectOption(T,settings,params); break;

	case UI_CALIBRATION: UI_Calibration(T); break;

	case UI_GLOBALSETTINGS: UI_GlobalSettings(T,settings,params); break;

	case UI_NORMAL: UI_Normal(T,settings,params); break;


	};


	// read the X/Y seed knobs
	long newseed = (T->seed1) + (T->seed2 << 8);
	if (newseed != oldseed) T->switchmode = 1;

	if (T->switchmode == 1)
	{
		T->switchmode = 0;
		Tuesday_RandomSeed(&MainRandom, newseed);
		Tuesday_Generate(T, params, settings);
		oldseed = newseed;
	}

	if (T->commitchange == 1 && tickssincecommit >= 10)
	{
		SaveEeprom();
		T->commitchange = 0;
		tickssincecommit = 0;
	}

}

void Tuesday_Tick( Tuesday_PatternGen *T,  Tuesday_Params *P)
{
	T->msecpertick = __max(1, T->msecsincelasttick);
	T->msecsincelasttick = 0;
	int CoolDownMax = T->intensity;
	if (T->CoolDown > 0)
	{
		T->CoolDown--;
		if (T->CoolDown > CoolDownMax) T->CoolDown = CoolDownMax;
		if (T->CoolDown < 0) T->CoolDown = 0;
	}

	Tuesday_Tick_t *Tick = &T->CurrentPattern.Ticks[T->Tick];

	if (Tick->vel >= T->CoolDown)
	{
		T->CoolDown = CoolDownMax;

		//	T->countdownNote =( T->msecpertick * 900) / 1000;
		//	if (T->countdownNote >= T->msecpertick) T->countdownNote = 0;

		T->TickOut = ((Tick->vel / 2) + (T->CurrentPattern.Ticks[T->Tick].accent * 127)) * (4096 / 256);

		if (T->CurrentPattern.Ticks[T->Tick].note != TUESDAY_NOTEOFF)
		{
			T->CVOutTarget = (DAC_NOTE(T->CurrentPattern.Ticks[T->Tick].note, 0)) << 16;
			if (Tick->slide > 0)
			{
				T->CVOutDelta = (T->CVOutTarget - T->CVOut) / (Tick->slide * 50);
				T->CVOutCountDown = Tick->slide * 50;
			}
			else
			{
				T->CVOut = T->CVOutTarget;
			}
			T->lastnote = T->CurrentPattern.Ticks[T->Tick].note;
			int Ticks = (T->msecpertick * T->CurrentPattern.Ticks[T->Tick].maxsubticklength)/T->ticklengthscaler;;
			if (T->Gates[GATE_GATE] > 0) { T->Gates[GATE_GATE] = -Ticks; T->GatesGap[GATE_GATE] = GATE_MINGATETIME; }
			else T->Gates[GATE_GATE] = Ticks;

			if (T->CurrentPattern.Ticks[T->Tick].accent > 0)
			{
				T->Gates[GATE_ACCENT] = T->Gates[GATE_GATE];
				T->GatesGap[GATE_ACCENT] = T->GatesGap[GATE_GATE];
			}
			else
			{
				T->Gates[GATE_ACCENT] = 0;
			}
		}
		if (T->CurrentPattern.Ticks[T->Tick].note == TUESDAY_NOTEOFF)
		{
			T->TickOut = 0;
			T->Gates[GATE_GATE] = 0;
			T->Gates[GATE_ACCENT] = 0;
			T->lastnote = T->CurrentPattern.Ticks[T->Tick].note;
		}
	}

	if (T->Tick == 0)
	{
		T->Gates[GATE_LOOP] = GATE_MINGATETIME;
	}

	if (T->Tick % T->CurrentPattern.TPB == 0)
	{
		T->Gates[GATE_BEAT] = GATE_MINGATETIME;
	}
	T->Gates[GATE_TICK] = GATE_MINGATETIME;
}

void Tuesday_TimerTick( Tuesday_PatternGen *T,  Tuesday_Params *P)
{
	T->timesincelastclocktick++;
	T->msecsincelasttick++;
	int clockmode = 1;
	if (T->clockup == 0 && T->timesincelastclocktick > 2000)
	{
		T->timesincelastclocktick = 3000;
		clockmode = 0;	
	}

	T->T++;

	if (T->T % 2 == 0)
	{

		//		if (T->countdownNote >= 0)
		//		{
		//			T->countdownNote--;
		//			if (T->countdownNote <= 0)
		//			{
		//				T->TickOut = 0;
		//				T->Gates[GATE_GATE] = 0;
		//			}
		//		}

		int bpm = 1 + (200 * T->tempo) / 256;
		int msecperbeat = (1000 * 60) /__max(1,  (T->TicksPerMeasure * (bpm / 4)));


		if (clockmode == 0)
		{
			T->countdownTick--;

			if (T->countdownTick > msecperbeat)
				T->countdownTick = msecperbeat;
			if (T->countdownTick <= 0 || T->directtick == 1)
			{
				DoClock(1);

				T->directtick = 0;
				T->countdownTick = msecperbeat;
			}
			else
			{
				DoClock(0);
			}
		}

		if (T->CVOutCountDown > 0)
		{
			T->CVOut += T->CVOutDelta;
			T->CVOutCountDown--;
			if (T->CVOutCountDown == 0) T->CVOut = T->CVOutTarget;
		}
		else
		{
			T->CVOut = T->CVOutTarget;
		}

	}


	for (int i = 0; i < 6; i++)
	{
		if (T->Gates[i] > 0)
		{
			T->Gates[i]--;
			T->GatesGap[i] = 0;
		}
		else
		{
			if (T->GatesGap[i] > 0)
			{
				T->GatesGap[i]--;
			}
			else
			{
				T->Gates[i] = -T->Gates[i];
			}
		}
	}

};

int Tuesday_IsClockPassed(Tuesday_PatternGen *P, Tuesday_Settings *S)
{
	if ((S->ClockMode & CLOCKMODE_RESET_BLOCKS_TICKS) == 0)
	{
		return 1;
	}

	if (P->LastResetVal == GetResetEq(S))
	{
		return 0;
	}
	return 1;
}

void Tuesday_Clock( Tuesday_PatternGen *P,  Tuesday_Settings *S,  Tuesday_Params *Par, int ClockVal)
{
	if (ClockVal == 1 && (Tuesday_IsClockPassed(P, S)==1))
	{
		P->Gates[GATE_CLOCK] = GATE_MINGATETIME;
		Tuesday_SetupClockSubdivision(P, S, Par);

		if (P->clockssincereset >= P->TicksPerMeasure)
		{
			P->clockssincereset = 0;
			P->Measure++;
			if (P->Measure * P->CurrentPattern.TPB * 4 >= P->CurrentPattern.Length) P->Measure = 0;
		}


		//if (clockshad >= 96 / (Pattern.TPB * 4) || directtick == 1)
		P->NewTick = (P->Measure * (P->CurrentPattern.TPB * 4) + ((P->clockssincereset * (P->CurrentPattern.TPB * 4)) / P->TicksPerMeasure)) % P->CurrentPattern.Length;
		if (P->DoReset == 1)
		{
			P->DoReset = 0;
			//	P->Tick = 0;
			P->directtick = 1;
			P->NewTick = 0;
		}
		if (P->NewTick != P->Tick || P->directtick == 1)
		{
			//#define USE_SEMIHOST
			//printf("%d %d\n", Measure, NewTick);
			//if (P->Tick == -1)
			{
				//			P->Tick = 0;
				//NewTick = 0;
			}
			//	printf("NT: %d - Tick: %d - delta %d \n", P->NewTick, P->Tick);

			P->Tick = P->NewTick;
			doTick();
			P->directtick = 0;
			P->clockshad = 0;
		}

		P->clockshad++;
		P->clockssincereset++;

	}
	else
	{
		P->Gates[GATE_CLOCK] = 0;
	}
}


uint32_t KnobOpt(uint32_t val)
{
	int r = 0;
	if (val > (65536 * 1) / 5) r++;
	if (val > (65536 * 2) / 5) r++;
	if (val > (65536 * 3) / 5) r++;
	if (val > (65536 * 4) / 5) r++;
	return 1 + 4 - r;
}
int GetClockPolarityEq(Tuesday_Settings *Settings)
{
	if ((Settings->ClockMode & CLOCKMODE_DOWNSLOPE) > 0) return 0;
	return 1;
}

void Tuesday_ExtClock( Tuesday_PatternGen *P,  Tuesday_Params *Params,  Tuesday_Settings *Settings, int state)
{
	P->clockup = state;

	int clocksubdiv = KnobOpt(P->tempo << 8);

	if (P->lastclocksubdiv != clocksubdiv)
	{
		P->clockssincereset = P->extclockssincereset[clocksubdiv];
		P->lastclocksubdiv = clocksubdiv;
	}

	if (state == GetClockPolarityEq(Settings))
	{
		if ((P->extclocksupsincereset % clocksubdiv) == 0)
		{
			Tuesday_Clock(P, Settings, Params, 1);
		}
		P->extclocksupsincereset = (P->extclocksupsincereset + 1) % clocksubdiv;
	}
	else
	{
		if ((P->extclocksdowsincereset % clocksubdiv) == 0)
		{
			Tuesday_Clock(P, Settings, Params, 0);
		}
		P->extclocksdowsincereset = (P->extclocksdowsincereset + 1) % clocksubdiv;
	}

	if (state == GetClockPolarityEq(Settings))
	{
		for (int i = 1; i < 6; i++)
		{

			P->extclockssinceresetcounter[i]++;
			if (P->extclockssinceresetcounter[i] == i)
			{
				P->extclockssinceresetcounter[i] = 0;
				P->extclockssincereset[i] = (P->extclockssincereset[i] + 1) % P->TicksPerMeasure;
			}
		}

	}
	P->timesincelastclocktick = 0;
}

void Tuesday_ValidateParams( Tuesday_Params *P)
{
	P->algo = P->algo % TUESDAY_MAXALGO;
	P->beatopt = P->beatopt % TUESDAY_MAXBEAT;
	P->tpbopt = P->tpbopt % TUESDAY_MAXTPB;
	P->scale = P->scale % TUESDAY_MAXSCALE;
}


void Tuesday_InitScales(Tuesday_Settings *S)
{
#ifdef ALTERNATESCALES
S->scale[0] = SCALE_MELODICMINOR;
S->scale[1] = SCALE_MINORPENTA;
S->scale[2] = SCALE_LOCRIAN;
S->scale[3] = SCALE_OCTATONIC;



#else

S->scale[0] = SCALE_MAJOR;
S->scale[1] = SCALE_MINOR;
S->scale[2] = SCALE_DORIAN;
S->scale[3] = SCALE_BLUES;
#endif

S->scales[SCALE_MINORPENTA].notes[0] = 0;
S->scales[SCALE_MINORPENTA].notes[1] = 3;
S->scales[SCALE_MINORPENTA].notes[2] = 5;
S->scales[SCALE_MINORPENTA].notes[3] = 7;
S->scales[SCALE_MINORPENTA].notes[4] = 10;
S->scales[SCALE_MINORPENTA].count = 5; // Pentatonic


S->scales[SCALE_MAJOR].notes[0] = 0;
S->scales[SCALE_MAJOR].notes[1] = 2;
S->scales[SCALE_MAJOR].notes[2] = 4;
S->scales[SCALE_MAJOR].notes[3] = 5;
S->scales[SCALE_MAJOR].notes[4] = 7;
S->scales[SCALE_MAJOR].notes[5] = 9;
S->scales[SCALE_MAJOR].notes[6] = 11;
S->scales[SCALE_MAJOR].count = 7; // Major scale

S->scales[SCALE_MAJORTRIAD].notes[0] = 0;
S->scales[SCALE_MAJORTRIAD].notes[1] = 4;
S->scales[SCALE_MAJORTRIAD].notes[2] = 7;
S->scales[SCALE_MAJORTRIAD].notes[3] = 11;
S->scales[SCALE_MAJORTRIAD].count = 4; // Major scale triad

S->scales[SCALE_MINOR].notes[0] = 0;
S->scales[SCALE_MINOR].notes[1] = 2;
S->scales[SCALE_MINOR].notes[2] = 3;
S->scales[SCALE_MINOR].notes[3] = 5;
S->scales[SCALE_MINOR].notes[4] = 7;
S->scales[SCALE_MINOR].notes[5] = 8;
S->scales[SCALE_MINOR].notes[6] = 10;
S->scales[SCALE_MINOR].count = 7; // Minor scale

S->scales[SCALE_MINORTRIAD].notes[0] = 0;
S->scales[SCALE_MINORTRIAD].notes[1] = 3;
S->scales[SCALE_MINORTRIAD].notes[2] = 7;
S->scales[SCALE_MINORTRIAD].notes[3] = 10; // aaaarghhh!
S->scales[SCALE_MINORTRIAD].count = 4; // Minor scale triad

S->scales[SCALE_PENTA].notes[0] = 0;
S->scales[SCALE_PENTA].notes[1] = 2;
S->scales[SCALE_PENTA].notes[2] = 5;
S->scales[SCALE_PENTA].notes[3] = 7;
S->scales[SCALE_PENTA].notes[4] = 9;
S->scales[SCALE_PENTA].count = 5; // Pentatonic

S->scales[SCALE_BLUES].notes[0] = 0;
S->scales[SCALE_BLUES].notes[1] = 3;
S->scales[SCALE_BLUES].notes[2] = 5;
S->scales[SCALE_BLUES].notes[3] = 6;
S->scales[SCALE_BLUES].notes[4] = 7;
S->scales[SCALE_BLUES].notes[5] = 10;
S->scales[SCALE_BLUES].count = 6; // Blues

S->scales[SCALE_DORIAN].notes[0] = 0;
S->scales[SCALE_DORIAN].notes[1] = 2;
S->scales[SCALE_DORIAN].notes[2] = 3;
S->scales[SCALE_DORIAN].notes[3] = 6;
S->scales[SCALE_DORIAN].notes[4] = 7;
S->scales[SCALE_DORIAN].notes[5] = 9;
S->scales[SCALE_DORIAN].notes[6] = 10;
S->scales[SCALE_DORIAN].count = 7; // Dorian scale






S->scales[SCALE_12TONE].notes[0] = 0;
S->scales[SCALE_12TONE].notes[1] = 1;
S->scales[SCALE_12TONE].notes[2] = 2;
S->scales[SCALE_12TONE].notes[3] = 3;
S->scales[SCALE_12TONE].notes[4] = 4;
S->scales[SCALE_12TONE].notes[5] = 5;
S->scales[SCALE_12TONE].notes[6] = 6;
S->scales[SCALE_12TONE].notes[7] = 7;
S->scales[SCALE_12TONE].notes[8] = 8;
S->scales[SCALE_12TONE].notes[9] = 9;
S->scales[SCALE_12TONE].notes[10] = 10;
S->scales[SCALE_12TONE].notes[11] = 11;
S->scales[SCALE_12TONE].count = 12; // 12tone




S->scales[SCALE_EGYPTIAN].notes[0] = 0;
S->scales[SCALE_EGYPTIAN].notes[1] = 2;
S->scales[SCALE_EGYPTIAN].notes[2] = 5;
S->scales[SCALE_EGYPTIAN].notes[3] = 7;
S->scales[SCALE_EGYPTIAN].notes[4] = 10;
S->scales[SCALE_EGYPTIAN].count = 5; // Egyptian

S->scales[SCALE_PHRYGIAN].notes[0] = 0;
S->scales[SCALE_PHRYGIAN].notes[1] = 1;
S->scales[SCALE_PHRYGIAN].notes[2] = 4;
S->scales[SCALE_PHRYGIAN].notes[3] = 5;
S->scales[SCALE_PHRYGIAN].notes[4] = 7;
S->scales[SCALE_PHRYGIAN].notes[5] = 8;
S->scales[SCALE_PHRYGIAN].notes[6] = 10;
S->scales[SCALE_PHRYGIAN].count = 7; // Phrygian

S->scales[SCALE_LOCRIAN].notes[0] = 0;
S->scales[SCALE_LOCRIAN].notes[1] = 1;
S->scales[SCALE_LOCRIAN].notes[2] = 3;
S->scales[SCALE_LOCRIAN].notes[3] = 5;
S->scales[SCALE_LOCRIAN].notes[4] = 6;
S->scales[SCALE_LOCRIAN].notes[5] = 8;
S->scales[SCALE_LOCRIAN].notes[6] = 10;
S->scales[SCALE_LOCRIAN].count = 7; // Locrian

S->scales[SCALE_OCTATONIC].notes[0] = 0;
S->scales[SCALE_OCTATONIC].notes[1] = 2;
S->scales[SCALE_OCTATONIC].notes[2] = 3;
S->scales[SCALE_OCTATONIC].notes[3] = 5;
S->scales[SCALE_OCTATONIC].notes[4] = 6;
S->scales[SCALE_OCTATONIC].notes[5] = 8;
S->scales[SCALE_OCTATONIC].notes[6] = 9;
S->scales[SCALE_OCTATONIC].notes[7] = 11;
S->scales[SCALE_OCTATONIC].count = 8; // Octatonic

S->scales[SCALE_MELODICMINOR].notes[0] = 0;
S->scales[SCALE_MELODICMINOR].notes[1] = 2;
S->scales[SCALE_MELODICMINOR].notes[2] = 3;
S->scales[SCALE_MELODICMINOR].notes[3] = 5;
S->scales[SCALE_MELODICMINOR].notes[4] = 7;
S->scales[SCALE_MELODICMINOR].notes[5] = 9;
S->scales[SCALE_MELODICMINOR].notes[6] = 11;
S->scales[SCALE_MELODICMINOR].count = 7; // MelodicMinor /NOTDONE


}


void NOINLINE Tuesday_LoadDefaults( Tuesday_Settings *S,  Tuesday_Params *P)
{
	P->algo = 2;
	P->beatopt = 0;
	P->scale = 1;
	P->tpbopt = 2;

	S->algooptions[2] = ALGO_STOMPER + ALGO_ENABLE_SLIDES + ALGO_ENABLE_LENGTHS;
	S->algooptions[0] = ALGO_TRITRANCE + ALGO_ENABLE_SLIDES + ALGO_ENABLE_LENGTHS;
	S->algooptions[1] = ALGO_SAIKO_LEAD + ALGO_ENABLE_SLIDES + ALGO_ENABLE_LENGTHS;
	S->algooptions[3] = ALGO_WOBBLE + ALGO_ENABLE_SLIDES + ALGO_ENABLE_LENGTHS;

	S->tpboptions[0] = 2;
	S->tpboptions[1] = 3;
	S->tpboptions[2] = 4;
	S->tpboptions[3] = 5;

	S->beatoptions[0] = 4;
	S->beatoptions[1] = 8;
	S->beatoptions[2] = 16;
	S->beatoptions[3] = 32;

	S->ClockSubDivMode = CLOCKSUBDIV_24PPQN;

	S->ClockMode = CLOCKMODE_DOWNSLOPE | CLOCKMODE_RESETREGULAR | CLOCKMODE_RESET_TRIGGER_ONLY;

	S->OctaveLimiter = OCTAVELIMIT_OFF;

	for (int j = 0; j < __SCALE_COUNT; j++)
	{
		for (int i = 0; i < 12; i++)
		{
			S->scales[j].notes[i] = i;
		}
		S->scales[j].count = 1;
	}
	Tuesday_InitScales(S);

}

void Tuesday_LoadSettings( Tuesday_Settings *S,  Tuesday_Params *P)
{
	Tuesday_LoadDefaults(S, P);
}

const unsigned char dither[24 * 3] =
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

Tuesday_PatternFuncSpecific FuncSpecific[4];
Tuesday_Tick_t Ticks[4];
Tuesday_Tick_t Top;
Tuesday_Tick_t Bot;
Tuesday_RandomGen Randoms[4];

#pragma GCC push_options
#pragma GCC optimize ("Os")

void DefaultTick( Tuesday_Tick_t *Out)
{
	Out->maxsubticklength = TUESDAY_DEFAULTGATELENGTH;
	Out->slide = 0;
	Out->note = 12;
	Out->accent = 0;
	Out->vel = 255;
}

void RandomSlideAndLength( Tuesday_Tick_t *Out,  Tuesday_RandomGen *R)
{
	if (Tuesday_PercChance(R, 50))
	{
		Out->maxsubticklength = ((1 + (Tuesday_Rand(R) % 4)) * TUESDAY_SUBTICKRES) - 2;
	}
	else
	{
		Out->maxsubticklength = (TUESDAY_SUBTICKRES * 3) / 4;
	}

	if (Tuesday_BoolChance(R) && Tuesday_BoolChance(R))
	{
		Out->slide = (Tuesday_Rand(R) % 3) + 1;;
	}
	else
	{
		Out->slide = 0;
	}
}

void CopyTick( Tuesday_Tick_t *A,  Tuesday_Tick_t *Out)
{
	Out->accent = A->accent;
	Out->note = A->note;
	Out->vel = A->vel;
	Out->slide = A->slide;
	Out->maxsubticklength = A->maxsubticklength;
}

void ApplyDither(int tick, uint32_t ditherpattern,  Tuesday_Tick_t *A,  Tuesday_Tick_t *B,  Tuesday_Tick_t *Out)
{
	if (((ditherpattern >> (tick & 0b11)) & 1) == 1)
	{
		CopyTick(A, Out);
	}
	else
	{
		CopyTick(B, Out);
	}
}

void ApplySlideLength( Tuesday_Tick_t *T, int SlideMode, int LengthMode)
{
	if (SlideMode == 0) T->slide = 0;
	if (LengthMode == 0) T->maxsubticklength = TUESDAY_DEFAULTGATELENGTH;
}

void ApplyOctaveLimit( Tuesday_Settings *S,  Tuesday_Tick_t *T)
{
	if (S->OctaveLimiter == OCTAVELIMIT_OFF) return;
	ScaledNote SN;
	SN.note = T->note%12;
	SN.oct = (T->note/12) % S->OctaveLimiter;
	T->note = SN.note + (SN.oct * 12);

}

void Tuesday_Generate( Tuesday_PatternGen *T, Tuesday_Params *P,  Tuesday_Settings *S)
{

	int len = S->tpboptions[P->tpbopt] * S->beatoptions[P->beatopt];
	T->CurrentPattern.Length = len;
	T->CurrentPattern.TPB = S->tpboptions[P->tpbopt];
	int X = T->seed1 >> 3;
	int Y = T->seed2 >> 3;

	int XFade = (T->seed1 & 0b110) >> 1;
	int YFade = (T->seed2 & 0b110) >> 1;

	unsigned char xbase = T->seed1 & 0b1111 + ((T->seed1) >> 6);
	unsigned char ybase = T->seed2 & 0b1111 + ((T->seed2) >> 6);

	unsigned char ditherx = 0;
	unsigned char dithery = 0;

	if (XFade > 0) ditherx = dither[xbase * 3 + XFade - 1];
	if (YFade > 0) dithery = dither[ybase * 3 + YFade - 1];

	Tuesday_RandomSeed(&Randoms[0], X + Y * 32);
	Tuesday_RandomSeed(&Randoms[1], X + Y * 32 + 1);
	Tuesday_RandomSeed(&Randoms[2], X + Y * 32 + 32);
	Tuesday_RandomSeed(&Randoms[3], X + Y * 32 + 33);

	int CurrentAlgo = (S->algooptions[P->algo] & 0xf) % ALGO_COUNT;
	int SlideMode = (S->algooptions[P->algo] >> 4) & 1;
	int LengthMode = (S->algooptions[P->algo] >> 5) & 1;
	int LengthMultMode = (S->algooptions[P->algo] >> 6) & 1;
	if (LengthMultMode > 0) T->ticklengthscaler = 9; else T->ticklengthscaler = 3;
	PatternFunctions *Algo = &PatternTypes[CurrentAlgo];

	for (int j = 0; j < 4; j++)
	{
		Algo->Init(T, P, S, &Randoms[j], &FuncSpecific[j]);
	}

	Algo->PatternInit(T, P, S, &T->CurrentPattern);

	if (Algo->Dither == 1)
	{
		for (int i = 0; i < len; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Algo->Gen(T, P, S, &Randoms[j], &FuncSpecific[j], i, &Ticks[j]);
			}

			ApplyDither(i, ditherx, &Ticks[0], &Ticks[1], &Top);
			ApplyDither(i, ditherx, &Ticks[2], &Ticks[3], &Bot);
			ApplyDither(i, dithery, &Top, &Bot, &T->CurrentPattern.Ticks[i]);

			ApplySlideLength(&T->CurrentPattern.Ticks[i], SlideMode, LengthMode);
			ApplyOctaveLimit(S, &T->CurrentPattern.Ticks[i]);
		}
	}
	else
	{
		for (int i = 0; i < len; i++)
		{
			Algo->Gen(T, P, S, &Randoms[0], &FuncSpecific[0], i, &T->CurrentPattern.Ticks[i]);
			ApplySlideLength(&T->CurrentPattern.Ticks[i], SlideMode, LengthMode);
			ApplyOctaveLimit(S, &T->CurrentPattern.Ticks[i]);
		}
	}
}

#pragma GCC pop_options
