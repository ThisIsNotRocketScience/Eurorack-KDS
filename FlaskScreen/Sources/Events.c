/* ###################################################################
 **     Filename    : Events.c
 **     Project     : FlaskScreen
 **     Processor   : MK22FN256VLH12
 **     Component   : Events
 **     Version     : Driver 01.00
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2017-11-19, 03:02, # CodeGen: 0
 **     Abstract    :
 **         This is user's event module.
 **         Put your event handler code here.
 **     Contents    :
 **         Cpu_OnNMI - void Cpu_OnNMI(void);
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
#include "AD1.h"
#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

/*
 ** ===================================================================
 **     Event       :  Cpu_OnNMI (module Events)
 **
 **     Component   :  Cpu [MK22FN256DC12]
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
extern int sent;
void SM1_OnBlockSent(LDD_TUserData *UserDataPtr)
{
	sent = 1;
	/* Write your code here ... */
}

/*
 ** ===================================================================
 **     Event       :  SM1_OnBlockReceived (module Events)
 **
 **     Component   :  SM1 [SPIMaster_LDD]
 */
/*!
 **     @brief
 **         This event is called when the requested number of data is
 **         moved to the input buffer. This method is available only if
 **         the ReceiveBlock method is enabled.
 **     @param
 **         UserDataPtr     - Pointer to the user or
 **                           RTOS specific data. The pointer is passed
 **                           as the parameter of Init method.
 */
/* ===================================================================*/
void SM1_OnBlockReceived(LDD_TUserData *UserDataPtr)
{
	/* Write your code here ... */
}

/*
 ** ===================================================================
 **     Event       :  AD1_OnMeasurementComplete (module Events)
 **
 **     Component   :  AD1 [ADC_LDD]
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

extern int32_t Param[4];
extern int measuring ;
void hmm()
{

}

/*
** ===================================================================
**     Event       :  AD1_OnEnd (module Events)
**
**     Component   :  AD1 [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void AD1_OnEnd(void)
{
	uint16_t P[4];
		//AD1_GetMeasuredValues(AD1_DeviceData, P);
	AD1_GetValue16(P);
		for(int i =0 ;i<4;i++)
		{
			uint16_t Pp = ~P[i];

			Param[i] =Pp;
		}
		measuring = 0;
		/* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  AD1_OnCalibrationEnd (module Events)
**
**     Component   :  AD1 [ADC]
**     Description :
**         This event is called when the calibration has been finished.
**         User should check if the calibration pass or fail by
**         Calibration status method./nThis event is enabled only if
**         the <Interrupt service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void AD1_OnCalibrationEnd(void)
{
  /* Write your code here ... */
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
