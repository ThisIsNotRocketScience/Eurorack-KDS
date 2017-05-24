/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : ADMUXED.c
**     Project     : BigFish
**     Processor   : MK22FN256VLH12
**     Component   : ADC_LDD
**     Version     : Component 01.183, Driver 01.08, CPU db: 3.00.000
**     Repository  : Kinetis
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-05-24, 04:21, # CodeGen: 9
**     Abstract    :
**         This device "ADC_LDD" implements an A/D converter,
**         its control methods and interrupt/event handling procedure.
**     Settings    :
**          Component name                                 : ADMUXED
**          A/D converter                                  : ADC1
**          Discontinuous mode                             : no
**          Interrupt service/event                        : Enabled
**            A/D interrupt                                : INT_ADC1
**            A/D interrupt priority                       : medium priority
**          DMA                                            : Disabled
**          A/D channel list                               : 3
**            Channel 0                                    : 
**              Channel mode                               : Single Ended
**                Input                                    : 
**                  A/D channel (pin)                      : ADC1_SE5b/CMP0_IN3/PTC9/I2S0_RX_BCLK/FTM2_FLT0
**            Channel 1                                    : 
**              Channel mode                               : Single Ended
**                Input                                    : 
**                  A/D channel (pin)                      : ADC1_SE6b/PTC10/I2C1_SCL/I2S0_RX_FS
**            Channel 2                                    : 
**              Channel mode                               : Single Ended
**                Input                                    : 
**                  A/D channel (pin)                      : ADC1_SE7b/PTC11/LLWU_P11/I2C1_SDA
**          Static sample groups                           : Disabled
**          Max. samples                                   : 8
**          A/D resolution                                 : Autoselect
**          Low-power mode                                 : Disabled
**          High-speed conversion mode                     : Disabled
**          Asynchro clock output                          : Disabled
**          Sample time                                    : 24 clock periods
**          Number of conversions                          : 32
**          Conversion time                                : 240 �s
**          ADC clock                                      : 5.999 MHz (166.667 ns)
**          Single conversion time - Single-ended          : 240.583 us
**          Single conversion time - Differential          : 288.583 us
**          Additional conversion time - Single-ended      : 240 us
**          Additional conversion time - Differential      : 288 us
**          Result type                                    : unsigned 16 bits, right justified
**          Trigger                                        : Disabled
**          Voltage reference                              : 
**            High voltage reference                       : 
**              Volt. ref. pin                             : VREFH
**            Low voltage reference                        : 
**              Volt. ref. pin                             : VREFL
**          Initialization                                 : 
**            Enabled in init. code                        : yes
**            Auto initialization                          : no
**            Event mask                                   : 
**              OnMeasurementComplete                      : Enabled
**              OnError                                    : Disabled
**          CPU clock/configuration selection              : 
**            Clock configuration 0                        : This component enabled
**            Clock configuration 1                        : This component disabled
**            Clock configuration 2                        : This component disabled
**            Clock configuration 3                        : This component disabled
**            Clock configuration 4                        : This component disabled
**            Clock configuration 5                        : This component disabled
**            Clock configuration 6                        : This component disabled
**            Clock configuration 7                        : This component disabled
**     Contents    :
**         Init                   - LDD_TDeviceData* ADMUXED_Init(LDD_TUserData *UserDataPtr);
**         StartSingleMeasurement - LDD_TError ADMUXED_StartSingleMeasurement(LDD_TDeviceData *DeviceDataPtr);
**         GetMeasuredValues      - LDD_TError ADMUXED_GetMeasuredValues(LDD_TDeviceData *DeviceDataPtr,...
**         CreateSampleGroup      - LDD_TError ADMUXED_CreateSampleGroup(LDD_TDeviceData *DeviceDataPtr,...
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
** @file ADMUXED.c
** @version 01.08
** @brief
**         This device "ADC_LDD" implements an A/D converter,
**         its control methods and interrupt/event handling procedure.
*/         
/*!
**  @addtogroup ADMUXED_module ADMUXED module documentation
**  @{
*/         

/* MODULE ADMUXED. */

#include "Events.h"
#include "ADMUXED.h"
/* {Default RTOS Adapter} No RTOS includes */

#ifdef __cplusplus
extern "C" { 
#endif

#define ADMUXED_AVAILABLE_CHANNEL0_31_PIN_MASK (LDD_ADC_CHANNEL_0_PIN | LDD_ADC_CHANNEL_1_PIN | LDD_ADC_CHANNEL_2_PIN) /*!< Mask of all allocated channel pins from 0 to 31 */
#define ADMUXED_AVAILABLE_CHANNEL32_63_PIN_MASK 0x00U /*!< Mask of all allocated channel pins from 32 to 63 */
#define ADMUXED_AVAILABLE_TRIGGER_PIN_MASK 0x00U /*!< Mask of all allocated trigger pins */
#define ADMUXED_AVAILABLE_VOLT_REF_PIN_MASK (LDD_ADC_LOW_VOLT_REF_PIN | LDD_ADC_HIGH_VOLT_REF_PIN) /*!< Mask of all allocated voltage reference pins */

static const uint8_t ChannelToPin[] = { /* Channel to pin conversion table */
  /* ADC1_SC1A: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,COCO=0,AIEN=1,DIFF=0,ADCH=5 */
  0x45U,                               /* Status and control register value */
  /* ADC1_SC1A: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,COCO=0,AIEN=1,DIFF=0,ADCH=6 */
  0x46U,                               /* Status and control register value */
  /* ADC1_SC1A: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,COCO=0,AIEN=1,DIFF=0,ADCH=7 */
  0x47U                                /* Status and control register value */
};

typedef struct {
  uint8_t SampleCount;                 /* Number of samples in the last selected/created sample group */
  uint8_t FirstSample;                 /* First sample of group store */
  LDD_TUserData *UserData;             /* RTOS device data structure */
} ADMUXED_TDeviceData;                 /* Device data structure type */

typedef ADMUXED_TDeviceData* ADMUXED_TDeviceDataPtr ; /* Pointer to the device data structure. */

/* {Default RTOS Adapter} Static object used for simulation of dynamic driver memory allocation */
static ADMUXED_TDeviceData DeviceDataPrv__DEFAULT_RTOS_ALLOC;
/* {Default RTOS Adapter} Global variable used for passing a parameter into ISR */
static ADMUXED_TDeviceDataPtr INT_ADC1__DEFAULT_RTOS_ISRPARAM;
/*
** ===================================================================
**     Method      :  ADMUXED_Init (component ADC_LDD)
*/
/*!
**     @brief
**         Initializes the device. Allocates memory for the device data
**         structure, allocates interrupt vectors and sets interrupt
**         priority, sets pin routing, sets timing, etc.
**         If the "Enable in init. code" is set to "yes" value then the
**         device is also enabled(see the description of the Enable()
**         method). In this case the Enable() method is not necessary
**         and needn't to be generated. 
**         This method can be called only once. Before the second call
**         of Init() the Deinit() must be called first.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer will be
**                           passed as an event or callback parameter.
**     @return
**                         - Device data structure pointer.
*/
/* ===================================================================*/
LDD_TDeviceData* ADMUXED_Init(LDD_TUserData *UserDataPtr)
{
  /* Allocate LDD device structure */
  ADMUXED_TDeviceDataPtr DeviceDataPrv;

  /* {Default RTOS Adapter} Driver memory allocation: Dynamic allocation is simulated by a pointer to the static object */
  DeviceDataPrv = &DeviceDataPrv__DEFAULT_RTOS_ALLOC;
  DeviceDataPrv->UserData = UserDataPtr; /* Store the RTOS device structure */
  /* Interrupt vector(s) allocation */
  /* {Default RTOS Adapter} Set interrupt vector: IVT is static, ISR parameter is passed by the global variable */
  INT_ADC1__DEFAULT_RTOS_ISRPARAM = DeviceDataPrv;
  DeviceDataPrv->SampleCount = 0U;     /* Initializing SampleCount for right access of some methods to SC1n registers before first conversion is done */
  /* SIM_SCGC6: ADC1=1 */
  SIM_SCGC6 |= SIM_SCGC6_ADC1_MASK;
  /* Interrupt vector(s) priority setting */
  /* NVICIP73: PRI73=0x70 */
  NVICIP73 = NVIC_IP_PRI73(0x70);
  /* NVICISER2: SETENA|=0x0200 */
  NVICISER2 |= NVIC_ISER_SETENA(0x0200);
  /* Enable device clock gate */
  /* SIM_SCGC5: PORTC=1 */
  SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
  /* SIM_SCGC6: ADC1=1 */
  SIM_SCGC6 |= SIM_SCGC6_ADC1_MASK;
  /* Initialization of pin routing */
  /* PORTC_PCR9: ISF=0,MUX=0 */
  PORTC_PCR9 &= (uint32_t)~(uint32_t)((PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07)));
  /* PORTC_PCR10: ISF=0,MUX=0 */
  PORTC_PCR10 &= (uint32_t)~(uint32_t)((PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07)));
  /* PORTC_PCR11: ISF=0,MUX=0 */
  PORTC_PCR11 &= (uint32_t)~(uint32_t)((PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07)));
  /* ADC1_SC2: REFSEL=0 */
  ADC1_SC2 &= (uint32_t)~(uint32_t)(ADC_SC2_REFSEL(0x03));
  /* ADC1_CFG1: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,ADLPC=0,ADIV=3,ADLSMP=1,MODE=3,ADICLK=1 */
  ADC1_CFG1 = ADC_CFG1_ADIV(0x03) |
              ADC_CFG1_ADLSMP_MASK |
              ADC_CFG1_MODE(0x03) |
              ADC_CFG1_ADICLK(0x01);

  /* ADC1_CFG2: MUXSEL=1,ADACKEN=0,ADHSC=0,ADLSTS=0 */
  ADC1_CFG2 = (uint32_t)((ADC1_CFG2 & (uint32_t)~(uint32_t)(
               ADC_CFG2_ADACKEN_MASK |
               ADC_CFG2_ADHSC_MASK |
               ADC_CFG2_ADLSTS(0x03)
              )) | (uint32_t)(
               ADC_CFG2_MUXSEL_MASK
              ));
  /* ADC1_SC2: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,ADACT=0,ADTRG=0,ACFE=0,ACFGT=0,ACREN=0,DMAEN=0,REFSEL=0 */
  ADC1_SC2 = ADC_SC2_REFSEL(0x00);
  /* ADC1_SC3: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,CAL=0,CALF=1,??=0,??=0,ADCO=0,AVGE=1,AVGS=3 */
  ADC1_SC3 = (ADC_SC3_CALF_MASK | ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(0x03));
  /* Registration of the device structure */
  PE_LDD_RegisterDeviceStructure(PE_LDD_COMPONENT_ADMUXED_ID,DeviceDataPrv);
  return ((LDD_TDeviceData *)DeviceDataPrv); /* Return pointer to the data data structure */
}

/*
** ===================================================================
**     Method      :  ADMUXED_StartSingleMeasurement (component ADC_LDD)
*/
/*!
**     @brief
**         This method starts one measurement of the selected group of
**         samples and exits immediately. This is ADC SW trigger method.
**         The group of samples for measurement is specified by
**         preceding call to [SelectSampleGroup()] or
**         [CreateSampleGroup] method. 
**         DMA disabled: The [OnMeasurementComplete() ] event is
**         invoked after the measurement is done and if the event is
**         enabled. Results of the measurement can be read by the
**         [GetMeasuredValues()] method. 
**         DMA enabled: DMA request from configured ADC is enabled
**         automatically. The [OnMeasurementComplete() ] event is
**         invoked after the requested number of results are
**         transferred to destination buffer by DMA and if the event is
**         enabled. [GetMeasuredValues()] method is not available if
**         DMA mode is enabled. If the DMA transfer was completed
**         before and DMA is not recofingured, DMA error can occur. See
**         also [SetBuffer()] method. 
**         The state of the measurement can be also polled by the
**         [GetMeasurementCompleteStatus()] method. The [Discontinuous
**         mode] doesn't support this method.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - The device doesn't work in the
**                           active clock configuration
**                           ERR_DISABLED - Component is disabled
**                           ERR_BUSY - A measurement is in progress 
*/
/* ===================================================================*/
LDD_TError ADMUXED_StartSingleMeasurement(LDD_TDeviceData *DeviceDataPtr)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  if (ADC_PDD_GetConversionActiveFlag(ADC1_BASE_PTR) != 0U) { /* Last measurement still pending? */
    return ERR_BUSY;                   /* Yes, return ERR_BUSY */
  }
  ADC_PDD_SetContinuousMode(ADC1_BASE_PTR, ADC_PDD_ONE_CONVERSION); /* Set one conversion mode */
  ADC_PDD_SetConversionTriggerType(ADC1_BASE_PTR, ADC_PDD_SW_TRIGGER); /* Select SW triggering */
  ADC_PDD_WriteStatusControl1Reg(ADC1_BASE_PTR, 0U, ((ADMUXED_TDeviceDataPtr)DeviceDataPtr)->FirstSample); /* Set sample 0 and start conversion */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  ADMUXED_CreateSampleGroup (component ADC_LDD)
*/
/*!
**     @brief
**         This method prepares HW for next measurement according to
**         array of samples defined during run-time. The array of
**         samples should be prepared prior to calling this method.
**         Pointer to the array is passed into this method in parameter
**         SampleGroupPtr. The number of samples is defined by
**         parameter SampleCount. Once any group is prepared, the
**         measurement can be started multiple times. Note: This method
**         works only with the sample groups defined during run-time.
**         For design-time defined groups use [SelectSampleGroup()]
**         method.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         SampleGroupPtr  - Pointer to the
**                           sample definition array. This array can be
**                           released as soon as the method ends.
**     @param
**         SampleCount     - Number of items in the
**                           sample definition array. Must be less than
**                           or equal to
**                           ComponentName_MAX_HW_SAMPLE_COUNT.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - The device doesn't work in the
**                           active clock configuration
**                           ERR_DISABLED - Component is disabled
**                           ERR_PARAM_INDEX - Channel index in
**                           SampleGroup structure is out of range
**                           ERR_PARAM_SAMPLE_COUNT - SampleCount
**                           variable value is out of range
**                           ERR_BUSY - Measurement is in progress 
*/
/* ===================================================================*/
LDD_TError ADMUXED_CreateSampleGroup(LDD_TDeviceData *DeviceDataPtr, LDD_ADC_TSample *SampleGroupPtr, uint8_t SampleCount)
{
  ADMUXED_TDeviceDataPtr DeviceDataPrv = (ADMUXED_TDeviceDataPtr)DeviceDataPtr;

  /* Sample count test - this test can be disabled by setting the "Ignore range checking"
     property to the "yes" value in the "Configuration inspector" */
  if ((SampleCount > ADMUXED_MAX_HW_SAMPLE_COUNT) || (SampleCount == 0U)) { /* Is number of sample greater then supported by the HW? */
    return ERR_PARAM_SAMPLE_COUNT;     /* Yes, return ERR_PARAM_SAMPLE_COUNT */
  }
  if (ADC_PDD_GetConversionActiveFlag(ADC1_BASE_PTR) != 0U) { /* Last measurement still pending? */
    return ERR_BUSY;                   /* Yes, return ERR_BUSY */
  }
  DeviceDataPrv->SampleCount = SampleCount; /* Remember sample count */
  /* Channel index test - this test can be disabled by setting the "Ignore range checking"
     property to the "yes" value in the "Configuration inspector" */
  if (SampleGroupPtr[0].ChannelIdx >= ADMUXED_CHANNEL_COUNT) { /* Is channel index out of range? */
    return ERR_PARAM_INDEX;            /* Yes, return ERR_PARAM_INDEX */
  }
  DeviceDataPrv->FirstSample = (ChannelToPin[SampleGroupPtr[0].ChannelIdx]) | (uint8_t)(LDD_ADC_ON_MEASUREMENT_COMPLETE); /* Remember first sample */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  ADMUXED_GetMeasuredValues (component ADC_LDD)
*/
/*!
**     @brief
**         This method copies results of the last measurement to the
**         user supplied buffer. Data size depends on the size of
**         measured sample group (see [SelectSampleGroup()] or
**         [CreateSampleGroup()] method). Data representation is
**         defined by the [Result type] property. Typically this method
**         is called from [OnMeasurementComplete] event to get results
**         of the last measurement. This method is not available if DMA
**         is enabled.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         BufferPtr       - Pointer to the start of the
**                           buffer for new results. Count of stored
**                           measured values equals to the count of the
**                           samples in the active sample group. It is
**                           in the user responsibility to provide
**                           buffer with appropriate size.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - The device doesn't work in the
**                           active clock configuration
**                           ERR_DISABLED - Component is disabled
*/
/* ===================================================================*/
LDD_TError ADMUXED_GetMeasuredValues(LDD_TDeviceData *DeviceDataPtr, LDD_TData *BufferPtr)
{
  uint8_t Sample;
  ADMUXED_TResultData *pBuffer = (ADMUXED_TResultData *)BufferPtr;

  /* Copy values from result registers defined in the active sample
     group to the user supplied buffer */
  for (Sample = 0U; Sample < ((ADMUXED_TDeviceDataPtr)DeviceDataPtr)->SampleCount; Sample++) {
    pBuffer[Sample] =(uint16_t)(ADC_PDD_GetResultValueRaw(ADC1_BASE_PTR, Sample));
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  ADMUXED_MeasurementCompleteInterrupt (component ADC_LDD)
**
**     Description :
**         Measurement complete interrupt handler
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(ADMUXED_MeasurementCompleteInterrupt)
{
  /* {Default RTOS Adapter} ISR parameter is passed through the global variable */
  ADMUXED_TDeviceDataPtr DeviceDataPrv = INT_ADC1__DEFAULT_RTOS_ISRPARAM;
  ADMUXED_OnMeasurementComplete(DeviceDataPrv->UserData);
  (void)ADC_PDD_GetResultValueRaw(ADC1_BASE_PTR, 0U); /* Clear conversion complete flag */
}

/* END ADMUXED. */

#ifdef __cplusplus
}  /* extern "C" */
#endif

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
