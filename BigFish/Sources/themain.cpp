/* ###################################################################
 **     Filename    : main.c
 **     Project     : BigFish
 **     Processor   : MK22FN256VLH12
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2017-05-24, 01:44, # CodeGen: 0
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

#include "ADMUXED.h"
#include "ADMAIN.h"
#include "LATCH.h"
#include "CLOCK.h"
#include "SHIFTERS.h"
#include "ADCSELA.h"
#include "ADCSELB.h"
#include "ADCSELC.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "GATE_TRIGGER.h"
#include "GATE_BUTTON.h"
#include "FILTER_BUTTON.h"
#include "ACCENT_TRIGGER.h"
#include "Init_Config.h"
/* User includes (#include below this line is not maintained by Processor Expert) */

#include "LEDS.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define __max(a,b) (((a)>(b))?(a):(b))
#include "../../BigFishLib/BigFish.cpp"
#include "../../BigFishLib/ADSREnvelope.cpp"
#include "../../BigFishLib/BleppyOscs.cpp"
#include "../../BigFishLib/MinBlepGen.cpp"
#include "../../BigFishLib/OrganOsc.cpp"
#include "../../BigFishLib/FMOsc.cpp"


#include "ak4558.h"

#define LONGPRESSCYCLES 2000

struct denoise_state_t
{
	int counter;
	int down;
	unsigned char pressed:4;
	unsigned char released:4;
	int longpressed;
	int lastcounter;
};


int islongpress( struct denoise_state_t *state)
{
	if (state->longpressed >= LONGPRESSCYCLES)
	{
		state->longpressed  = 0;
		return 1;
	}
	return 0;
}

int pressed(struct denoise_state_t *state)
{
	if (state->pressed == 1)
	{
		state->pressed = 0;
		return 1;
	}
	return 0;
}

int released(struct denoise_state_t *state)
{
	if (state->released== 1)
	{
		state->released = 0;
		return 1;
	}
	return 0;
}

void denoise(int sw_down, struct denoise_state_t *state)
{
	if (sw_down)
	{
		state->counter++;
	}
	else
	{
		state->counter--;
	}

	//state->pressed = 0;
	//state->released = 0;

	if (state->counter < 2)
	{
		if (state->lastcounter == 2)
		{
			state->pressed = 1;
		}
		state->counter = 1;
		state->down = 1;
	}
	else
	{
		if (state->counter > 30)
		{
			if (state->lastcounter == 30)
			{
				state->released = 1;
			}
			state->counter = 31;
			state->down = 0;
		}
	}

	if (state->down > 0)
	{
		state->longpressed++;
	}
	else
	{
		state->longpressed = 0;
	}

	state->lastcounter = state->counter;

	if (state->longpressed >= LONGPRESSCYCLES)
	{
		state->longpressed = LONGPRESSCYCLES;
	}
}



#define BIGFISH_LEDS 16
const int SHAPELEDS[7+1] ={

		(7-3)+8,
		(7-1)+8,

		(7-0)+8,
		(7-2)+8,
		(7-4)+8,

		(7-6)+8,
		(7-5)+8,


		BIGFISH_LEDS+1};
#define TRIGGERBUTTONLED (0+8)
#define FILTERBUTTONLED 4
const int FILTERLEDS[5] = {7-1, 7-2, 7-4, 7-6, 7-5};
#define ENVELOPELED 0
uint8_t LEDS[BIGFISH_LEDS + 2] ={0};
uint8_t LEDTARGET[BIGFISH_LEDS + 2]={0} ;
void UpdateLeds()
{
	for (int i = 0; i < BIGFISH_LEDS; i++)
	{
		if (LEDTARGET[i] > LEDS[i])
		{
			int newL = LEDS[i] +2;
			if(newL>255) newL = 255;

			LEDS[i]++;
		}
		else
		{
			if (LEDTARGET[i] < LEDS[i])
			{
				int newL = LEDS[i] -2;
				if(newL<0) newL = 0;
				LEDS[i] = newL;
			}
			if (LEDTARGET[i] < LEDS[i])
			{
				int newL = LEDS[i] -2;
				if(newL<0) newL = 0;
				LEDS[i]--;
			}
		}
	}
}



BigFish_t Fish;
extern "C"
{
extern void AudioInit();

uint16_t PARAMADC[24];
enum
{
	ADC_SHAPE,
	ADC_ATTACK,
	ADC_FILTERENVELOPE,
	ADC_FINE,
	ADC_DECAY,
	ADC_FILTERDECAY,
	ADC_COARSE,
	ADC_RESONANCE,
	ADC_FILTERATTACK,
	ADC_KEYTRACK,
	ADC_DRIFT,
	ADC_CUTOFF,
	ADC_CHORD,
	ADC_PEAKS,
	ADC_UNUSED2,
	ADC_SPREAD,
	ADC_SUSTAIN,
	ADC_UNUSED3,
	ADC_SIZE,
	ADC_DRIVE,
	ADC_UNUSED4,
	ADC_ACCENT,
	ADC_RELEASE,
	ADC_UNUSED5,
};

void ADCMUXING();
int Measuring = 0;
int MainMeasuring = 0;

extern void DoProcessing(int32_t *In, int32_t *Out1,int32_t *Out2, uint32_t len);



void NextBlock(int32_t *input, int32_t *output)
{

	int32_t OutMain[32];
	int32_t OutOsc[32] ;
	int32_t InFilt[32] ;
	for(int i =0 ;i<32;i++)
	{
		InFilt[i] = input[i*2];
	}
	DoProcessing(InFilt, OutMain, OutOsc,32);
	for(int i =0 ;i<32;i++)
	{
		output[i * 2 + 1] = OutMain[i] << 13;
		output[i * 2 ] = OutOsc[i] << 13;
	}


}

void DoProcessing(int32_t *In, int32_t *Out1,int32_t *Out2, uint32_t len)
{
	Fish.Parameters[OSC_SHAPE] = PARAMADC[ADC_SHAPE];
	Fish.Parameters[OSC_SPREAD] = ~PARAMADC[ADC_SPREAD];
	Fish.Parameters[OSC_SIZE] = ~PARAMADC[ADC_SIZE];

	Fish.Parameters[PITCH_FINE] = PARAMADC[ADC_FINE];
	Fish.Parameters[PITCH_COARSE] = PARAMADC[ADC_COARSE];
	Fish.Parameters[PITCH_CHORD] = ~PARAMADC[ADC_CHORD];

	Fish.Parameters[AMP_ATTACK] = PARAMADC[ADC_ATTACK];
	Fish.Parameters[AMP_DECAY] = PARAMADC[ADC_DECAY];
	Fish.Parameters[AMP_RELEASE] = PARAMADC[ADC_RELEASE];
	Fish.Parameters[AMP_SUSTAIN] = PARAMADC[ADC_SUSTAIN];


	Fish.Parameters[FILTER_CUTOFF] = ~PARAMADC[ADC_CUTOFF];
	Fish.Parameters[FILTER_RESONANCE] = PARAMADC[ADC_RESONANCE];

	Fish.Parameters[FILTER_ENVELOPE] = PARAMADC[ADC_FILTERENVELOPE];
	Fish.Parameters[FILTER_ATTACK] = PARAMADC[ADC_FILTERATTACK];
	Fish.Parameters[FILTER_DECAY] = PARAMADC[ADC_FILTERDECAY];
	Fish.Parameters[FILTER_KEYTRACK] = PARAMADC[ADC_KEYTRACK];
	Fish.Parameters[FILTER_DRIFT] = ~PARAMADC[ADC_DRIFT];
	Fish.Parameters[FILTER_PEAKS] = ~PARAMADC[ADC_PEAKS];
	Fish.Parameters[FILTER_DRIVE] = PARAMADC[ADC_DRIVE];
	Fish.Parameters[FILTER_ACCENT] = PARAMADC[ADC_ACCENT];



	int gate =0;
	if (ACCENT_TRIGGER_GetVal(ACCENT_TRIGGER_DeviceData) == FALSE);
	if (GATE_BUTTON_GetVal(GATE_BUTTON_DeviceData) == FALSE || GATE_TRIGGER_GetVal(GATE_TRIGGER_DeviceData) == FALSE)
	{
		gate = 1;
		LEDS[TRIGGERBUTTONLED] = 255;
		LEDTARGET[TRIGGERBUTTONLED] = 255;
		Fish.Gates[FISHGATE_GATE] = 1;

	}
	else
	{
		Fish.Gates[FISHGATE_GATE] = 0;
		Fish.Gates[FISHGATE_ACCENT] = 0;
		LEDTARGET[TRIGGERBUTTONLED] = 0;
		gate = 0;

	}
		BigFish_GenerateBlock(&Fish, In, Out1,Out2,len);
//	for(int i =0 ;i<len;i++) Out1[i] = Out2[i] = 0;
	LEDS[ENVELOPELED] = LEDTARGET[ENVELOPELED] = Fish.AmpEnvelope.LinearOutput >> (ENVFIXEDBITS - 8) ;
}


int adcmux = 0;


void ADCMUXING()
{
	Measuring = 0;
	uint16_t newvals[3];
	ADMUXED_GetValue16(newvals);
	PARAMADC[adcmux * 3 + 2] = newvals[0];
	PARAMADC[adcmux * 3 + 1] = newvals[1];
	PARAMADC[adcmux * 3 + 0] = newvals[2];

	adcmux = (adcmux + 1)&7;


	if ((adcmux &4)>0)
	{
		ADCSELA_SetVal(ADCSELA_DeviceData);
	}
	else
	{
		ADCSELA_ClrVal(ADCSELA_DeviceData);
	}

	if ((adcmux &2)>0)
	{
		ADCSELB_SetVal(ADCSELB_DeviceData);
	}
	else
	{
		ADCSELB_ClrVal(ADCSELB_DeviceData);
	}

	if ((adcmux &1)>0)
	{
		ADCSELC_SetVal(ADCSELC_DeviceData);
	}
	else
	{
		ADCSELC_ClrVal(ADCSELC_DeviceData);
	}



}

int Tick10Khz =0 ;
struct denoise_state_t filterbutton_state = {0};

void Timer10khz()
{

	int filterbut = FILTER_BUTTON_GetVal( FILTER_BUTTON_DeviceData);
	denoise(filterbut, &filterbutton_state);

	if (pressed(&filterbutton_state))
	{
		LEDTARGET[FILTERBUTTONLED] = LEDS[FILTERBUTTONLED] = 255;
		Fish.Parameters[FILTER_TYPE] =(Fish.Parameters[FILTER_TYPE]  +1)% __FILTERTYPE_COUNT;
	}
	else
	{
		LEDTARGET[FILTERBUTTONLED] = 0;
	}

	for (int i =0 ;i<7;i++)
	{
		LEDTARGET[SHAPELEDS[i]] = 0;
	}

	SteppedResult_t sr;
	GetSteppedResult(Fish.Parameters[OSC_SHAPE], 6, &sr);
	LEDTARGET[SHAPELEDS[sr.index]] = 255 - sr.fractional;
	LEDTARGET[SHAPELEDS[sr.index+1]] =   sr.fractional;

	for (int i =0 ;i<5;i++)
	{
		if (i == Fish.Parameters[FILTER_TYPE])
		{
			LEDTARGET[FILTERLEDS[i]] = 255;
		}
		else
		{
			LEDTARGET[FILTERLEDS[i]] = 0;
		};
	}

	//	LEDTARGET[TRIGGERBUTTONLED] = (((Tick10Khz/10000)%3)==1)?255:0;
	//LEDTARGET[FILTERBUTTONLED] = (((Tick10Khz/10000)%3)==2)?255:0;
	//LEDTARGET[ENVELOPELED] = (((Tick10Khz/10000)%3)==0)?255:0;

	//for(int i =0 ;i<BIGFISH_LEDS;i++)
	//{
	//	LEDTARGET[i] = PARAMADC[i]>>8;
	//}

	UpdateLeds();
	LEDS_Update(LEDS, 16, 0, 0);
	Tick10Khz++;
}


int cppmain(void)
{
	LEDS_InitHardware();
	ak4558_init();

	Measuring = 1;


	BigFish_Init(&Fish, 44100);

	for (int i = 0; i < __PARAMCOUNT; i++)
	{
		Fish.Parameters[i] = 0;
	}
	Fish.Parameters[AMP_ATTACK] = (10 * 65536) / 127;;
	Fish.Parameters[AMP_DECAY] = (10 * 65536) / 127;;

	Fish.Parameters[FILTER_ATTACK] = (10 * 65536) / 127;;
	Fish.Parameters[FILTER_DECAY] = (80 * 65536) / 127;;

	Fish.Parameters[FILTER_TYPE] = 0;
	Fish.Parameters[AMP_SUSTAIN] = (64 * 65536) / 127;
	Fish.Parameters[AMP_RELEASE] = (64 * 65536) / 127;;
	Fish.Parameters[PITCH_COARSE] = (64 * 65536) / 127;;
	Fish.Parameters[PITCH_FINE] = (64 * 65536) / 127;;



	Fish.Gates[FISHGATE_GATE] = 1;
	int N = 70;
	int32_t pitchtarget = ((N-24) * (1 << 14)) / 12;
	Fish.PitchInput = pitchtarget;
	BigFish_Update(&Fish);



	for(;;)
	{
		if (audio_buffers_fresh)
		{
			audio_buffers_fresh = 0;

			int32_t* inbuf = audio_in_buffer;
			int32_t* outbuf = audio_out_buffer;
			NextBlock(inbuf, outbuf);


		}
		switch (Measuring)
		{
		case 1:
			{	ADCMUXING();
				Measuring = 2;
				ADMAIN_Measure(false);
			} break;
		case 3:
			{
				uint16_t values[2];
				ADMAIN_GetValue16(values);
				float pitchvoltage = ((values[0] * 3.3f)/65535.0f) * 1.98f  - 1.0f;
				Fish.PitchInput = (pitchvoltage *  (float)(1<<14));
				Measuring = 0;
				ADMUXED_Measure(false);
			}break;
		}


	}
}

}
