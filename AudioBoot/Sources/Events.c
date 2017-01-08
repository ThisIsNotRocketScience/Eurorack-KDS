/* ###################################################################
 **     Filename    : Events.c
 **     Project     : AudioBoot
 **     Processor   : MKL02Z32VFM4
 **     Component   : Events
 **     Version     : Driver 01.00
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2016-11-12, 18:36, # CodeGen: 0
 **     Abstract    :
 **         This is user's event module.
 **         Put your event handler code here.
 **     Contents    :
 **         Cpu_OnNMIINT - void Cpu_OnNMIINT(void);
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

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */
#include "MessageDecoder.h"
#include "Reader.h"
/*
 ** ===================================================================
 **     Event       :  Cpu_OnNMIINT (module Events)
 **
 **     Component   :  Cpu [MKL02Z32FM4]
 */
/*!
 **     @brief
 **         This event is called when the Non maskable interrupt had
 **         occurred. This event is automatically enabled when the [NMI
 **         interrupt] property is set to 'Enabled'.
 */
/* ===================================================================*/
void Cpu_OnNMIINT(void)
{
	/* Write your code here ... */
}

/*
 ** ===================================================================
 **     Event       :  TI1_OnInterrupt (module Events)
 **
 **     Component   :  TI1 [TimerInt]
 **     Description :
 **         When a timer interrupt occurs this event is called (only
 **         when the component is enabled - <Enable> and the events are
 **         enabled - <EnableEvent>). This event is enabled only if a
 **         <interrupt service/event> is enabled.
 **     Parameters  : None
 **     Returns     : Nothing
 ** ===================================================================
 */
extern AudioReaderStruct Reader;
void TI1_OnInterrupt(void)
{
	AD1_Measure(FALSE);
	/* Write your code here ... */
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

extern int historypos;
extern int History[64];
void AD1_OnEnd(void)
{
	word V;
	AD1_GetValue16( &V);
	History[historypos] = V;
	historypos =( historypos + 1)%64;
	DecoderUpdate(V);

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

/*
 ** ===================================================================
 **     Event       :  IFsh1_OnWriteEnd (module Events)
 **
 **     Component   :  IFsh1 [IntFLASH]
 */
/*!
 **     @brief
 **         Event is called after a write operation to FLASH memory is
 **         finished (except [SetPage]). This event is available only if
 **         an [Interrupt service/event] is selected.
 */
/* ===================================================================*/
void IFsh1_OnWriteEnd(void)
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
