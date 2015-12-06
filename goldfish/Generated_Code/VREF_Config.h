/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : VREF_Config.h
**     Project     : goldfish
**     Processor   : MK22FN512VLH12
**     Component   : Init_VREF
**     Version     : Component 01.004, Driver 01.04, CPU db: 3.00.000
**     Repository  : Kinetis
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-12-06, 18:36, # CodeGen: 0
**     Abstract    :
**          This file implements the VREF (VREF) module initialization
**          according to the Peripheral Initialization settings, and
**          defines interrupt service routines prototypes.
**     Settings    :
**          Component name                                 : VREF
**          Device                                         : VREF
**          Settings                                       : 
**            Clock gate                                   : Enabled
**            Mode                                         : Tight
**            Regulator                                    : Enabled
**          Pins                                           : 
**            VREFO Pin                                    : Enabled
**              Output Pin                                 : <Automatic>
**          Initialization                                 : 
**            Enable VREF                                  : yes
**            Call Init method                             : yes
**            Utilize after reset values                   : default
**     Contents    :
**         Init - void VREF_Init(void);
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
** @file VREF_Config.h                                                  
** @version 01.04
** @brief
**          This file implements the VREF (VREF) module initialization
**          according to the Peripheral Initialization settings, and
**          defines interrupt service routines prototypes.
*/         
/*!
**  @addtogroup VREF_Config_module VREF_Config module documentation
**  @{
*/         

#ifndef VREF_Config_H_
#define VREF_Config_H_

/* MODULE VREF. */

/* VREF_SC: VREFEN=1,REGEN=1,ICOMPEN=0,??=0,??=0,VREFST=0,MODE_LV=1 */
#define VREF_SC_VALUE        0xC1U

#define VREF_AUTOINIT

/* END VREF. */
#endif /* #ifndef __VREF_Config_H_ */
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
