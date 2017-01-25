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
#include "SW_MODE.h"
#include "LATCH.h"
#include "DATA.h"
#include "CLOCK.h"
#include "SW_TRIGGER.h"
#include "SW_SPEED.h"
#include "RETRIGGERINT.h"
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


#include "EdgeCutter.h"
#include "../../EurorackShared/EurorackShared.c"

struct EdgeCutter_Envelope Envelope;
struct EdgeCutter_Settings Settings;
struct EdgeCutter_Params Params;

static struct denoise_state_t speedsw_state = {0};
static struct denoise_state_t modesw_state = {0};
static struct denoise_state_t triggersw_state = {0};

void UpdateButtons()
{
	denoise(SW_TRIGGER_GetVal(0), &triggersw_state);
	 denoise(SW_MODE_GetVal(0), &modesw_state);
	denoise(SW_SPEED_GetVal(0), &speedsw_state);

}
uint32_t t = 0;

//4096 = 2.048v
/*2.5 * (2.048 * INP)/4096
		(x * 4096)
		/ 2.5 * 2.048
		 = inp*/

#define VOLT(x) ((int)((4096.0 * (x)) / (2.5 * 2.048)))
#define NOTE(x) VOLT((x) / 12.0)

byte outleds[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0};
byte targetleds[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0};
//                    13                 5, 6, 7, 8, 9,10, 4, 3,2,1,0

byte ledorder[13] = {19,18,17,16,15, 8,9, 10,11,12,13,14, 2};
unsigned char pwm = 3;
//int counter = 0;

void ShiftOut()
{
	//counter++;
	pwm += 9;

	LATCH_ClrVal(LATCH_DeviceData);




	for (int i = 0; i < 20; i++)
	{
		if (outleds[i] > pwm)
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
	for (int i = 0; i < 4; i++)
	{
		if (Envelope.Gates[3-i] > 0)
			DATA_ClrVal(DATA_DeviceData);
		else
			DATA_SetVal(DATA_DeviceData);
		CLOCK_ClrVal(CLOCK_DeviceData);
		CLOCK_SetVal(CLOCK_DeviceData);
	}
	LATCH_SetVal(LATCH_DeviceData);
}



int KnobOpt(int val)
{
	int r = 0;
	if (val > (65536*1)/5) r++;
	if (val > (65536*2)/5) r++;
	if (val > (65536*3)/5) r++;
	if (val > (65536*4)/5) r++;
	return 1 + 4 - r;
}


int tickssincecommit = 0;

// half-millisecond timer -> update each dacchannel in turn
int LinearOut = 0;
int CurvedOut = 0;

void doTimer()
{
	UpdateButtons();
	tickssincecommit++;
	t++;
	switch(t%2)
	{
		case 0:
		{
			EdgeCutter_GetEnv(&Envelope, &Params);
			LinearOut = Envelope.LinearOutput;
			DAC_Write(0, LinearOut);

			for(int i =0 ;i<13;i++)
				{
					targetleds[ledorder[i]] = Envelope.StateLeds[i];
					if (outleds[ledorder[i]] < targetleds[ledorder[i]]) outleds[ledorder[i]] = Envelope.StateLeds[i];;

				}
				for (int i = 0; i < 20; i++)
					{
						if (targetleds[i] > outleds[i])
						{
							outleds[i]++;
						}
						else
						{
							if (targetleds[i] < outleds[i])
							{
								outleds[i]--;
							}

						}
					}

		}
		break;
		case 1:
		{
			CurvedOut = Envelope.CurvedOutput;
			DAC_Write(1, CurvedOut);
		}

		break;
	}
	ShiftOut();
}

void SetModeLeds(int mode)
{
	switch(mode)
	{
	case 0: targetleds[5]=0; targetleds[6] = 0;targetleds[7] =255 ;break;
	case 1: targetleds[5]=0; targetleds[6] = 255;targetleds[7] =0 ;break;
	case 2: targetleds[5]=255; targetleds[6] = 0;targetleds[7] =0 ;break;
	}
}

void SetSpeedLeds(int speed)
{
	switch(speed)
	{
	case 0: targetleds[3]=255; targetleds[4] = 0;break;
	case 1: targetleds[3]=0; targetleds[4] = 255;break;

	}
}

#define VERSIONBYTE 0x10

void SaveEeprom()
{
	EE24_WriteByte(0, VERSIONBYTE);
	int paramsize = sizeof(Params);
	EE24_WriteBlock(1, (byte *)&Params, paramsize);
}

void LoadEeprom()
{
	byte Ver;
	Ver = EE24_ReadByte(0);
	if (Ver == VERSIONBYTE)
	{
		int paramsize = sizeof(Params);
		EE24_ReadBlock(1, (byte *)&Params, paramsize);

		EdgeCutter_ValidateParams(&Params);
	}
	else
	{
		SaveEeprom();
	}
}

void SetupLeds()
{
	SetSpeedLeds(Params.speed);
	SetModeLeds(Params.mode);
}

void EnvelopeTrigger(int sw)
{
	EdgeCutter_Trigger(&Envelope, sw, &Params);
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


	EdgeCutter_Init(&Envelope);

#ifdef USE_SEMIHOST
	initialise_monitor_handles();
#endif

	EdgeCutter_LoadSettings(&Settings, &Params);
	LoadEeprom();

	TI1_Enable();
	AD1_Calibrate(TRUE);
	AD1_Measure(FALSE);

	ShiftOut();
	int switchmode = 1;
	SetupLeds();
	ShiftOut();

	byte commitchange = 0;
	for (;;)
	{


		if (pressed(&modesw_state))
		{
			switchmode = 1;
			Params.mode = (Params.mode + 1) % EDGECUTTER_MAXMODE;
			commitchange = 1;
		}

		if (pressed(&speedsw_state))
		{
			switchmode = 1;
			Params.speed = (Params.speed + 1) % EDGECUTTER_MAXSPEED;
			commitchange = 1;
		}

		if (pressed(&triggersw_state) )
		{
			EdgeCutter_Trigger(&Envelope, 1, &Params);
		}
		if (released(&triggersw_state) )
		{
			EdgeCutter_Trigger(&Envelope, 0 ,&Params);
		}



		if (measured == 1)
		{
			measured = 0;
			AD1_Measure(FALSE);
		}

		Envelope.A = ~(adcchannels[ADC_A] >> 8);
		Envelope.D = ~(adcchannels[ADC_D] >> 8);
		Envelope.S = ~(adcchannels[ADC_S] >> 8);
		Envelope.R = ~(adcchannels[ADC_R] >> 8);
		Envelope.Curvature = ~(adcchannels[ADC_CURVATURE] >> 8);

		if (switchmode == 1)
		{
			SetupLeds();

		}

		if (commitchange == 1 && tickssincecommit >= 10)
		{
			SaveEeprom();
			commitchange = 0;
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
