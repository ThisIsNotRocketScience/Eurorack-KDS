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
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
void ak4558_init()
{
	CODEC_PDN_SetVal(CODEC_PDN_DeviceData);
	WAIT1_Waitms(100);
	volatile byte res = I2C1_SelectSlave(0b0010000);
	unsigned char ak4558_configdata[] = { 0, 0b00011111, 2, 0b00000000, 3,
			0b00111110, 4, 0b00010100, 5, 0b01110010, 6, 0b00011001, 7,
			0b00001100, 8, 255, 9, 255, 1, 0b00001111 };
	for (int i = 0; i < sizeof(ak4558_configdata) / 2; i += 2) {
		word sent = 0;
		byte res = I2C1_SendBlock(&ak4558_configdata[i], 2, &sent);
	}
}

uint32_t pos = 0;

float value = 0.0;

PE_ISR(I2S0_TX)
{
    I2S0_TDR0 = pos;
    //I2S0_TDR0 = pos;

    value = value + (1000.0f/187500.0f);
    if (value > 1.0) value = 0.0;
    pos = 0x80000000 + (int32_t)(value * (float)(1ULL<<31ULL));

    I2S_PDD_ClearTxInterruptFlags(I2S0_BASE_PTR, I2S_PDD_ALL_INT_FLAG);
}

PE_ISR(I2S0_RX)
{
    volatile uint32_t in = I2S0_RDR0;
    //in = I2S0_RDR0;

    I2S_PDD_ClearRxInterruptFlags(I2S0_BASE_PTR, I2S_PDD_ALL_INT_FLAG);
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
  ak4558_init();

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
