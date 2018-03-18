/* ###################################################################
 **     Filename    : main.c
 **     Project     : Keyboard_Keyscanner
 **     Processor   : MK22FN256VLH12
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2018-03-08, 23:03, # CodeGen: 0
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
#include "GPIO1.h"
#include "SINK1.h"
#include "SINK2.h"
#include "SINK3.h"
#include "SINK4.h"
#include "SINK5.h"
#include "SINK6.h"
#include "SINK7.h"
#include "SINK8.h"
#include "TI1.h"
#include "TU1.h"
#include "GATEPIN.h"
#include "WAIT1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "DAC_BitBang.h"

int NoteVal = 0;
int VelVal = 0;



/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();

	PORTC_PCR0 |= 2;
	PORTC_PCR1 |= 2;
	PORTC_PCR2 |= 2;
	PORTC_PCR3 |= 2;
	PORTC_PCR4 |= 2;
	PORTC_PCR5 |= 2;
	PORTC_PCR6 |= 2;
	PORTC_PCR7 |= 2;
	PORTC_PCR8 |= 2;
	PORTC_PCR9 |= 2;
	PORTC_PCR10 |= 2;
	PORTC_PCR11 |= 2;


	PORTA_PCR12 !=2;
	PORTA_PCR13 !=2;

	PORTB_PCR18 !=2;

	PORTD_PCR0 !=2;
	PORTD_PCR3 !=2;
	PORTD_PCR5 !=2;
	PORTD_PCR6 !=2;
	PORTD_PCR7 !=2;


	GATEPIN_ClrVal(0);
	WAIT1_Waitms(200);
	GATEPIN_SetVal(0);
	WAIT1_Waitms(200);

	initialise_monitor_handles();
	/*** End of Processor Expert internal initialization.                    ***/
	DACBITBANG_InitHardware();
	//int i =0 ;
	for(;;)
	{

		//WAIT1_Waitms(1);
	}
	/* Write your code here */
	/* For example: for(;;) { } */

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
