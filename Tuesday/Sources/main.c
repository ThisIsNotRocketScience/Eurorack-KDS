/* ###################################################################
 **     Filename    : main.c
 **     Project     : SonicScrewdriver
 **     Processor   : MKL02Z32VFM4
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2015-12-04, 13:49, # CodeGen: 0
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file main.c
 ** @version 01.01
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup main_module main module documentation
 **  @{
 */
/* MODULE main */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "AD1.h"
#include "AdcLdd1.h"
#include "PTA.h"
#include "DACSEL.h"
#include "CLOCKINT.h"
#include "SW_ALGO.h"
#include "SW_SCALE.h"
#include "LATCH.h"
#include "DATA.h"
#include "CLOCK.h"
#include "SW_TPB.h"
#include "SW_BEATS.h"
#include "RESETINT.h"
#include "CI2C1.h"
#include "PTB.h"
#include "KSDK1.h"
#include "WAIT1.h"
#include "SM1.h"
#include "TI1.h"
#include "TimerIntLdd1.h"
#include "TU1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */

#include <math.h>
#include <stdio.h>
#include "main.h"

volatile int measured = 0;

int adcchannels[ADC_Count];

#include "Tuesday.h"
#include "Algo.h"

#include "../../EurorackShared/EurorackShared.c"
#include "../../EurorackShared/Math.c"

struct Tuesday_PatternGen Tuesday;
struct Tuesday_Settings Settings;
struct Tuesday_Params Params;
struct Tuesday_Params LastParams;
struct Tuesday_RandomGen MainRandom;

int tickssincecommit = 0;


long oldseed = -1;
byte pwm = 0;

void _SetupLeds();

void UpdateGates()
{
	for (int i = 0; i < 6; i++)
	{
		if (Tuesday.Gates[i] > 0)
		{
			Tuesday.Gates[i]--;
		}
		else
		{
			Tuesday.Gates[i] = -Tuesday.Gates[i];
		}
	}
}

void ShiftOut()
{
	pwm += 16;

	LATCH_ClrVal(LATCH_DeviceData);
	for (int i = 0; i < 6; i++)
	{
		if (Tuesday.Gates[i] > 0)
		{
			DATA_ClrVal(DATA_DeviceData);
		}
		else
		{
			DATA_SetVal(DATA_DeviceData);
		}
		CLOCK_ClrVal(CLOCK_DeviceData);
		CLOCK_SetVal(CLOCK_DeviceData);
	}

	for (int i = 0; i < 16; i++)
	{
		if (Tuesday.StateLeds[i] > pwm)
		{
			DATA_SetVal(DATA_DeviceData);
		}
		else
		{
			DATA_ClrVal(DATA_DeviceData);
		}
		CLOCK_ClrVal(CLOCK_DeviceData);
		CLOCK_SetVal(CLOCK_DeviceData);
	}

	LATCH_SetVal(LATCH_DeviceData);
}

void doTick()
{
	Tuesday_Tick(&Tuesday,&Params);
	ShiftOut();
}

void PatternReset()
{
	Tuesday_Reset(&Tuesday);
}

void ExtClockTick(int state)
{
	Tuesday_ExtClock(&Tuesday,&Params, state);
}


void DoClock(int state)
{
	Tuesday_Clock(&Tuesday, state);
}


// half-millisecond timer -> update each dacchannel in turn
void doTimer()
{
	tickssincecommit++;
	switch(Tuesday.UIMode)
	{
	case UI_NORMAL:
	case UI_SELECTOPTION:
	{

		Tuesday_TimerTick(&Tuesday, &Params);

		if (Tuesday.T%2==0)
		{
			if (Tuesday.CVOutCountDown > 0)
			{
				Tuesday.CVOut  += Tuesday.CVOutDelta;
				Tuesday.CVOutCountDown--;
				if (Tuesday.CVOutCountDown == 0) Tuesday.CVOut = Tuesday.CVOutTarget;
			}
			else
			{
				Tuesday.CVOut = Tuesday.CVOutTarget;
			}
			DAC_Write(0, Tuesday.CVOut >> 16) ;
		}
		else
		{
			DAC_Write(1, Tuesday.TickOut);
		}
		UpdateGates();}
	break;

	case UI_CALIBRATION:
		break;
	}
	_SetupLeds();
	ShiftOut();
}

void SetLedNumber(int offset, int number)
{
	switch (number % 13)
	{
	case 0:
		Tuesday.StateLeds[offset + 0] = 255;
		Tuesday.StateLeds[offset + 1] = 0;
		Tuesday.StateLeds[offset + 2] = 0;
		Tuesday.StateLeds[offset + 3] = 0;
		break;
	case 1:
		Tuesday.StateLeds[offset + 0] = 0;
		Tuesday.StateLeds[offset + 1] = 255;
		Tuesday.StateLeds[offset + 2] = 0;
		Tuesday.StateLeds[offset + 3] = 0;
		break;
	case 2:
		Tuesday.StateLeds[offset + 0] = 0;
		Tuesday.StateLeds[offset + 1] = 0;
		Tuesday.StateLeds[offset + 2] = 255;
		Tuesday.StateLeds[offset + 3] = 0;
		break;
	case 3:
		Tuesday.StateLeds[offset + 0] = 0;
		Tuesday.StateLeds[offset + 1] = 0;
		Tuesday.StateLeds[offset + 2] = 0;
		Tuesday.StateLeds[offset + 3] = 255;
		break;
	case 4:
		Tuesday.StateLeds[offset + 0] = 255;
		Tuesday.StateLeds[offset + 1] = 0;
		Tuesday.StateLeds[offset + 2] = 0;
		Tuesday.StateLeds[offset + 3] = 255;
		break;
	case 5:
		Tuesday.StateLeds[offset + 0] = 255;
		Tuesday.StateLeds[offset + 1] = 255;
		Tuesday.StateLeds[offset + 2] = 0;
		Tuesday.StateLeds[offset + 3] = 0;
		break;
	case 6:
		Tuesday.StateLeds[offset + 0] = 0;
		Tuesday.StateLeds[offset + 1] = 255;
		Tuesday.StateLeds[offset + 2] = 255;
		Tuesday.StateLeds[offset + 3] = 0;
		break;
	case 7:
		Tuesday.StateLeds[offset + 0] = 0;
		Tuesday.StateLeds[offset + 1] = 0;
		Tuesday.StateLeds[offset + 2] = 255;
		Tuesday.StateLeds[offset + 3] = 255;
		break;
	case 8:
		Tuesday.StateLeds[offset + 0] = 255;
		Tuesday.StateLeds[offset + 1] = 0;
		Tuesday.StateLeds[offset + 2] = 255;
		Tuesday.StateLeds[offset + 3] = 255;
		break;
	case 9:
		Tuesday.StateLeds[offset + 0] = 255;
		Tuesday.StateLeds[offset + 1] = 255;
		Tuesday.StateLeds[offset + 2] = 0;
		Tuesday.StateLeds[offset + 3] = 255;
		break;
	case 10:
		Tuesday.StateLeds[offset + 0] = 255;
		Tuesday.StateLeds[offset + 1] = 255;
		Tuesday.StateLeds[offset + 2] = 255;
		Tuesday.StateLeds[offset + 3] = 0;
		break;
	case 11:
		Tuesday.StateLeds[offset + 0] = 0;
		Tuesday.StateLeds[offset + 1] = 255;
		Tuesday.StateLeds[offset + 2] = 255;
		Tuesday.StateLeds[offset + 3] = 255;
		break;
	case 12:
		Tuesday.StateLeds[offset + 0] = 255;
		Tuesday.StateLeds[offset + 1] = 255;
		Tuesday.StateLeds[offset + 2] = 255;
		Tuesday.StateLeds[offset + 3] = 255;
		break;
	}
}

#define VERSIONBYTE 0x10

void SaveEeprom()
{
	int paramsize = sizeof(Params);
	EE24_WriteByte(0, VERSIONBYTE);
	EE24_WriteBlock(1, (byte *)&Params, paramsize);
}

void SaveSettingsEeprom()
{
	int settingssize = sizeof(Settings);
	EE24_WriteByte(EEPROM_SETTINGSBASE, VERSIONBYTE);
	EE24_WriteBlock(EEPROM_SETTINGSBASE+1, (byte *)&Settings, settingssize);
}

void SaveCalibrationEeprom()
{
	int calibrationsize = sizeof(MasterCalibration);
	EE24_WriteByte(EEPROM_CALIBRATIONBASE, VERSIONBYTE);
	EE24_WriteBlock(EEPROM_CALIBRATIONBASE+1, (byte *)&MasterCalibration, calibrationsize);
}

void LoadEepromSettings()
{
	byte Ver;
	Ver = EE24_ReadByte(EEPROM_SETTINGSBASE);
	if (Ver == VERSIONBYTE)
	{
		int settingssize = sizeof(Settings);

		EE24_ReadBlock(EEPROM_SETTINGSBASE+ 1, (byte *)&Settings, settingssize);
	}
	else
	{
		SaveSettingsEeprom();
	}
}

void LoadEepromCalibration()
{
	byte Ver;
	Ver = EE24_ReadByte(EEPROM_CALIBRATIONBASE);
	if (Ver == VERSIONBYTE)
	{
		int calibrationsize = sizeof(MasterCalibration);
		EE24_ReadBlock(EEPROM_CALIBRATIONBASE+ 1, (byte *)&MasterCalibration, calibrationsize);
	}
	else
	{
		SaveCalibrationEeprom();
	}
}

void LoadEeprom()
{
	byte Ver;
	Ver = EE24_ReadByte(0);
	if (Ver == VERSIONBYTE)
	{
		int paramsize = sizeof(Params);
		EE24_ReadBlock(1, (byte *)&Params, paramsize);

		Tuesday_ValidateParams(&Params);
	}
	else
	{
		SaveEeprom();
	}
}

void ShowSets(int algogroup, int scalegroup, int ticksgroup, int beatsgroup)
{
	if (ticksgroup > -1) SetLedNumber(0, ticksgroup);
	if (beatsgroup > -1) SetLedNumber(4, beatsgroup);
	if (scalegroup > -1) SetLedNumber(8, scalegroup);
	if (algogroup > -1) SetLedNumber(12, algogroup);
}

void _SetupLeds()
{
	switch(Tuesday.UIMode)
	{
	case UI_NORMAL:
		SetLedNumber(0, Params.tpbopt);
		SetLedNumber(4, Params.beatopt);
		SetLedNumber(8, Params.scale);
		SetLedNumber(12, Params.algo);
		break;

	case UI_SELECTOPTION:
	{
		for (int i = 0; i < TUESDAY_LEDS; i++) Tuesday.StateLeds[i] = 0;
		int D = -1 ;
		if ((tickssincecommit>>7) & 0x1) D = Tuesday.OptionIndex;

		switch(Tuesday.OptionSelect)
		{
		case OPTION_ALGO:
		{
			int S = Settings.algooptions[Tuesday.OptionIndex];
			ShowSets(D, S&0x3, (S>>2)&0x03, (S>>4)&0x03);
		}
		break;
		case OPTION_SCALE:
		{
			int S = Settings.scale[Tuesday.OptionIndex];
			ShowSets(S & 0x3, D, (S >> 2) & 0x03, (S>>4)&0x03);
		}
		break;
		case OPTION_BEATS:
			{
				int S = Settings.beatoptions[Tuesday.OptionIndex]-1;
				ShowSets( S&0x3, (S>>2)&0x03, (S>>4)&0x03, D);
			}
			break;
		case OPTION_TPB:
			{
				int S = Settings.tpboptions[Tuesday.OptionIndex]-1;
				ShowSets( S&0x3, (S>>2)&0x03, D, (S>>4)&0x03);
			}
			break;
		}
	}break;

	case UI_CALIBRATION:
		for (int i = 0;i<TUESDAY_LEDS;i++) Tuesday.StateLeds[i] = 0;
		switch(Tuesday.CalibTarget)
		{
		case CALIBRATION_VEL:
			Tuesday.StateLeds[0] = 255;
			Tuesday.StateLeds[1] = 255;
			Tuesday.StateLeds[2] = 255;
			Tuesday.StateLeds[3] = 255;
			break;
		case CALIBRATION_NOTE:

			Tuesday.StateLeds[4] = 255;
			Tuesday.StateLeds[5] = 255;
			Tuesday.StateLeds[6] = 255;
			Tuesday.StateLeds[7] = 255;
			break;
		case CALIBRATION_NOTARGET:
		{
			unsigned char T = Triangle(tickssincecommit << 24)>>24;
			for(int i = 0;i<4;i++)
			{
				Tuesday.StateLeds[i] = T;
				Tuesday.StateLeds[i+4] = ~T;
			}
		}
		break;
		}
		break;
	}
}

void SwitchToOptionMode(int mode, int startoption)
{
	Tuesday.OptionSelect = mode;
	Tuesday.OptionIndex = startoption;
	Tuesday.UIMode = UI_SELECTOPTION;
}

int algosw = 0;
int scalesw = 0;
int beatsw = 0;
int tpbsw = 0;
struct denoise_state_t algosw_state = {0};
struct denoise_state_t scalesw_state = {0};
struct denoise_state_t beatsw_state = {0};
struct denoise_state_t tpbsw_state = {0};


void UpdateButtons()
{
	algosw = denoise(SW_ALGO_GetVal(0), &algosw_state);
	scalesw = denoise(SW_SCALE_GetVal(0), &scalesw_state);
	beatsw = denoise(SW_BEATS_GetVal(0), &beatsw_state);
	tpbsw = denoise(SW_TPB_GetVal(0), &tpbsw_state);
}


/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */


#ifdef USE_SEMIHOST
	initialise_monitor_handles();
#endif

	Tuesday_Init(&Tuesday);
	Tuesday_LoadSettings(&Settings, &Params);
	Tuesday_RandomSeed(&MainRandom, oldseed);
	EuroRack_InitCalibration();

	LoadEeprom();

	TI1_Enable();
	AD1_Measure(FALSE);
	int StartWithCalibration =0 ;
	for(int j =0 ;j<16;j++)
	{
		for (int i =0 ;i<16;i++)
		{
			Tuesday.StateLeds[15-i] = j==i?255:0;
			UpdateButtons();
			StartWithCalibration += scalesw_state.down;
		}
		ShiftOut();
		WAIT1_Waitms(40);
	}

	for (int i = 0;i < 16;i++)
	{
		Tuesday.StateLeds[i] = 0;
	}

	_SetupLeds();
	ShiftOut();
	int switchmode = 1;



	if (StartWithCalibration > 16*5)
	{
		Tuesday.UIMode = UI_CALIBRATION;
		Tuesday.CalibTarget =CALIBRATION_NOTARGET;

	};

	byte commitchange = 0;
	for (;;)
	{
		UpdateButtons();

		switch (Tuesday.UIMode)
		{


		case UI_SELECTOPTION:
		{
			int butconf,but1, but2, but3;
			int butconflong =0;;
			int S = 0;
			int MaxS = 64;
			switch(Tuesday.OptionSelect)
			{
			case OPTION_ALGO:
			{
				butconf = algosw;
				butconflong = islongpress(&algosw_state);
				S = Settings.algooptions[Tuesday.OptionIndex];
				//MaxS = __ALGO_COUNT;

				but1 = scalesw;
				but3 = beatsw;
				but2 = tpbsw;
			};
			break;

			case OPTION_SCALE:
			{
				butconf = scalesw;
				butconflong = islongpress(&scalesw_state);
				S = Settings.scale[Tuesday.OptionIndex];
		//		MaxS = __SCALE_COUNT;

				but1 = algosw;
				but3 = beatsw;
				but2 = tpbsw;

			};
			break;

			case OPTION_BEATS:
			{
				butconf = beatsw;
				butconflong = islongpress(&beatsw_state);
				//Settings.beatoptions[]
				but1 = algosw;
				but2 = scalesw;
				but3 = tpbsw;
			};
			break;

			case OPTION_TPB:
			{
				butconf = tpbsw;
				butconflong = islongpress(&tpbsw_state);

				but1 = algosw;
				but2 = scalesw;
				but3 = beatsw;
			};
			break;
			}

			int NewS = S;

			if (but1) NewS = (S & (0xff - (3 << 0))) + (((((S >> 0) & 3) + 1) & 3) << 0);
			if (but2) NewS = (S & (0xff - (3 << 2))) + (((((S >> 2) & 3) + 1) & 3) << 2);
			if (but3) NewS = (S & (0xff - (3 << 4))) + (((((S >> 4) & 3) + 1) & 3) << 4);

			if (NewS != S)
			{
				S = NewS;

				switch(Tuesday.OptionSelect)
				{
				case OPTION_ALGO:  Settings.algooptions[Tuesday.OptionIndex] =  S ; break;
				case OPTION_BEATS: Settings.beatoptions[Tuesday.OptionIndex] =  (S+1) ; break;
				case OPTION_TPB:   Settings.tpboptions[Tuesday.OptionIndex] =  (S+1) ; break;
				case OPTION_SCALE: Settings.scale[Tuesday.OptionIndex] =  S ; break;
				}

				// save to eeprom
			}
			if (butconf == 1)
			{
				Tuesday.OptionIndex = (Tuesday.OptionIndex + 1) % 4;
			}
			if (butconflong == 1)
			{
				// save eeprom here and exit!

				switch(Tuesday.OptionSelect)
				{
				case OPTION_ALGO:  Params.algo = Tuesday.OptionIndex; break;
				case OPTION_BEATS: Params.beatopt = Tuesday.OptionIndex; break;
				case OPTION_TPB:   Params.tpbopt = Tuesday.OptionIndex; break;
				case OPTION_SCALE: Params.scale = Tuesday.OptionIndex; break;
				}


				Tuesday.UIMode = UI_NORMAL;
			}
			if (measured == 1)
			{
				measured = 0;
				Tuesday.seed1 = (adcchannels[ADC_INX] >> 8);
				Tuesday.seed2 = (adcchannels[ADC_INY] >> 8);
				Tuesday.intensity = (adcchannels[ADC_INTENSITY] >> 8);
				Tuesday.tempo = (adcchannels[ADC_TEMPO] >> 8);
				AD1_Measure(FALSE);
			}
		}
		break;

		case UI_CALIBRATION:
		{
			if (algosw == 1)
			{
				Tuesday.UIMode = UI_NORMAL;
			}

			if (measured == 1)
			{
				measured = 0;
				Tuesday.seed1 = (adcchannels[ADC_INX] >> 8);
				Tuesday.seed2 = (adcchannels[ADC_INY] >> 8);
				Tuesday.intensity = (adcchannels[ADC_INTENSITY] >> 8);
				Tuesday.tempo = (adcchannels[ADC_TEMPO] >> 8);
				AD1_Measure(FALSE);
			}

			if (beatsw == 1)
			{
				Tuesday.CalibTarget = CALIBRATION_NOTE;
			}
			else
			{
				if (beatsw == 2)
				{
					if (Tuesday.CalibTarget == CALIBRATION_NOTE)
					{
						// Write Velocity output calibration value!
						Tuesday.CalibTarget = CALIBRATION_NOTARGET;
					}
				}
			}

			if (tpbsw == 1)
			{
				Tuesday.CalibTarget = CALIBRATION_VEL;
			}
			else
			{
				if (tpbsw == 2)
				{
					if (Tuesday.CalibTarget == CALIBRATION_VEL)
					{
						//					Write Note output calibration value!
						Tuesday.CalibTarget = CALIBRATION_NOTARGET;
					}
				}
			}
		}
		break;

		case UI_NORMAL:
		{

			{
				if (algosw == 1)
				{
					switchmode = 1;
					Params.algo = (Params.algo + 1) % TUESDAY_MAXALGO;
					commitchange = 1;
				}
				else
				{
					if (islongpress(&algosw_state)) // longpress!
					{
						//Params.algo = (Params.algo + TUESDAY_MAXALGO - 1) % TUESDAY_MAXALGO;
						SwitchToOptionMode(OPTION_ALGO, Params.algo);
					}
				}

				if (scalesw == 1)
				{
					switchmode = 1;
					Params.scale = (Params.scale + 1) % TUESDAY_MAXSCALE;
					commitchange = 1;
				}
				else
				{
					if (islongpress(&scalesw_state)) // longpress!
					{

						SwitchToOptionMode(OPTION_SCALE, Params.scale);
					}
				}

				if (beatsw == 1)
				{
					switchmode = 1;
					Params.beatopt = (Params.beatopt + 1) % TUESDAY_MAXBEAT;

					commitchange = 1;
				}
				else
				{
					if (islongpress(&beatsw_state)) // longpress!
					{
						//Params.beatopt = (Params.beatopt + TUESDAY_MAXBEAT -1) % TUESDAY_MAXBEAT;

						SwitchToOptionMode(OPTION_BEATS, Params.beatopt);
					}
				}

				if (tpbsw == 1)
				{
					switchmode = 1;

					Settings.beatoptions[Params.beatopt];

					Params.tpbopt = (Params.tpbopt + 1) % TUESDAY_MAXTPB;
					commitchange = 1;
				}
				else
				{
					if (islongpress(&tpbsw_state)) // longpress!
					{
						//Params.tpbopt = (Params.tpbopt + TUESDAY_MAXTPB - 1) % TUESDAY_MAXTPB;

						SwitchToOptionMode(OPTION_TPB, Params.tpbopt);
					}
				}
			}

			if (measured == 1)
			{
				measured = 0;
				Tuesday.seed1 = (adcchannels[ADC_INX] >> 8);
				Tuesday.seed2 = (adcchannels[ADC_INY] >> 8);
				Tuesday.intensity = (adcchannels[ADC_INTENSITY] >> 8);
				Tuesday.tempo = (adcchannels[ADC_TEMPO] >> 8);
				AD1_Measure(FALSE);
			}

			// read the X/Y seed knobs
			long newseed = (adcchannels[0] >> 8) + ((adcchannels[1] >> 8) << 8);
			if (newseed != oldseed)
				switchmode = 1;

			if (switchmode == 1)
			{
				//SetupLeds();
				// updated pattern needed for some reason!

				switchmode = 0;
				Tuesday_RandomSeed(&MainRandom, newseed);
				oldseed = newseed;



				Tuesday_Generate(&Tuesday, &Params, &Settings);
			}

			if (commitchange == 1 && tickssincecommit >= 10)
			{
				SaveEeprom();
				commitchange = 0;
				tickssincecommit = 0;
			}
		}
		break;
		};
	}
	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
	/*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
#ifdef PEX_RTOS_START
	PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
#endif
	/*** End of RTOS startup code.  ***/
	/*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
	for(;;){}
	/*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.5 [05.21]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */
