#pragma once

#include <stdint.h>

struct cvsmooth_state_t
{
	uint32_t lastvalue;
};

//uint32_t cvsmooth(uint32_t value, struct cvsmooth_state_t* state);

inline uint32_t cvsmooth(uint32_t value, struct cvsmooth_state_t* state)
{
	int32_t diff = (value>>1) - (state->lastvalue>>1);
	if (diff > -0x20000 && diff < 0x20000) {
		return state->lastvalue;
	}

	uint32_t newvalue = state->lastvalue + (diff >> 6);
	state->lastvalue = newvalue;
	return newvalue;
}
