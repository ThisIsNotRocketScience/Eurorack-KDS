/* ###################################################################
 **     Filename    : main.c
 **     Project     : BigFish
 **     Processor   : MK22FN256VLH12
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2017-05-24, 01:44, # CodeGen: 0
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
#include "Pins1.h"
#include "CI2C1.h"
#include "ADMUXED.h"
#include "AD1.h"
#include "LATCH.h"
#include "CLOCK.h"
#include "SHIFTERS.h"
#include "ADCSELA.h"
#include "ADCSELB.h"
#include "ADCSELC.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"
/* User includes (#include below this line is not maintained by Processor Expert) */

#include "LEDS.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define __max(a,b) (((a)>(b))?(a):(b))
#include "../../BigFishLib/BigFish.cpp"
#include "../../BigFishLib/ADSREnvelope.cpp"
#include "../../BigFishLib/BleppyOscs.cpp"
#include "../../BigFishLib/MinBlepGen.cpp"
#include "../../BigFishLib/OrganOsc.cpp"

//BigFish_t Fish;
extern "C"
{
int cppmain(void)
{
	LEDS_InitHardware();
	uint8_t LEDS[16];

	//BigFish_Init(&Fish, 44100);
	for(int i =0 ;i<16;i++)
	{
		LEDS[i] = 0;
	}
	uint32_t T = 0;
	for(;;)
	{
		LEDS_Update(LEDS, 16, 0, 0);

		for(int i =0 ;i<16;i++)
		{
			LEDS[i] = 0;
			if (((T/5000)%8) == i%8) LEDS[i] = 255;
		}
		T++;


	}
}

}
