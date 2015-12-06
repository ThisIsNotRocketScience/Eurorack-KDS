
#include "GoldfishGlobals.h"

#ifndef TABLES
#define TABLES
#include "DistortionTable_1.h"
#include "DistortionTable_2.h"
#endif

FISHTYPE Fish;
PlatinumClip Dist;


#include "GATE.h"
extern "C"{

#include "../cv_adc.h"
#include "../dsplib/dpw_osc.h"
#include "../dsplib/cvsmooth.h"

extern int dsp_switch_octave ;
extern int dsp_switch_gate ;
extern int dsp_led_octave ;
extern int dsp_led_gate;

extern int gate ;
}

static struct dpw_sawtooth_state_t state1;
static struct dpw_pulse_state_t state2;
static struct cvsmooth_state_t pw_smooth_state;


#include "Goldfish_Interface.h"

extern "C"{
	inline void GoldfishInit()
	{
		for (int i = 0; i < MAXFISH; i++)
		{
			Fish.Event(ALL_NOTES_OFF, 0, 0);

			Fish.Event(CUTOFF, 40, 0);
			Fish.Event(RESONANCE, 90, 0);
			Fish.Event(FENV, 80, 0);
			Fish.Event(FDEC, 80, 0);
			Fish.Event(WAVEFORM, 0, 0);
			Fish.Event(GLIDE, 4, 0);
		}
	}
	void GoldfishProcess(int32_t *inputsamples, int32_t * outputsamples, int32_t frames)
	{

		int32_t pitch = cv_adc_value() + (adc_value(0) >> 2);

		if (dsp_switch_octave) pitch += 1985; // 1V*.2/3.3V*32768
		pitch += 2*1985; // scale pitch to 1/4 samplerate


		if (dsp_switch_gate || !GATE_GetVal(GATE_DeviceData))
		{
			gate = 1500 / 32;
		}

		dsp_led_octave = dsp_switch_octave;

		if (gate)
		{
			dsp_led_gate = 1;
			gate--;
		}
		else
		{
			dsp_led_gate = 0;
		}




		for (int i =0 ;i<frames;i++)
		{
			*inputsamples++ = 0;
			*inputsamples++ = 0;
			*outputsamples++ = 0;
			*outputsamples++ = 0;

		}


		uint32_t pulsewidth = cvsmooth(adc_value(2) << 16, &pw_smooth_state);


	}
}
