#pragma once

#include <stdint.h>

typedef struct FMOsc_t
{
	uint32_t p1;
	uint32_t p2;
	uint32_t dp1;
	uint32_t dp2;
	int32_t Amt;
} FMOsc_t;

void FMOsc_Init(FMOsc_t *osc);
void FMOsc_Update(FMOsc_t *osc, float odsr, float centerfreq, int size, int spread);
int32_t FMOsc_Get(FMOsc_t *osc);