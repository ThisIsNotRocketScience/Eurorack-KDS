/** ###################################################################
 **     Filename    : I2S0_Init.c
 **     Processor   : MK22FN512LH12
 **     Abstract    :
 **          This file implements the I2S0 module initialization
 **          according to the Peripheral Initialization settings, and
 **          defines interrupt service routines prototypes.
 **
 **     Contents    :
 **         Int   - void I2S0_Init(void);
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
 * @file I2S0_Init.c
 * @brief This file implements the I2S0 module initialization according to the
 *        Peripheral Initialization settings, and defines interrupt service
 *        routines prototypes.
 */


/* MODULE I2S0_Init. */

#include "I2S0_Init.h"
#include "MK22F51212.h"
#include "Init_Config.h"

#ifdef __cplusplus
extern "C" {
#endif

void I2S0_Init(void) {

  /* Register 'I2S0_MCR' initialization */
  #ifdef I2S0_MCR_VALUE_1
  I2S0_MCR = I2S0_MCR_VALUE_1;
  #endif

  /* Wait for MCLK disable */
  #ifdef I2S0_WAIT_FOR_MCLK
  while ((I2S0_MCR & I2S_MCR_MOE_MASK) != 0U) {}
  #endif

  /* Register 'I2S0_MCR' initialization */
  #ifdef I2S0_MCR_VALUE_3
  I2S0_MCR = I2S0_MCR_VALUE_3;
  #endif

  /* Register 'I2S0_MDR' initialization */
  #ifdef I2S0_MDR_VALUE
  I2S0_MDR = I2S0_MDR_VALUE;
  #endif

  /* Register 'I2S0_MCR' initialization */
  #if I2S0_MCR_MASK_4
    #if I2S0_MCR_MASK_4 == 0xFFFFFFFF
  I2S0_MCR = I2S0_MCR_VALUE_4;
    #elif I2S0_MCR_MASK_4 == I2S0_MCR_VALUE_4
  I2S0_MCR |= I2S0_MCR_VALUE_4;
    #elif I2S0_MCR_VALUE_4 == 0
  I2S0_MCR &= ~I2S0_MCR_MASK_4;
    #else
  I2S0_MCR = (I2S0_MCR & (~I2S0_MCR_MASK_4)) | I2S0_MCR_VALUE_4;
    #endif
  #elif defined(I2S0_MCR_VALUE_4)
  I2S0_MCR = I2S0_MCR_VALUE_4;
  #endif

  /* Register 'I2S0_TCSR' initialization */
  #ifdef I2S0_TCSR_VALUE_1
  I2S0_TCSR = I2S0_TCSR_VALUE_1;
  #endif

  /* Wait for transmitter disable */
  #ifdef I2S0_WAIT_FOR_TRANSMITTER
  while ((I2S0_TCSR & I2S_TCSR_TE_MASK) != 0U) {}
  #endif

  /* Register 'I2S0_RCSR' initialization */
  #ifdef I2S0_RCSR_VALUE_1
  I2S0_RCSR = I2S0_RCSR_VALUE_1;
  #endif

  /* Wait for receiver disable */
  #ifdef I2S0_WAIT_FOR_RECEIVER
  while ((I2S0_RCSR & I2S_RCSR_RE_MASK) != 0U) {}
  #endif

  /* Register 'I2S0_TCR1' initialization */
  #ifdef I2S0_TCR1_VALUE
  I2S0_TCR1 = I2S0_TCR1_VALUE;
  #endif

  /* Register 'I2S0_RCR1' initialization */
  #ifdef I2S0_RCR1_VALUE
  I2S0_RCR1 = I2S0_RCR1_VALUE;
  #endif

  /* Register 'I2S0_TCR2' initialization */
  #ifdef I2S0_TCR2_VALUE
  I2S0_TCR2 = I2S0_TCR2_VALUE;
  #endif

  /* Register 'I2S0_RCR2' initialization */
  #ifdef I2S0_RCR2_VALUE
  I2S0_RCR2 = I2S0_RCR2_VALUE;
  #endif

  /* Register 'I2S0_TCR3' initialization */
  #ifdef I2S0_TCR3_VALUE
  I2S0_TCR3 = I2S0_TCR3_VALUE;
  #endif

  /* Register 'I2S0_RCR3' initialization */
  #ifdef I2S0_RCR3_VALUE
  I2S0_RCR3 = I2S0_RCR3_VALUE;
  #endif

  /* Register 'I2S0_TCR4' initialization */
  #ifdef I2S0_TCR4_VALUE
  I2S0_TCR4 = I2S0_TCR4_VALUE;
  #endif

  /* Register 'I2S0_RCR4' initialization */
  #ifdef I2S0_RCR4_VALUE
  I2S0_RCR4 = I2S0_RCR4_VALUE;
  #endif

  /* Register 'I2S0_TCR5' initialization */
  #ifdef I2S0_TCR5_VALUE
  I2S0_TCR5 = I2S0_TCR5_VALUE;
  #endif

  /* Register 'I2S0_RCR5' initialization */
  #ifdef I2S0_RCR5_VALUE
  I2S0_RCR5 = I2S0_RCR5_VALUE;
  #endif

  /* Register 'I2S0_TMR' initialization */
  #ifdef I2S0_TMR_VALUE
  I2S0_TMR = I2S0_TMR_VALUE;
  #endif

  /* Register 'I2S0_RMR' initialization */
  #ifdef I2S0_RMR_VALUE
  I2S0_RMR = I2S0_RMR_VALUE;
  #endif

  /* Register 'I2S0_TCSR' initialization */
  #ifdef I2S0_TCSR_VALUE_3
  I2S0_TCSR = I2S0_TCSR_VALUE_3;
  #endif

  /* Register 'I2S0_RCSR' initialization */
  #ifdef I2S0_RCSR_VALUE_3
  I2S0_RCSR = I2S0_RCSR_VALUE_3;
  #endif
}

#ifdef __cplusplus
}
#endif

/* END I2S0_Init. */

/** ###################################################################
 **
 **     This file is a part of Processor Expert static initialization
 **     library for Freescale microcontrollers.
 **
 ** ################################################################### */
