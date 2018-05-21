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


void aLED_LATCH_SET() {GPIOB_PDOR |= (1<<LED_LATCH);};
void aLED_LATCH_CLR() {GPIOB_PDOR &= ~(1<<LED_LATCH);};
void aLED_CLK_SET() {GPIOB_PDOR |= (1<<LED_CLOCK);};
void aLED_CLK_CLR() {GPIOB_PDOR &= ~(1<<LED_CLOCK);};
void aLED_DATA_SET() {GPIOB_PDOR |= (1<<LED_DATA);};
void aLED_DATA_CLR() {GPIOB_PDOR &= ~(1<<LED_DATA);};


#define SetIf(x){if (x) {aLED_DATA_SET();}else {aLED_DATA_CLR();}aLED_CLK_CLR(); aLED_CLK_SET();};
#define SetNotIf(x){if (x) {aLED_DATA_CLR();}else {aLED_DATA_SET();}aLED_CLK_CLR(); aLED_CLK_SET();};
uint8_t LEDS[25]={0};

uint8_t Order[23]=
{
		 12,17,16,15,14,13,18,4,
		  5, 6, 7, 8, 9,10,11,19,
		 20,21, 3, 2, 1, 0,22};

#define GATE_ATTACKEND 19
#define GATE_DECAYEND 20
#define GATE_RELEASESTART 21
#define GATE_RELEASEEND 22

void DoShiftOut()
{
	//counter++;


	aLED_LATCH_CLR();

for(int i = 0;i<23;i++) SetIf(LEDS[Order[i]] );

aLED_LATCH_SET();




}

int T = 0;


void SetLed(int i)
{
	LEDS[i] = 1;
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

// 15 1
	/*
	SetIf(outleds[12] > pwm);
	SetIf(outleds[17] > pwm);
	SetIf(outleds[16] > pwm);
	SetIf(outleds[15] > pwm);
	SetIf(outleds[14] > pwm);
	SetIf(outleds[13] > pwm);
	SetIf(outleds[18] > pwm);
	SetIf(outleds[4] > pwm);
	SetIf(outleds[5] > pwm);
	SetIf(outleds[6] > pwm);
	SetIf(outleds[7] > pwm);
	SetIf(outleds[8] > pwm);
	SetIf(outleds[9] > pwm);
	SetIf(outleds[10] > pwm);
	SetIf(outleds[11] > pwm);
	SetNotIf(Envelope.Gates[GATE_DECAYEND]>0);
	SetNotIf(Envelope.Gates[GATE_RELEASESTART]>0);
	SetNotIf(Envelope.Gates[GATE_RELEASEEND]>0);
	SetIf(outleds[3] > pwm);
	SetIf(outleds[2] > pwm);
	SetIf(outleds[1] > pwm);
	SetIf(outleds[0] > pwm);
	SetNotIf(Envelope.Gates[GATE_ATTACKEND]>0);

	 */

	uint8_t GATES[6]={1,1,1,1,1,1};
	for(;;) {
		T++;
		for (int i =0;i<23;i++) LEDS[i] = 0;
		SetLed(GATE_ATTACKEND +  Reader.Sync );
		//SetLed((T/1000)%23);
		for(int i = 0;i<64;i++)
		{
			SetLed(((History[i])/4000 + 8)%13);
		}
		if (started==1)
		{
			{

				for (int i =0;i<16;i++) LEDS[i] = 0;
				for (int i =0;i<theprogress;i++)
				{
					SetLed((i*13)/256);
				}

			}


			if (error>0)
			{
				if ((T/400) % 2 == 0)
				{
					for (int i =0;i<23;i++)
					{
						 SetLed(i);
					}
				}
				//show error
			}
		}
		else
		{
			// show activity
		}

		DoShiftOut();


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
