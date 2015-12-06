#pragma once

#include <stdint.h>

#include "GATE.h"

#include "cv_adc.h"
#include "dsplib/dpw_osc.h"
#include "dsplib/cvsmooth.h"

int dsp_switch_octave = 0;
int dsp_switch_gate = 0;
int dsp_led_octave = 0;
int dsp_led_gate = 0;

int gate = 0;

static inline void dsp_work(uint32_t *outl, uint32_t *outr, uint32_t inl, uint32_t inr)
{
	static struct dpw_sawtooth_state_t state1;
	static struct dpw_pulse_state_t state2;
	static struct cvsmooth_state_t pw_smooth_state;

	uint32_t pulsewidth = cvsmooth(adc_value(2) << 16, &pw_smooth_state);

	int32_t pitch = cv_adc_value() + (adc_value(0) >> 2);

	if (dsp_switch_octave) pitch += 1985; // 1V*.2/3.3V*32768
	pitch += 2*1985; // scale pitch to 1/4 samplerate

	*outl = dpw_sawtooth(pitch, &state1);
	*outr = dpw_pulse(pitch, pulsewidth, &state2);

	if (dsp_switch_gate || !GATE_GetVal(GATE_DeviceData)) {
		gate = 1500;
	}

	dsp_led_octave = dsp_switch_octave;

	if (gate) {
		dsp_led_gate = 1;
		gate--;
	}
	else {
		dsp_led_gate = 0;
	}
}
