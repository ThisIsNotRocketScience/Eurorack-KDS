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
#include "LATCH.h"
#include "DATA.h"
#include "CLOCK.h"
#include "RETRIGGERINT.h"
#include "CI2C1.h"
#include "IntI2cLdd1.h"
#include "CLOCK2.h"
#include "DATA2.h"
#include "SYNCINT.h"
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

#include "Wobbler2.h"
#include "../../EurorackShared/EurorackShared.c"

#include "../../EurorackShared/Math.c"


Wobbler2_LFO_t LFO;
Wobbler2_Settings Settings;
Wobbler2_Params Params;


uint32_t t = 0;

//4096 = 2.048v
/*2.5 * (2.048 * INP)/4096
		(x * 4096)
		/ 2.5 * 2.048
		 = inp*/

#define VOLT(x) ((int)((4096.0 * (x)) / (2.5 * 2.048)))
#define NOTE(x) VOLT((x) / 12.0)


int pwm = 3;
int counter = 0;

#define SetIf(x){if (x) {DATA_SetVal(DATA_DeviceData);}else {DATA_ClrVal(DATA_DeviceData);}CLOCK_ClrVal(CLOCK_DeviceData); CLOCK_SetVal(CLOCK_DeviceData);};
#define SetNotIf(x){if (x) {DATA_ClrVal(DATA_DeviceData);}else {DATA_SetVal(DATA_DeviceData);}CLOCK_ClrVal(CLOCK_DeviceData); CLOCK_SetVal(CLOCK_DeviceData);};
#define SetIfdat2(x){if (x) {DATA2_SetVal(DATA2_DeviceData);}else {DATA2_ClrVal(DATA2_DeviceData);}CLOCK2_ClrVal(CLOCK2_DeviceData); CLOCK2_SetVal(CLOCK2_DeviceData);};


void ShiftOut()
{
	//return;
	counter++;
	pwm = (pwm + 8)&255;

	LATCH_ClrVal(LATCH_DeviceData);

	SetIf(LFO.Led[1][8] > pwm);
	SetIf(LFO.Led[1][7] > pwm);
	SetIf(LFO.Led[1][6] > pwm);
	SetIf(LFO.Led[1][5] > pwm);
	SetIf(LFO.Led[1][4] > pwm);
	SetIf(LFO.Led[1][3] > pwm);
	SetIf(LFO.Led[1][2] > pwm);
	SetIf(LFO.Led[1][1] > pwm);
	SetIf(LFO.Led[1][0] > pwm);

	SetIf(LFO.TriggerLed > pwm);
	SetNotIf(LFO.Gate[0]>0);
	SetNotIf(LFO.Gate[1]>0);

	unsigned int pwm2 = 10000;

	SetIf(LFO.Led[0][0] > pwm);

	SetIf(LFO.ModeLed[0] > pwm);
	SetIf(LFO.ModeLed[1] > pwm);
	SetIf(LFO.ModeLed[2] > pwm);

	SetIf(LFO.Led[0][1] > pwm);
	SetIf(LFO.Led[0][2] > pwm);
	SetIf(LFO.Led[0][3] > pwm);
	SetIf(LFO.Led[0][4] > pwm);
	SetIf(LFO.Led[0][5] > pwm);
	SetIf(LFO.Led[0][6] > pwm);
	SetIf(LFO.Led[0][7] > pwm);
	SetIf(LFO.Led[0][8] > pwm);

	SetIfdat2(0);
	SetIfdat2(0);
	SetIfdat2(LFO.ModeLed[4] > pwm);
	SetIfdat2(LFO.ModeLed[3] > pwm);
	SetIfdat2(LFO.ModeLed[2] > pwm);
	SetIfdat2(LFO.ModeLed[1] > pwm);
	SetIfdat2(LFO.ModeLed[0] > pwm);
	SetIfdat2(0);


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
int timermult =0 ;
void doTimer()
{
	timermult++;
	ShiftOut();
	if (timermult < 2) return;
	timermult = 0;
	tickssincecommit++;
	t++;
	switch(t%2)
	{
	case 0:
	{
		LinearOut = Wobbler2_Get(&LFO, &Params);
		LinearOut = (int)(LFO.Output);
		CurvedOut = (int)(LFO.OutputPhased);
		DAC_Write(1, CurvedOut);
	}
	break;
	case 1:
	{
		DAC_Write(0, LinearOut);
	}

	break;
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

		Wobbler2_ValidateParams(&Params);
	}
	else
	{
		SaveEeprom();
	}
}

void SyncIn(int sw)
{
	Wobbler2_SyncPulse(&LFO);
}
void EnvelopeTrigger(int sw)
{
	Wobbler2_Trigger(&LFO, sw, &Params);
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

	static struct denoise_state_t speedsw_state = {0};
	static struct denoise_state_t modesw_state = {0};
	static struct denoise_state_t triggersw_state = {0};
	Wobbler2_Init(&LFO);

#ifdef USE_SEMIHOST
	initialise_monitor_handles();
#endif

	Wobbler2_LoadSettings(&Settings, &Params);
	EuroRack_InitCalibration();

	LoadEeprom();

	AD1_Calibrate(TRUE);
	AD1_Start();
	AD1_Measure(FALSE);

	for(int i = 0;i<9;i++)
	{
		for(int j =0 ;j<9;j++)
		{
			if (i == j)
			{
				LFO.Led[0][j] = 255;
				LFO.Led[1][j] = 255;
			}
			else
			{
				LFO.Led[0][j] = 0;
				LFO.Led[1][j] = 0;
			}
		}

		ShiftOut();
		WAIT1_Waitms(20);
	}
	for(int i = 8;i>=0;i--)
	{
		for(int j =0 ;j<9;j++)
		{
			if (i == j)
			{
				LFO.Led[0][j] = 255;
				LFO.Led[1][j] = 255;
			}
			else
			{
				LFO.Led[0][j] = 0;
				LFO.Led[1][j] = 0;
			}
		}

		ShiftOut();
		WAIT1_Waitms(20);
	}

	for(int i = 0;i<5;i++)
	{
		for(int j =0 ;j<5;j++)
		{
			if (i == j) LFO.ModeLed[j] = 255;
			else LFO.ModeLed[j] = 0;
		}

		ShiftOut();
		WAIT1_Waitms(20);
	}

	for(int i = 4;i>=0;i--)
	{
		for(int j =0 ;j<5;j++)
		{
			if (i == j) LFO.ModeLed[j] = 255;
			else LFO.ModeLed[j] = 0;
		}

		ShiftOut();
		WAIT1_Waitms(20);
	}

	ShiftOut();
	TI1_Enable();

	int switchmode = 1;


	ShiftOut();

	byte commitchange = 0;
	for (;;)
	{


		if (measured == 1)
		{
			LFO.Mod = ~adcchannels[ADC_MODULATION];
			LFO.Shape = ~adcchannels[ADC_SHAPE] ;
			LFO.Phasing = (adcchannels[ADC_PHASING])>>4 ;
			LFO.Speed = ((0xffff-adcchannels[ADC_SPEED]) >> 7);
			LFO.Amount1 = ((adcchannels[ADC_AMTNORMAL])>>1 ) -(1<<14);
			LFO.Amount2 = ((adcchannels[ADC_AMTPHASED])>>1 )-(1<<14) ;


			measured = 0;

			Wobbler2_UpdatePendulumSettings(&LFO.Pendulum, &LFO);

			AD1_Measure(FALSE);

		}


		if (commitchange == 1 && tickssincecommit >= 10)
		{
			//	SaveEeprom();
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
