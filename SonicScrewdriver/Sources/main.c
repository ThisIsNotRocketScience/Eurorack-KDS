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
#include "SW1.h"
#include "SW2.h"
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

struct PatternGen_Target Pattern;

struct denoise_state_t
{
	int counter;
	int down;
	int pressed;
	int released;
	int lastcounter;
};


int denoise(int sw_down, struct denoise_state_t* state)
{
	if (sw_down) state->counter++;
	else state->counter--;
	state->pressed = 0;
	state->released = 0;

	if (state->counter < 2)
	{
		if (state->lastcounter == 2)
		{
			state->pressed =1;
		}
		state->counter = 1;
		state->down = 1;

	}
	else if (state->counter > 30)
	{
		if (state->lastcounter == 30)
		{
			state->released =1;
		}
		state->counter = 31;
		state->down = 0;

	}
	state->lastcounter = state->counter;
	return state->down;
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

uint32_t t =0 ;

#define VOLT(x) ((int)((4095.0*(x))/6.08))
#define NOTE(x) VOLT((x)/12.0)


int countdownTick = 1;
int countdownNote = 1;
word TickOut = 0;
word CVOut = 0;
int Tick = -1;
long oldseed= -1;
int beatpwm = 0;
byte pwm = 0;


// half-millisecond timer -> update each dacchannel in turn
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
		int bpm = 1 + (200 * adcchannels[3])/ 65536;
		int msecperbeat = (1000*60)/(Pattern.TPB*bpm);

		countdownTick--;
		if (countdownTick > msecperbeat)  countdownTick = msecperbeat;
		if (countdownTick <= 0)
		{
			countdownTick = msecperbeat;

			if (Pattern.Ticks[Tick].vel >= (1.0 - (adcchannels[5] / 65535.0)) )
			{
				countdownNote = (countdownTick * 900 ) / 1000;
				//1 +  (countdownTick * adcchannels[5])/67000;

				if (countdownNote >= countdownTick ) countdownNote = 0;

				TickOut = 4095-(Pattern.Ticks[Tick].accent*2048 + 2047);
				CVOut = 4095 - NOTE(Pattern.Ticks[Tick].note+24);

			}

			beatpwm = (Tick%Pattern.Length==0)?255:((Tick%Pattern.TPB==0)?65:0);

			Tick = (Tick + 1) % Pattern.Length;

		}
		WriteDac(0, CVOut);
	}
	else
	{
		WriteDac(1, TickOut);


	}

	pwm+=16;
	if (pwm < beatpwm) SW2LED_ClrVal(0); else SW2LED_SetVal(0);

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

	static struct denoise_state_t triggersw_state = {0};
	static struct denoise_state_t sonicsw_state = {0};
	static struct denoise_state_t gatesw_state = {0};
	int patternmode = 3;

	AD1_Measure(FALSE);
	PatternGen_ZRANDOMSEED(oldseed);
	PatternGen_Goa(&Pattern,16);
	int switchmode = 1;
	SW2LED_ClrVal(0);
	LED1_SetVal(0);

	for(;;){
		int sonicsw= denoise(SW2_GetVal(0), &sonicsw_state);
		//int gatesw = denoise(GATE_GetVal(0), &gatesw_state);
		//int triggersw = denoise(SW2_GetVal(0), &triggersw_state);


		if (sonicsw_state.pressed == 1)
		{
			switchmode=1;
			patternmode = (patternmode+ 1) % 4;
		}

		if (measured == 1)
		{
			measured  = 0;
			AD1_Measure(FALSE);
		}

		// read the X/Y seed knobs
		long newseed= (adcchannels[2]>>11) + (adcchannels[4]>>11)<<16;
		if (newseed!= oldseed) switchmode = 1;

		if (switchmode == 1){
			// updated pattern needed for some reason!
			switchmode = 0;
			PatternGen_ZRANDOMSEED(newseed);
			oldseed = newseed;


			switch(patternmode)
			{
			case 0:
				PatternGen_Goa(&Pattern,16);
				LED4_SetVal(0);
				LED2_ClrVal(0);
				LED3_ClrVal(0);
				break;
			case 1:
				PatternGen_Flat(&Pattern,16);
				LED4_ClrVal(0);
				LED2_SetVal(0);
				LED3_ClrVal(0);

				break;
			case 2:
				PatternGen_Psych(&Pattern,16);
				LED4_ClrVal(0);
				LED2_ClrVal(0);
				LED3_SetVal(0);

				break;

			case 3:
							PatternGen_Zeph(&Pattern, 4, 4, 4);
							LED4_SetVal(0);
							LED2_SetVal(0);
							LED3_SetVal(0);

							break;
			}
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
 **     This file was created by Processor Expert 10.4 [05.10]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */
