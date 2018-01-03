/* ###################################################################
**     Filename    : Events.c
**     Project     : BigFish
**     Processor   : MK22FN256VLH12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-05-24, 04:07, # CodeGen: 8
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Contents    :
**         AD1_OnMeasurementComplete     - void AD1_OnMeasurementComplete(LDD_TUserData *UserDataPtr);
**         ADMUXED_OnMeasurementComplete - void ADMUXED_OnMeasurementComplete(LDD_TUserData *UserDataPtr);
**         CI2C1_OnMasterBlockSent       - void CI2C1_OnMasterBlockSent(LDD_TUserData *UserDataPtr);
**         CI2C1_OnMasterBlockReceived   - void CI2C1_OnMasterBlockReceived(LDD_TUserData *UserDataPtr);
**         Cpu_OnNMI                     - void Cpu_OnNMI(void);
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "Init_Config.h"
#include "PDD_Includes.h"

#ifdef __cplusplus
extern "C" {
#endif 

extern void Timer10khz();
extern void ADCMUXING();
/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Event       :  ADMUXED_OnMeasurementComplete (module Events)
**
**     Component   :  ADMUXED [ADC_LDD]
*/
/*!
**     @brief
**         Called after measurement is done, [Interrupt service/event]
**         is enabled, OnMeasurementComplete event is enabled and ADC
**         device is enabled. See [SetEventMask()] method or [Event
**         mask] property group to enable this event and [Enable]
**         method or [Enabled in init. code] property to enable ADC
**         device. If DMA is enabled , this event is called after the
**         configured number of measurements and DMA transfer is done.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer is passed
**                           as the parameter of Init method. 
*/
/* ===================================================================*/
void ADMUXED_OnMeasurementComplete(LDD_TUserData *UserDataPtr)
{
}

/*
** ===================================================================
**     Event       :  Cpu_OnNMI (module Events)
**
**     Component   :  Cpu [MK22FN256LH12]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMI(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
**
**     Component   :  TI1 [TimerInt_LDD]
*/
/*!
**     @brief
**         Called if periodic event occur. Component and OnInterrupt
**         event must be enabled. See [SetEventMask] and [GetEventMask]
**         methods. This event is available only if a [Interrupt
**         service/event] is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer passed as
**                           the parameter of Init method.
*/
/* ===================================================================*/
void TI1_OnInterrupt(LDD_TUserData *UserDataPtr)
{
	Timer10khz();
}

/*
** ===================================================================
**     Event       :  ADMUXED_OnEnd (module Events)
**
**     Component   :  ADMUXED [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
extern int Measuring;
void ADMUXED_OnEnd(void)
{
	Measuring = 1;

  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  ADMAIN_OnEnd (module Events)
**
**     Component   :  ADMAIN [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void ADMAIN_OnEnd(void)
{

	Measuring = 3;
}

/*
** ===================================================================
**     Event       :  ADMAIN_OnCalibrationEnd (module Events)
**
**     Component   :  ADMAIN [ADC]
**     Description :
**         This event is called when the calibration has been finished.
**         User should check if the calibration pass or fail by
**         Calibration status method./nThis event is enabled only if
**         the <Interrupt service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void ADMAIN_OnCalibrationEnd(void)
{
  /* Write your code here ... */
}
extern int sent;

/*
** ===================================================================
**     Event       :  SM1_OnBlockSent (module Events)
**
**     Component   :  SM1 [SPIMaster_LDD]
*/
/*!
**     @brief
**         This event is called after the last character from the
**         output buffer is moved to the transmitter. This event is
**         available only if the SendBlock method is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer is passed
**                           as the parameter of Init method. 
*/
/* ===================================================================*/
void SM1_OnBlockSent(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
	sent = 1;
}

/* END Events */

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
