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
int measured = 0;
int adcchannels[6];

#define GATE_TICK 4
#define GATE_CLOCK 5
#define GATE_BEAT 3
#define GATE_LOOP 2
#define GATE_ACCENT 1
#define GATE_GATE 0

#include "PatternGen.h"
#include "DAC.h"

struct PatternGen_Target Pattern;
struct PatternGen_Settings Settings;
struct PatternGen_Params Params;
struct PatternGen_Random MainRandom;

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
	return state->pressed ;
}


uint32_t t =0 ;

//4096 = 2.048v

/*2.5 * (2.048 * INP)/4096

		(x * 4096)
		/ 2.5 * 2.048
		 = inp*/


#define VOLT(x) ((int)((4096.0*(x))/(2.5 * 2.048)))
#define NOTE(x) VOLT((x)/12.0)


int countdownTick = 1;
int countdownNote = 1;
word TickOut = 0;
word CVOut = 0;
int Tick = -1;
long oldseed= -1;
byte pwm = 0;

int timesincelastclocktick;
int clocktick = 0;


byte leds[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
byte gates[6] = {0,0,0,0,0,0};

void ShiftOut()
{
	pwm+=16;

	LATCH_ClrVal(LATCH_DeviceData);
	for(int i =0 ;i< 6;i++)
	{
		if (gates[i] >  0) DATA_ClrVal(DATA_DeviceData);else DATA_SetVal(DATA_DeviceData);
		CLOCK_ClrVal(CLOCK_DeviceData);
		CLOCK_SetVal(CLOCK_DeviceData);
	}

	for(int i =0 ;i< 16;i++)
	{
		if (leds[i] > pwm)
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
	if (Pattern.Ticks[Tick].vel >= (255 - ((65535-adcchannels[2]) / 256.0)) )
	{
		countdownNote = (countdownTick * 900 ) / 1000;

		if (countdownNote >= countdownTick ) countdownNote = 0;

		TickOut = (Pattern.Ticks[Tick].accent*2048 + 2047);
		CVOut = NOTE(Pattern.Ticks[Tick].note+24);
		gates[GATE_GATE] = 1;
		if (Pattern.Ticks[Tick].accent > 0) gates[GATE_ACCENT] = 1;
	}


	if (Tick == 0) gates[GATE_LOOP] = 1;
	if (Tick%Pattern.TPB==0) gates[GATE_BEAT] = 1;

	Tick = (Tick + 1) % Pattern.Length;

	gates[GATE_TICK] = 1;
}

void clearTick()
{
	gates[GATE_BEAT] = 0;
	gates[GATE_TICK] = 0;
	gates[GATE_GATE] = 0;
	gates[GATE_ACCENT] = 0;
	gates[GATE_LOOP] = 0;
}

int directtick  =0;
void PatternReset()
{
	clearTick();
	TickOut = 0;
	Tick = 0 ;
	countdownTick = 0;
	directtick = 1;
}
int clockup = 0;
int clockshad =0 ;


void DoClock(int state)
{
	if (state == 1 && timesincelastclocktick > 2) // slight deadzone debounce
	{
		timesincelastclocktick = 0;
		clockup = 1;
		clockshad ++;
		if (clockshad >= 96 / (Pattern.TPB*4) || directtick ==1) {
			doTick();
			directtick =0 ;
			clockshad =0 ;
		}
	}
	else
	{
		clearTick();
		clockup = 0;
	}
}



// half-millisecond timer -> update each dacchannel in turn
void doTimer()
{

	timesincelastclocktick++;
	int clockmode = 1;
	if (clockup == 0 && timesincelastclocktick > 2000)
	{
		timesincelastclocktick = 3000;
		clockmode = 0;
	}

	t++;

	if (t %2 == 0)
	{
		if (countdownNote >= 0)
		{
			countdownNote--;
			if (countdownNote <= 0)
			{
				TickOut = 0;
				gates[5] = 0;
			}
		}


		int bpm = 1 + (200 * (65535-adcchannels[3]))/ 65536;
		int msecperbeat = (1000*60)/(Pattern.TPB*bpm);



		if (clockmode == 0)
		{
			countdownTick--;

			if (countdownTick > msecperbeat)  countdownTick = msecperbeat;
			if (countdownTick <= 0 || directtick == 1)
			{
				directtick = 0;
				countdownTick = msecperbeat;
				doTick();
			}
			else
			{
				clearTick();
			}
		}
		DAC_Write(0, CVOut);
	}
	else
	{
		DAC_Write(1, TickOut);
	}

	ShiftOut();
}

void SetLedNumber(int offset, int number)
{
	switch(number %13)
	{
	case 0:
		leds[offset + 0] = 255;
		leds[offset + 1] = 0;
		leds[offset + 2] = 0;
		leds[offset + 3] = 0;
		break;
	case 1:
		leds[offset + 0] = 0;
		leds[offset + 1] = 255;
		leds[offset + 2] = 0;
		leds[offset + 3] = 0;
		break;
	case 2:
		leds[offset + 0] = 0;
		leds[offset + 1] = 0;
		leds[offset + 2] = 255;
		leds[offset + 3] = 0;
		break;
	case 3:
		leds[offset + 0] = 0;
		leds[offset + 1] = 0;
		leds[offset + 2] = 0;
		leds[offset + 3] = 255;
		break;
	case 4:
		leds[offset + 0] = 255;
		leds[offset + 1] = 0;
		leds[offset + 2] = 0;
		leds[offset + 3] = 255;
		break;
	case 5:
		leds[offset + 0] = 255;
		leds[offset + 1] = 255;
		leds[offset + 2] = 0;
		leds[offset + 3] = 0;
		break;
	case 6:
		leds[offset + 0] = 0;
		leds[offset + 1] = 255;
		leds[offset + 2] = 255;
		leds[offset + 3] = 0;
		break;
	case 7:
		leds[offset + 0] = 0;
		leds[offset + 1] = 0;
		leds[offset + 2] = 255;
		leds[offset + 3] = 255;
		break;
	case 8:
		leds[offset + 0] = 255;
		leds[offset + 1] = 0;
		leds[offset + 2] = 255;
		leds[offset + 3] = 255;
		break;
	case 9:
		leds[offset + 0] = 255;
		leds[offset + 1] = 255;
		leds[offset + 2] = 0;
		leds[offset + 3] = 255;
		break;
	case 10:
		leds[offset + 0] = 255;
		leds[offset + 1] = 255;
		leds[offset + 2] = 255;
		leds[offset + 3] = 0;
		break;
	case 11:
		leds[offset + 0] = 0;
		leds[offset + 1] = 255;
		leds[offset + 2] = 255;
		leds[offset + 3] = 255;
		break;
	case 12:
		leds[offset + 0] = 255;
		leds[offset + 1] = 255;
		leds[offset + 2] = 255;
		leds[offset + 3] = 255;
		break;
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

	static struct denoise_state_t algosw_state = {0};
	static struct denoise_state_t scalesw_state = {0};
	static struct denoise_state_t beatsw_state = {0};
	static struct denoise_state_t tpbsw_state = {0};

	PatternGen_LoadSettings(&Settings, &Params);
	PatternGen_RandomSeed(&MainRandom, oldseed);
	AD1_Measure(FALSE);
	int switchmode = 1;
	SetLedNumber(8,Params.scale );
	SetLedNumber(12,Params.algo );
	SetLedNumber(4,Params.beatopt);
	SetLedNumber(0,Params.tpbopt);

	for(;;){

		int algosw= denoise(SW_ALGO_GetVal(0), &algosw_state);
		int scalesw = denoise(SW_SCALE_GetVal(0), &scalesw_state);
		int beatsw = denoise(SW_BEATS_GetVal(0), &beatsw_state);
		int tpbsw = denoise(SW_TPB_GetVal(0), &tpbsw_state);

		if (algosw == 1)
		{
			switchmode = 1;
			Params.algo = (Params.algo + 1) % PATTERNGEN_MAXALGO;
			SetLedNumber(12,Params.algo );
		}

		if (scalesw == 1)
		{
			switchmode = 1;
			Params.scale = (Params.scale + 1) % PATTERNGEN_MAXSCALE;
			SetLedNumber(8,Params.scale);
		}


		if (beatsw == 1)
		{
			switchmode = 1;
			Params.beatopt = (Params.beatopt + 1) % PATTERNGEN_MAXBEAT;
			SetLedNumber(4,Params.beatopt);
		}


		if (tpbsw == 1)
		{
			switchmode = 1;
			Params.tpbopt = (Params.tpbopt + 1) % PATTERNGEN_MAXTPB;
			SetLedNumber(0,Params.tpbopt);
		}


		if (measured == 1)
		{
			measured  = 0;
			AD1_Measure(FALSE);
		}

		// read the X/Y seed knobs
		long newseed= (adcchannels[0]>>8) + ((adcchannels[1]>>8)<<8);
		if (newseed!= oldseed) switchmode = 1;

		if (switchmode == 1)
		{
			// updated pattern needed for some reason!

			switchmode = 0;
			PatternGen_RandomSeed(&MainRandom,newseed);
			oldseed = newseed;


			Params.seed1 = (adcchannels[0]>>8);
			Params.seed2 = (adcchannels[1]>>8);

			PatternGen_Generate(&Pattern,&Params, &Settings);


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
