/* ###################################################################
**     Filename    : main.c
**     Project     : codec
**     Processor   : MK22FN512VLH12
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-12-04, 21:41, # CodeGen: 0
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
#include "CODEC_PDN.h"
#include "WAIT1.h"
#include "I2C1.h"
#include "SDA1.h"
#include "BitIoLdd1.h"
#include "SCL1.h"
#include "BitIoLdd2.h"
#include "DA1.h"
#include "AD1.h"
#include "AD2.h"
#include "AdcLdd1.h"
#include "TESTLED.h"
#include "MODE2A.h"
#include "MODE2B.h"
#include "MODE1A.h"
#include "MODE1B.h"
#include "LEDS.h"
#include "STCP1.h"
#include "BitIoLdd3.h"
#include "DS1.h"
#include "BitIoLdd4.h"
#include "SHCP1.h"
#include "BitIoLdd5.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "ak4558.h"
#include "cv_adc.h"

word adcvalues[AdcLdd1_CHANNEL_COUNT];

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */

  cv_adc_init();
  ak4558_init();

  Cpu_EnableInt();

  int counter = 0;
  for(;;) {
	  /*counter++;
	  if (counter > 10*1000) {
		  cv_adc_recalibrate();
		  counter = 0;
	  }*/

	  AD2_Measure(1);

	  //float fvalue = cv_adc_voltage();
	  //dac_set_voltage(fvalue);
	  //WAIT1_Waitms(1);

	  AD2_GetValue16(adcvalues);

	  ak4558_loop();
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
