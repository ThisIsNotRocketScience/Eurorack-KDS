/* ###################################################################
 **     Filename    : main.c
 **     Project     : AudioBoot
 **     Processor   : MKL02Z32VFM4
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2016-11-12, 18:36, # CodeGen: 0
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
#include "TI1.h"
#include "TimerIntLdd1.h"
#include "TU1.h"
#include "IFsh1.h"
#include "IntFlashLdd1.h"
#include "PTA.h"
#include "PTB.h"
#include "CI2C1.h"
#include "WAIT1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "SharedBoot/Boot.h"
#include "SharedBoot/Reader.h"

#include "../../EurorackShared/LEDS.c"
#include "../../EurorackShared/Eeprom.c"
#include "../../EurorackShared/DAC_BitBang.c"


extern AudioReaderStruct Reader;
byte theprogress =0 ;
byte error = 0;
byte started = 0;
void  GUIProgress(uint8_t progress)
{
	theprogress = progress;
} // 255 = 99.999%

void  GUIReset()
{
	theprogress =0;
	error =0 ;
	started = 1;
} // 255 = 99.999%

void GUIErrorState()
{
	error++;
}

extern void DecoderInit();

int History[64];
int historypos = 0;

unsigned char buffer[64*4];

void InitSequence()
{
}

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
  {
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	Boot_Check();

	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	/* For example: for(;;) { } */

	AD1_Calibrate(TRUE);
	LEDS_InitHardware();
	DACBITBANG_InitHardware();

	InitSequence();

	DecoderInit();
	int T = 0;
	uint8_t LEDS[16]={255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0};
	uint8_t GATES[6]={1,1,1,1,1,1};
	for(;;) {
		T++;
		for (int i =0;i<6;i++) GATES[i] = 0;
		GATES[Reader.Sync+1] = 1;
		for (int i =0;i<16;i++) LEDS[i] = 0;
		for(int i = 0;i<64;i++)
		{
		LEDS[((History[i])/4000 + 8)%16] = 255;
		}
		if (started==1)
		{
			{

				for (int i =0;i<16;i++) LEDS[i] = 0;
				for (int i =0;i<theprogress;i++)
				{
					LEDS[i/8] += 63;
				}

			}

			if (error>0)
			{
				//show error
			}
		}
		//else
		{
			// show activity
		}
		//OLED_Blit(buffer, buffer, 10,10,10,10,10,10);
		LEDS_Update(LEDS, 16,GATES, 6);

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
