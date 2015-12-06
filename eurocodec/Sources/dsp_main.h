#pragma once

#include <stdint.h>

#include "cv_adc.h"
//#include "dsplib/dpw_osc.h"
//#include "dsplib/cvsmooth.h"
#include "dsplib/zdfsvf.h"

static inline void dsp_work(uint32_t *outl, uint32_t *outr, uint32_t inl, uint32_t inr)
{
	//static struct dpw_sawtooth_state_t state1;
	//static struct dpw_pulse_state_t state2;
	//static struct cvsmooth_state_t pw_smooth_state;

	//uint32_t pulsewidth = cvsmooth(adc_value(2) << 16, &pw_smooth_state);

	int32_t pitch = cv_adc_value() + (adc_value(0)>>1);
	if (pitch > 0x7fff) pitch = 0x7fff;

	*outl = svf(inl, pitch, adc_value(1));//dpw_sawtooth(pitch, &state1);
	*outr = inr;//dpw_pulse(pitch, pulsewidth, &state2);
}
