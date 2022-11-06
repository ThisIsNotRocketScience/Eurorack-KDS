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
#include "IntI2cLdd1.h"
#include "PTB.h"
#include "KSDK1.h"
#include "EE241.h"
#include "GI2C1.h"
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
extern struct Tuesday_RandomGen MainRandom;
extern int oldseed;
extern int tickssincecommit;

byte pwm = 0;

void SetupLeds();

extern struct denoise_state_t algosw_state ;
extern struct denoise_state_t scalesw_state ;
extern struct denoise_state_t beatsw_state ;
extern struct denoise_state_t tpbsw_state ;

void UpdateButtons()
{
	denoise(SW_ALGO_GetVal(0), &algosw_state);
	denoise(SW_SCALE_GetVal(0), &scalesw_state);
	denoise(SW_BEATS_GetVal(0), &beatsw_state);
	denoise(SW_TPB_GetVal(0), &tpbsw_state);
}


void __attribute__((noinline)) ShiftOut()
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
	Tuesday_Tick(&Tuesday, &Params);
	ShiftOut();
}

void PatternReset(int Val)
{
	Tuesday_Reset(&Tuesday,&Settings, Val);
}

void ExtClockTick(int state)
{
	Tuesday_ExtClock(&Tuesday, &Params,&Settings, state);
}

void DoClock(int state)
{
	Tuesday_Clock(&Tuesday, &Settings,&Params, state);
}

// half-millisecond timer -> update each dacchannel in turn
void doTimer()
{
	tickssincecommit++;
	UpdateButtons();

	switch (Tuesday.UIMode)
	{
	case UI_NORMAL:
	case UI_GLOBALSETTINGS:
	case UI_SELECTOPTION:
	{

		Tuesday_TimerTick(&Tuesday, &Params);

		if (Tuesday.T % 2 == 0)
		{

			DAC_Write(0, Tuesday.CVOut >> 16);
		}
		else
		{
			DAC_Write(1, Tuesday.TickOut);
		}

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
			if (Tuesday.intensity > 128 + 28)
			{
				CalibPattern = DAC_NOTE(12 * 3, chan);
			}
			else
			{
				CalibPattern = ((Tuesday.T >> 4) & 0x01) ? DAC_NOTE(12 * 1, chan) : DAC_NOTE(12 * 3, chan);
			}
		}

		uint32_t CalibNote = 0;
		uint32_t CalibVel = 0;
		Tuesday.T++;

		switch (Tuesday.CalibTarget)
		{
		case CALIBRATION_NOTE: CalibNote = CalibPattern; CalibVel = 0; break;
		case CALIBRATION_VEL: CalibNote = 0; CalibVel = CalibPattern; break;
		}

		if (Tuesday.T % 2 == 0)
		{
			DAC_Write(0, CalibNote);
		}
		else
		{
			DAC_Write(1, CalibVel);
		}
	}
	break;
	}

	if (Tuesday.T % 2 == 0)
	{
		Tuesday_SetupLeds(&Tuesday,&Settings, &Params);
	}

	ShiftOut();
}

void SaveEeprom()
{
	int paramsize = sizeof(Params);
	EE24_WriteByte(EEPROM_OPTIONBASE, VERSIONBYTE);
	EE24_WriteBlock(EEPROM_OPTIONBASE + 1, (byte *)&Params, paramsize);
}

void SaveSettingsEeprom()
{
	int settingssize = sizeof(Settings);
	EE24_WriteByte(EEPROM_SETTINGSBASE, VERSIONBYTE);
	EE24_WriteBlock(EEPROM_SETTINGSBASE + 1, (byte *)&Settings, settingssize);
}

void SaveCalibrationEeprom()
{
	int calibrationsize = sizeof(MasterCalibration);
	EE24_WriteByte(EEPROM_CALIBRATIONBASE, CALIBRATIONVERSIONBYTE);
	EE24_WriteBlock(EEPROM_CALIBRATIONBASE + 1, (byte *)&MasterCalibration, calibrationsize);
}

void LoadEepromSettings()
{
	byte Ver;
	Ver = EE24_ReadByte(EEPROM_SETTINGSBASE);
	if (Ver == VERSIONBYTE)
	{
		int settingssize = sizeof(Settings);

		EE24_ReadBlock(EEPROM_SETTINGSBASE + 1, (byte *)&Settings, settingssize);
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
	if (Ver == CALIBRATIONVERSIONBYTE)
	{
		int calibrationsize = sizeof(MasterCalibration);
		EE24_ReadBlock(EEPROM_CALIBRATIONBASE + 1, (byte *)&MasterCalibration, calibrationsize);
		if (EuroRack_ValidateCalibration() > 0)
		{
			SaveCalibrationEeprom();
		}
	}
	else
	{
		SaveCalibrationEeprom();
	}
}

void LoadEeprom()
{
	byte Ver;
	Ver = EE24_ReadByte(EEPROM_OPTIONBASE);
	if (Ver == VERSIONBYTE)
	{
		int paramsize = sizeof(Params);
		EE24_ReadBlock(EEPROM_OPTIONBASE + 1, (byte *)&Params, paramsize);
		Tuesday_ValidateParams(&Params);
	}
	else
	{
		SaveEeprom();
	}
}




// reset basic options, but not calibration
void FactoryReset(int all)
{
	Tuesday_LoadDefaults(&Settings, &Params);
	SaveSettingsEeprom();
	if (all == 1)
	{
		EuroRack_InitCalibration();
		SaveCalibrationEeprom();
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

	Tuesday_SetupClockSubdivision(&Tuesday, &Settings, &Params);
	Tuesday_Generate(&Tuesday, &Params, &Settings);

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
	for (int j = 0; j < 16; j++)
	{
		int idx = a[15 - j];
		Tuesday.StateLedTargets[idx] = 255;
		ShiftOut();
		WAIT1_Waitms(70);
	}



	for (int i = 0; i < 16; i++)
	{
		Tuesday.StateLedTargets[i] = 0;
	}

	ShiftOut();


	Tuesday.UIMode = UI_NORMAL;

	if (islongpress(&scalesw_state))
	{
		Tuesday.UIMode = UI_CALIBRATION;
		Tuesday.CalibTarget = CALIBRATION_NOTARGET;
	}
	else
	{

		//		if (islongpress( &tpbsw_state))
			//	{
				//	Tuesday.UIMode = UI_GLOBALSETTINGS;
			//	}
				//else
			//	{
		if (islongpress(&beatsw_state))
		{
			FactoryReset(1);
		}

		Tuesday.UIMode = UI_NORMAL;
		//	}
	}

	Tuesday_InitScales(&Settings);

	for (;;)
	{


		if (measured == 1)
		{
			measured = 0;
			Tuesday.seed1 = (adcchannels[ADC_INX] >> 8);
			Tuesday.seed2 = (adcchannels[ADC_INY] >> 8);
			Tuesday.intensity = (adcchannels[ADC_INTENSITY] >> 8);
			Tuesday.tempo = (adcchannels[ADC_TEMPO] >> 8);
			AD1_Measure(FALSE);
		}

		Tuesday_MainLoop(&Tuesday,&Settings, &Params);


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
