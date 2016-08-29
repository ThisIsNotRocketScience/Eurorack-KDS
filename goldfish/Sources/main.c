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
#include "DA1.h"
#include "AD1.h"
#include "AD2.h"
#include "AdcLdd1.h"
#include "SWITCH2.h"
#include "SWITCH1.h"
#include "LED2.h"
#include "LED1.h"
#include "GATE.h"
#include "EInt1.h"
#include "TI1.h"
#include "TU1.h"
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
#include "dsp_main.h"


#include "Goldfish/Goldfish_Interface.h"

word adcvalues[AdcLdd1_CHANNEL_COUNT];
int last_dsp_switch_octave = 1;
extern int dsp_switch_octave;
extern int dsp_switch_gate;
extern int dsp_led_octave;
extern int dsp_led_gate;
extern int pitchcvcomplete;

struct denoise_state_t
{
	int counter;
	int down;
};

byte pwm =0 ;
void updateleds()
{
	pwm+= 16;
	LED1_PutVal(LED1_DeviceData, dsp_led_octave>pwm?1:0);
	LED2_PutVal(LED2_DeviceData, dsp_led_gate>pwm?1:0);
}

int denoise(int sw_down, struct denoise_state_t* state)
{
	if (sw_down) state->counter++;
	else state->counter--;

	if (state->counter < 2) {
		state->counter = 1;
		state->down = 1;
	}
	else if (state->counter > 9) {
		state->counter = 9;
		state->down = 0;
	}

	return state->down;
}

struct toggle_sw_state_t
{
	int toggle;
	int down;
};

int toggle_sw(int down, struct toggle_sw_state_t* state)
{
	if (down && !state->down) {
		state->toggle = !state->toggle;
	}

	state->down = down;

	return state->toggle;
}

struct trigger_sw_state_t
{
	int down;
};

int trigger_sw(int down, struct trigger_sw_state_t* state)
{
	int trigger = 0;
	if (down && !state->down) {
		trigger = 1;
	}

	state->down = down;

	return trigger;
}

volatile int measurement_ongoing = 0;
volatile int measurement_done = 0;

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

  GoldfishInit();

  int counter = 0;
  static struct denoise_state_t octave_denoise_state = {0};
  static struct toggle_sw_state_t octave_sw_state = {0};
  static struct denoise_state_t gate_denoise_state = {0};
  static struct trigger_sw_state_t gate_sw_state = {0};

  for(;;) {
	  /*counter++;
	  if (counter > 10*1000) {
		  cv_adc_recalibrate();
		  counter = 0;
	  }*/

	  if (audio_buffers_fresh) {
		  audio_buffers_fresh = 0;

		  int32_t* inbuf = audio_in_buffer;
		  int32_t* outbuf = audio_out_buffer;
		  GoldfishProcess(inbuf, outbuf, AUDIO_BUFFER_SIZE);


	  }
	  if (pitchcvcomplete == 1)
	  {
		  pitchcvcomplete = 0;
		  AD1_StartSingleMeasurement(AD1_DeviceData);

	  }

	  if (!measurement_ongoing) {
		  measurement_ongoing = 1;
		  AD2_Measure(0);
	  }
	  else if (measurement_done) {
		  measurement_ongoing = 0;
		  measurement_done = 0;

		  AD2_GetValue16(adcvalues);
	  }

	  int switch_octave = SWITCH1_GetVal(SWITCH1_DeviceData);
	  int switch_gate = SWITCH2_GetVal(SWITCH2_DeviceData);
	  dsp_switch_octave = toggle_sw(denoise(switch_octave, &octave_denoise_state), &octave_sw_state);
	  if (dsp_switch_octave != last_dsp_switch_octave)
	  {
		  GoldfishOctavePressed();
	  }
	  last_dsp_switch_octave = dsp_switch_octave;

	  dsp_switch_gate = trigger_sw(denoise(switch_gate, &gate_denoise_state), &gate_sw_state);
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
