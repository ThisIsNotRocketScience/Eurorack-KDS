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

struct denoise_state_t algosw_state = {0};
struct denoise_state_t scalesw_state = {0};
struct denoise_state_t beatsw_state = {0};
struct denoise_state_t tpbsw_state = {0};


void UpdateButtons()
{
	denoise(SW_ALGO_GetVal(0), &algosw_state);
	denoise(SW_SCALE_GetVal(0), &scalesw_state);
	denoise(SW_BEATS_GetVal(0), &beatsw_state);
	denoise(SW_TPB_GetVal(0), &tpbsw_state);
}

void UpdateGates()
{
	for (int i = 0; i < 6; i++)
	{
		if (Tuesday.Gates[i] > 0)
		{
			Tuesday.Gates[i]--;
			Tuesday.GatesGap[i] = 0;
		}
		else
		{
			if (Tuesday.GatesGap[i] > 0)
			{
				Tuesday.GatesGap[i]--;
			}
			else
			{
				Tuesday.Gates[i] = -Tuesday.Gates[i];
			}
		}
	}
}

void __attribute__ ((noinline)) ShiftOut()
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
		if (Tuesday.RStateLeds[i] > pwm)
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
	UpdateButtons();

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
		UpdateGates();
	}
	break;

	case UI_CALIBRATION:
	{
		uint32_t CalibPattern = 0;
		int chan = 0;
		switch (Tuesday.CalibTarget)
		{
		case CALIBRATION_NOTE:chan = 0; break;
		case CALIBRATION_VEL: chan = 1; break;
		};

		if (Tuesday.intensity < 100)
		{
			CalibPattern = DAC_NOTE(12 * 1, chan);
		}
		else
		{
			if (Tuesday.intensity > 128+28)
			{
				CalibPattern = DAC_NOTE(12*3, chan);
			}
			else
			{
				CalibPattern = ((Tuesday.T >> 4)& 0x01) ? DAC_NOTE(12*1, chan): DAC_NOTE(12*3, chan);
			}
		}

		uint32_t CalibNote =0 ;
		uint32_t CalibVel =0 ;
		Tuesday.T++;

		switch(Tuesday.CalibTarget)
		{
		case CALIBRATION_NOTE: CalibNote = CalibPattern; CalibVel= 0;break;
		case CALIBRATION_VEL: CalibNote = 0; CalibVel= CalibPattern;break;
		}

		if (Tuesday.T%2 == 0)
		{
			DAC_Write(0,CalibNote);
		}
		else
		{
			DAC_Write(1,CalibVel);
		}
	}
	break;
	}

	if (Tuesday.T%2 ==0)
	{
		_SetupLeds();
	}

	ShiftOut();
}

void SetLedNumber(int offset, int number)
{
	switch (number % 13)
	{
	case 0:
		Tuesday.StateLedTargets[offset + 0] = 255;
		Tuesday.StateLedTargets[offset + 1] = 0;
		Tuesday.StateLedTargets[offset + 2] = 0;
		Tuesday.StateLedTargets[offset + 3] = 0;
		break;
	case 1:
		Tuesday.StateLedTargets[offset + 0] = 0;
		Tuesday.StateLedTargets[offset + 1] = 255;
		Tuesday.StateLedTargets[offset + 2] = 0;
		Tuesday.StateLedTargets[offset + 3] = 0;
		break;
	case 2:
		Tuesday.StateLedTargets[offset + 0] = 0;
		Tuesday.StateLedTargets[offset + 1] = 0;
		Tuesday.StateLedTargets[offset + 2] = 255;
		Tuesday.StateLedTargets[offset + 3] = 0;
		break;
	case 3:
		Tuesday.StateLedTargets[offset + 0] = 0;
		Tuesday.StateLedTargets[offset + 1] = 0;
		Tuesday.StateLedTargets[offset + 2] = 0;
		Tuesday.StateLedTargets[offset + 3] = 255;
		break;
	case 4:
		Tuesday.StateLedTargets[offset + 0] = 255;
		Tuesday.StateLedTargets[offset + 1] = 0;
		Tuesday.StateLedTargets[offset + 2] = 0;
		Tuesday.StateLedTargets[offset + 3] = 255;
		break;
	case 5:
		Tuesday.StateLedTargets[offset + 0] = 255;
		Tuesday.StateLedTargets[offset + 1] = 255;
		Tuesday.StateLedTargets[offset + 2] = 0;
		Tuesday.StateLedTargets[offset + 3] = 0;
		break;
	case 6:
		Tuesday.StateLedTargets[offset + 0] = 0;
		Tuesday.StateLedTargets[offset + 1] = 255;
		Tuesday.StateLedTargets[offset + 2] = 255;
		Tuesday.StateLedTargets[offset + 3] = 0;
		break;
	case 7:
		Tuesday.StateLedTargets[offset + 0] = 0;
		Tuesday.StateLedTargets[offset + 1] = 0;
		Tuesday.StateLedTargets[offset + 2] = 255;
		Tuesday.StateLedTargets[offset + 3] = 255;
		break;
	case 8:
		Tuesday.StateLedTargets[offset + 0] = 255;
		Tuesday.StateLedTargets[offset + 1] = 0;
		Tuesday.StateLedTargets[offset + 2] = 255;
		Tuesday.StateLedTargets[offset + 3] = 255;
		break;
	case 9:
		Tuesday.StateLedTargets[offset + 0] = 255;
		Tuesday.StateLedTargets[offset + 1] = 255;
		Tuesday.StateLedTargets[offset + 2] = 0;
		Tuesday.StateLedTargets[offset + 3] = 255;
		break;
	case 10:
		Tuesday.StateLedTargets[offset + 0] = 255;
		Tuesday.StateLedTargets[offset + 1] = 255;
		Tuesday.StateLedTargets[offset + 2] = 255;
		Tuesday.StateLedTargets[offset + 3] = 0;
		break;
	case 11:
		Tuesday.StateLedTargets[offset + 0] = 0;
		Tuesday.StateLedTargets[offset + 1] = 255;
		Tuesday.StateLedTargets[offset + 2] = 255;
		Tuesday.StateLedTargets[offset + 3] = 255;
		break;
	case 12:
		Tuesday.StateLedTargets[offset + 0] = 255;
		Tuesday.StateLedTargets[offset + 1] = 255;
		Tuesday.StateLedTargets[offset + 2] = 255;
		Tuesday.StateLedTargets[offset + 3] = 255;
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

void NOINLINE ShowSets(int algogroup, int scalegroup, int ticksgroup, int beatsgroup)
				{
	if (ticksgroup > -1) SetLedNumber(0, ticksgroup);
	if (beatsgroup > -1) SetLedNumber(4, beatsgroup);
	if (scalegroup > -1) SetLedNumber(8, scalegroup);
	if (algogroup > -1) SetLedNumber(12, algogroup);
				}

void NOINLINE _SetupLeds()
{
	switch(Tuesday.UIMode)
	{
	case UI_STARTUP:
		// give the opening animation some breathing space
		break;

	case UI_NORMAL:
		SetLedNumber(0, Params.tpbopt);
		SetLedNumber(4, Params.beatopt);
		SetLedNumber(8, Params.scale);
		SetLedNumber(12, Params.algo);
		break;

	case UI_SELECTOPTION:
	{
		for (int i = 0; i < TUESDAY_LEDS; i++) Tuesday.StateLedTargets[i] = 0;
		int D = -1 ;
		if ((tickssincecommit>>8) & 0x1) D = Tuesday.OptionIndex;

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
		for (int i = 0;i<TUESDAY_LEDS;i++) Tuesday.StateLedTargets[i] = 0;
		switch(Tuesday.CalibTarget)
		{
		case CALIBRATION_VEL:
			Tuesday.Gates[GATE_ACCENT] = 1;
			Tuesday.Gates[GATE_GATE] = 0;
			Tuesday.StateLedTargets[0] = 255;
			Tuesday.StateLedTargets[1] = 255;
			Tuesday.StateLedTargets[2] = 255;
			Tuesday.StateLedTargets[3] = 255;
			break;

		case CALIBRATION_NOTE:
			Tuesday.Gates[GATE_ACCENT] = 0;
			Tuesday.Gates[GATE_GATE] = 1;
			Tuesday.StateLedTargets[4] = 255;
			Tuesday.StateLedTargets[5] = 255;
			Tuesday.StateLedTargets[6] = 255;
			Tuesday.StateLedTargets[7] = 255;
			break;

		case CALIBRATION_NOTARGET:
		{
			Tuesday.Gates[GATE_ACCENT] = 0;
			Tuesday.Gates[GATE_GATE] = 0;
			unsigned char T = Triangle(tickssincecommit << 23)>>24;
			for(int i = 0;i<4;i++)
			{
				Tuesday.StateLedTargets[i] = T;
				Tuesday.StateLedTargets[i+4] = ~T;
			}
		}
		break;
		}
		break;
	}

	for (int i = 0; i < TUESDAY_LEDS; i++)
	{
		if (Tuesday.StateLedTargets[i] > Tuesday.RStateLeds[i])
		{
			Tuesday.RStateLeds[i]++;
		}
		else
		{
			if (Tuesday.StateLedTargets[i] < Tuesday.RStateLeds[i])
			{
				Tuesday.RStateLeds[i]--;
			}
			if (Tuesday.StateLedTargets[i] < Tuesday.RStateLeds[i])
			{
				Tuesday.RStateLeds[i]--;
			}
		}
	}
}

void SwitchToOptionMode(int mode, int startoption)
{
	Tuesday.OptionSelect = mode;
	Tuesday.OptionIndex = startoption;
	Tuesday.UIMode = UI_SELECTOPTION;
}

// reset basic options, but not calibration
void FactoryReset()
{
	Tuesday_LoadDefaults(&Settings, &Params);
	SaveSettingsEeprom();
}

void UI_SelectOption()
{
	int butconf,but1, but2, but3;
	int butconflong =0;;
	int S = 0;
	int MaxS = 64;
	switch(Tuesday.OptionSelect)
	{
	case OPTION_ALGO:
	{
		butconf = pressed(&algosw_state);
		butconflong = islongpress(&algosw_state);
		S = Settings.algooptions[Tuesday.OptionIndex];
		//MaxS = __ALGO_COUNT;

		but1 = pressed(&scalesw_state);
		but3 = pressed(&beatsw_state);
		but2 = pressed(&tpbsw_state);
	};
	break;

	case OPTION_SCALE:
	{
		butconf = pressed(&scalesw_state);
		butconflong = islongpress(&scalesw_state);
		S = Settings.scale[Tuesday.OptionIndex];
		//		MaxS = __SCALE_COUNT;

		but1 = pressed(&algosw_state);
		but3 = pressed(&beatsw_state);
		but2 = pressed(&tpbsw_state);

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
		Tuesday.switchmode = 1;
		SaveSettingsEeprom();

	}
	if (butconf == 1)
	{
		Tuesday.OptionIndex = (Tuesday.OptionIndex + 1) % 4;

		switch(Tuesday.OptionSelect)
		{
		case OPTION_ALGO:  Params.algo = Tuesday.OptionIndex; break;
		case OPTION_BEATS: Params.beatopt = Tuesday.OptionIndex; break;
		case OPTION_TPB:   Params.tpbopt = Tuesday.OptionIndex; break;
		case OPTION_SCALE: Params.scale = Tuesday.OptionIndex; break;
		}

		Tuesday.switchmode = 1;

	}
	if (butconflong == 1)
	{
		Tuesday.UIMode = UI_NORMAL;
	}


}

void UI_Calibration()
{
	if (pressed(&algosw_state))
	{
		Tuesday.UIMode = UI_NORMAL;
	}



	if (pressed(&beatsw_state))
	{
		Tuesday.CalibTarget = CALIBRATION_NOTE;
	}
	else
	{
		if (islongpress(&beatsw_state))
		{
			if (Tuesday.CalibTarget == CALIBRATION_NOTE)
			{
				// Write Velocity output calibration value!
				Tuesday.CalibTarget = CALIBRATION_NOTARGET;
			}
		}
	}

	if (pressed(&tpbsw_state))
	{
		Tuesday.CalibTarget = CALIBRATION_VEL;
	}
	else
	{
		if ( islongpress(&tpbsw_state ))
		{
			if (Tuesday.CalibTarget == CALIBRATION_VEL)
			{
				// Write Note output calibration value!
				Tuesday.CalibTarget = CALIBRATION_NOTARGET;
			}
		}
	}

}

void UI_Normal()
{

	if (pressed(&algosw_state))
	{
		Tuesday.switchmode = 1;
		Params.algo = (Params.algo + 1) % TUESDAY_MAXALGO;
		Tuesday.commitchange = 1;
	}
	else
	{
		if (islongpress(&algosw_state)) // longpress!
		{
			Params.algo = (Params.algo + TUESDAY_MAXALGO - 1) % TUESDAY_MAXALGO;
			Tuesday.commitchange = 1;
			SwitchToOptionMode(OPTION_ALGO, Params.algo);
		}
	}

	if (pressed(&scalesw_state))
	{
		Tuesday.switchmode = 1;
		Params.scale = (Params.scale + 1) % TUESDAY_MAXSCALE;
		Tuesday.commitchange = 1;
	}
	else
	{
		if (islongpress(&scalesw_state)) // longpress!
		{
			Params.scale = (Params.scale + TUESDAY_MAXSCALE - 1) % TUESDAY_MAXSCALE;
			Tuesday.commitchange = 1;
			SwitchToOptionMode(OPTION_SCALE, Params.scale);
		}
	}

	if (pressed(&beatsw_state))
	{
		Tuesday.switchmode = 1;
		Params.beatopt = (Params.beatopt + 1) % TUESDAY_MAXBEAT;

		Tuesday.commitchange = 1;
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
		Tuesday.switchmode = 1;

		Settings.beatoptions[Params.beatopt];

		Params.tpbopt = (Params.tpbopt + 1) % TUESDAY_MAXTPB;
		Tuesday.commitchange = 1;
	}
	else
	{
		if (islongpress(&tpbsw_state)) // longpress!
		{
			//Params.tpbopt = (Params.tpbopt + TUESDAY_MAXTPB - 1) % TUESDAY_MAXTPB;

			//SwitchToOptionMode(OPTION_TPB, Params.tpbopt);
		}
	}
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
	LoadEepromSettings();
	LoadEepromCalibration();
	TI1_Enable();
	AD1_Measure(FALSE);

#define G(a,b) (((a+1)%4)*4 + (b))

	unsigned char a[16] =
	{
			G(0,0),G(1,1),G(2,2),G(3,3),
			G(0,1),G(1,2),G(2,3),G(3,0),
			G(0,2),G(1,3),G(2,0),G(3,1),
			G(0,3),G(1,0),G(2,1),G(3,2)

	};
	for(int j =0 ;j<16;j++)
	{
		int idx = a[15-j];
		Tuesday.StateLedTargets[idx] = 255;
		ShiftOut();
		WAIT1_Waitms(70);
	}



	for (int i = 0;i < 16;i++)
	{
		Tuesday.StateLedTargets[i] = 0;
	}

	ShiftOut();



	if (islongpress( &scalesw_state))
	{
		Tuesday.UIMode = UI_CALIBRATION;
		Tuesday.CalibTarget =CALIBRATION_NOTARGET;
	}
	else
	{
		if (islongpress( &tpbsw_state))
		{
			FactoryReset();
		}

		Tuesday.UIMode = UI_NORMAL;
	}

	for (;;)
	{

		switch (Tuesday.UIMode)
		{

		case UI_SELECTOPTION: UI_SelectOption(); break;

		case UI_CALIBRATION: UI_Calibration(); break;

		case UI_NORMAL: UI_Normal(); break;


		};

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
		long newseed = (Tuesday.seed1) + (Tuesday.seed2 << 8);
		if (newseed != oldseed) Tuesday.switchmode = 1;

		if (Tuesday.switchmode == 1)
		{
			Tuesday.switchmode = 0;
			Tuesday_RandomSeed(&MainRandom, newseed);
			Tuesday_Generate(&Tuesday, &Params, &Settings);
			oldseed = newseed;
		}

		if (Tuesday.commitchange == 1 && tickssincecommit >= 10)
		{
			SaveEeprom();
			Tuesday.commitchange = 0;
			tickssincecommit = 0;
		}

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
