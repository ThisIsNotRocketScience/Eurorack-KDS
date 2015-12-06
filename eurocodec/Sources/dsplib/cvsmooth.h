#pragma once

#include <stdint.h>

struct cvsmooth_state_t
{
	uint32_t lastvalue;
};

uint32_t cvsmooth(uint32_t value, struct cvsmooth_state_t* state);
