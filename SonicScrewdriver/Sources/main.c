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
#include "GATE.h"
#include "EInt1.h"
#include "SM1.h"
#include "TI1.h"
#include "TimerIntLdd1.h"
#include "TU1.h"
#include "LED2.h"
#include "LED1.h"
#include "LED3.h"
#include "LED4.h"
#include "SW2LED.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */

#include <math.h>
int shutdown1 = 0;
int gain1 = 0;
int shutdown2 = 0	;
int gain2 = 0;
int senddone = 1;
int measured = 0;
int adcchannels[6];
unsigned char coms[4];

#include "Saiko.h"

struct PatternTarget Pattern;

void WriteBoth(int A, int B)
{
	while (!senddone){}
	DACSEL_ClrVal(0);

	unsigned int  command = 0x0000;
	command |= shutdown1 ? 0x0000 : 0x1000;
	command |= gain1 ? 0x0000 : 0x2000;
	command |= (A & 0x0FFF);
	coms[0] =  command >> 8;
	coms[1] = command &0xff;
	senddone = 0;

	//SM1_SendBlock(SM1_DeviceData, coms, 2 );

	//while (!senddone){}


	command = 0x8000;
	command |= shutdown2 ? 0x0000 : 0x1000;
	command |= gain2 ? 0x0000 : 0x2000;
	command |= (B & 0x0FFF);
	coms[2] =  command >> 8;
	coms[3] = command &0xff;
	senddone = 0;
	SM1_SendBlock(SM1_DeviceData, coms, 4 );

	//	while (!senddone){}

}
void WriteDac(int channel, int mv)
{
	while (!senddone)
	{
		return;
	}
	DACSEL_ClrVal(0);

	unsigned int command;
	if(channel == 1)
	{
		command = 0x0000;
		command |= shutdown1 ? 0x0000 : 0x1000;
		command |= gain1 ? 0x0000 : 0x2000;
		command |= (mv & 0x0FFF);
		coms[0] =  command >> 8;
		coms[1] = command &0xff;
		senddone = 0;

		SM1_SendBlock(SM1_DeviceData, coms, 2 );


	} else {
		command = 0x8000;
		command |= shutdown2 ? 0x0000 : 0x1000;
		command |= gain2 ? 0x0000 : 0x2000;
		command |= (mv & 0x0FFF);
		coms[0] =  command >> 8;
		coms[1] = command &0xff;
		senddone = 0;
		SM1_SendBlock(SM1_DeviceData, coms, 2 );

	}
}

word t1 = 0;
word t2 = 0;
float T = 0;
uint32_t t =0 ;
uint32_t tt =0 ;
unsigned char pwm = 0;
#define VOLT(x) ((int)((4095.0*(x))/6.08))
#define NOTE(x) VOLT((x)/12.0)

word vals[8] = {0,VOLT(1),VOLT(2),VOLT(3),VOLT(2.5),VOLT(1.5),VOLT(0.5),4095};

word major[8] = {NOTE(0),NOTE(4),NOTE(7),NOTE(0),NOTE(4),NOTE(7),NOTE(0),NOTE(7)};

int countdownTick = 1;
int countdownNote = 1;
word TickOut = 0;
word CVOut = 0;
int Tick = -1;
long oldseed= 0;

void doTimer()
{
	t++;

	if (t %2 == 0)
	{
		if (countdownNote >= 0)

		{
			countdownNote--;
			if (countdownNote <= 0)
			{
				TickOut = 4095;
			}
		}

		countdownTick--;
		if (countdownTick <= 0)
		{
			countdownTick = 4 + adcchannels[3]/64;
			countdownNote = 1 +  (countdownTick * adcchannels[5])/67000;
			if (countdownNote >= countdownTick ) countdownNote = 0;
			TickOut = 4095-(Pattern.Ticks[Tick].accent*2048 + 2047);
			Tick = (Tick + 1) % 16;

			CVOut = 4095 - NOTE(Pattern.Ticks[Tick].note+24);
		}
		WriteDac(0, CVOut);
	}
	else
	{
		WriteDac(1, TickOut);
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
	AD1_Measure(FALSE);
	szrand(oldseed);
	PatternGen_Goa(&Pattern);

	/* For example: for(;;) { } */
	for(;;){

		if (measured == 1)
		{
			measured  = 0;
			AD1_Measure(FALSE);
		}
		pwm ++;
		long newseed= (adcchannels[2]>>11) + (adcchannels[4]>>11)<<16;
		if (newseed!= oldseed)
		{
			oldseed = newseed;
			szrand(oldseed);
			PatternGen_Goa(&Pattern);

		}
		if (adcchannels[2]/128 > pwm ) LED1_ClrVal(0);else LED1_SetVal(0);
		if (adcchannels[3]/128 > pwm ) LED2_ClrVal(0);else LED2_SetVal(0);
		if (adcchannels[4]/128 > pwm ) LED3_ClrVal(0);else LED3_SetVal(0);
		if (adcchannels[5]/128 > pwm ) LED4_ClrVal(0);else LED4_SetVal(0);
		if (adcchannels[0]/32 > pwm ) SW2LED_SetVal(0);else SW2LED_ClrVal(0);


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
 **     This file was created by Processor Expert 10.4 [05.10]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */
