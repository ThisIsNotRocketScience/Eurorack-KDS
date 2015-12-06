#pragma once

#include <stdint.h>

struct dpw_sawtooth_state_t
{
	uint32_t phase;
	uint32_t lastvalue;
};

struct dpw_pulse_state_t
{
	uint32_t phase;
	uint32_t lastvalue1;
	uint32_t lastvalue2;
};

int32_t dpw_sawtooth(int32_t pitch, struct dpw_sawtooth_state_t* state);
int32_t dpw_pulse(int32_t pitch, uint32_t pulsewidth, struct dpw_pulse_state_t* state);
