/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : PTB.h
**     Project     : EdgeCutterV2
**     Processor   : MKL02Z32VFM4
**     Component   : Init_GPIO
**     Version     : Component 01.006, Driver 01.06, CPU db: 3.00.000
**     Repository  : Kinetis
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-07-08, 23:01, # CodeGen: 0
**     Abstract    :
**          This file implements the GPIO (PTB) module initialization
**          according to the Peripheral Initialization settings, and
**          defines interrupt service routines prototypes.
**     Settings    :
**          Component name                                 : PTB
**          Device                                         : PTB
**          Settings                                       : 
**            Pin 0                                        : Do not initialize
**            Pin 1                                        : Do not initialize
**            Pin 2                                        : Initialize
**              Pin direction                              : Input
**              Output value                               : No initialization
**              Pull resistor                              : No initialization
**              Interrupt/DMA request                      : No initialization
**            Pin 3                                        : Do not initialize
**            Pin 4                                        : Do not initialize
**            Pin 5                                        : Do not initialize
**            Pin 6                                        : Do not initialize
**            Pin 7                                        : Do not initialize
**            Pin 8                                        : Do not initialize
**            Pin 9                                        : Do not initialize
**            Pin 10                                       : Initialize
**              Pin direction                              : Input
**              Output value                               : 1
**              Pull resistor                              : Enabled
**            Pin 11                                       : Initialize
**              Pin direction                              : Input
**              Output value                               : No initialization
**              Pull resistor                              : No initialization
**            Pin 12                                       : Do not initialize
**            Pin 13                                       : Initialize
**              Pin direction                              : Input
**              Output value                               : No initialization
**              Pull resistor                              : No initialization
**          Pin selection/routing                          : 
**            Pin 0                                        : Disabled
**            Pin 1                                        : Disabled
**            Pin 2                                        : Disabled
**            Pin 3                                        : Disabled
**            Pin 4                                        : Disabled
**            Pin 5                                        : Disabled
**            Pin 6                                        : Disabled
**            Pin 7                                        : Disabled
**            Pin 8                                        : Disabled
**            Pin 9                                        : Disabled
**            Pin 10                                       : Disabled
**            Pin 11                                       : Disabled
**            Pin 12                                       : Disabled
**            Pin 13                                       : Disabled
**          Interrupts                                     : 
**            Port interrupt                               : Enabled
**              Interrupt                                  : INT_PORTB
**              Interrupt request                          : Disabled
**              Interrupt priority                         : 0 (Highest)
**              ISR Name                                   : 
**          Initialization                                 : 
**            Call Init method                             : yes
**     Contents    :
**         Init - void PTB_Init(void);
**
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file PTB.h
** @version 01.06
** @brief
**          This file implements the GPIO (PTB) module initialization
**          according to the Peripheral Initialization settings, and
**          defines interrupt service routines prototypes.
*/         
/*!
**  @addtogroup PTB_module PTB module documentation
**  @{
*/         

#ifndef PTB_H_
#define PTB_H_

/* MODULE PTB. */

/* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"

/* Peripheral base address parameter */
#define PTB_DEVICE PTB_BASE_PTR

/* Peripheral port base address parameter */
#define PTB_PORT_DEVICE PORTB_BASE_PTR



/*
** ===================================================================
**     Method      :  PTB_Init (component Init_GPIO)
**     Description :
**         This method initializes registers of the GPIO module
**         according to the Peripheral Initialization settings.
**         Call this method in user code to initialize the module. By
**         default, the method is called by PE automatically; see "Call
**         Init method" property of the component for more details.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void PTB_Init(void);


/* END PTB. */
#endif /* #ifndef __PTB_H_ */
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
