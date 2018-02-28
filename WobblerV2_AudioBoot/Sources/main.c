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
#include "IntI2cLdd1.h"
#include "WAIT1.h"
#include "EE241.h"
#include "GI2C1.h"
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

int T = 0;
uint8_t LEDS[24]={0};                  // 8, 9 , 19, 11,12,13, 14 , 15
//uint8_t LEDSORDER[24]={0,1,2,3,4,5,6,7,  255,255,255,8,255,255,255, 9,10,11,12,13,14,15,16,17};
//uint8_t LEDSORDER[18]={0,1,2,3,4,5,6,7,  11,  15,  16,17,18,19,20,21,22,23};

const  uint8_t LEDSORDER[18]={23,22,21,20,19,18,17,16,12,8,7,6,5,4,3,2,1,0};
const  uint8_t LEDSORDER1[9]={8,7,6,5,4,3,2,1,0};
const  uint8_t LEDSORDER2[9]={23,22,21,20,19,18,17,16,12};

  //uint8_t LEDSORDERo[18]={12,16,17,18,19,20,21,22,23,
    //                        0,1,2,3,4,5,6,7,8};

void SetLed(int i, unsigned char v)
{
	LEDS[LEDSORDER[i]] = v;
}
void AddLed(int i, unsigned char v)
{
	SetLed(i, LEDS[LEDSORDER[i]]+v);
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

//	AD1_Calibrate(TRUE);
	LEDS_InitHardware();
	DACBITBANG_InitHardware();

	InitSequence();

	DecoderInit();

	for(;;) {
		T++;

		for (int i =0;i<24;i++) LEDS[i] = 0;
		LEDS[9] = ((T/2000)%3)==0?255:0;
		LEDS[10] =((T/2000)%3)==1?255:0;
		LEDS[11] = ((T/2000)%3)==2?255:0;

		for(int i = 0;i<64;i++)
		{
			int num =(((History[i])/500 )%9);
			SetLed(num ,255);
			SetLed(17-  num,255);
		}
		if (started==1)
		{
			LEDS[9] = ((T/2000)%2)==0?255:0;
			LEDS[10] =((T/2000)%2)==1?255:0;
			LEDS[11] = ((T/2000)%2)==1?255:0;
			{
				SetLed(0, 255);
				SetLed(17,255);
				for (int i =0;i<16;i++) SetLed(i+1, 0);
				for (int i =0;i<theprogress;i++)
				{
					AddLed((i/ (256/16))+1, 63);
				}

			}


			if (error>0)
			{
				if ((T/400) % 2 == 0)
				{
					for (int i =0;i<16;i++)
							{
								LEDS[LEDSORDER[i+1]] = ((T/200) + i) % 2 == 0 ? 0: LEDS[LEDSORDER[i+1]];
							}
				}
				//show error
			}
		}
		else
		{
			// show activity
		}
		//OLED_Blit(buffer, buffer, 10,10,10,10,10,10);
		//for (int i =0;i<18;i++) SetLed(i,  ((T/1000)% 18 == i)?255:0);

	//	for (int i =0;i<24;i++) LEDS[i] = 0;
		//LEDS[LEDSORDER1[(T/1000)%9]] = 255;
		LEDS_Update(LEDS, 24,0, 0);

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
