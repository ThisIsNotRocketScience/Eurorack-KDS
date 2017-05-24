#include "OrganOsc.h"
#include <math.h>
#include <stdlib.h>



#define SINETABLE_BITS	11
#define SINETABLE_SIZE	(1 << SINETABLE_BITS)
static uint16_t const SineTable[SINETABLE_SIZE + 1] = {0};

inline int16_t FixedSin(uint32_t const phase)
{
	int i0 = (phase >> (32 - SINETABLE_BITS));
	int16_t s0 = SineTable[i0];
	int16_t s1 = SineTable[i0 + 1];

	uint32_t a = (phase & ((1 << (32 - SINETABLE_BITS)) - 1)) >> ((32 - SINETABLE_BITS) - 16);

	return s0 + ((a * (s1 - s0))>>16);
}




void Organ_Init(struct Organ_t *osc)
{
	osc->mPhase = 0;
	osc->mPhase2 = 0;
	osc->mPhaseIncrement = 0.1f;
	osc->mPhaseIncrement2 = 0.1f;
}

void Organ_Update(struct Organ_t  *osc, int samplerate, float centerfreq, float size, float spread)
{
	float C = centerfreq / (float)samplerate;
	osc->mPhaseIncrement = C;
	osc->mPhaseIncrement2 = (3.5) / samplerate;
	float Spr = spread / 65536.0;
	Spr *= Spr;
	osc->spread = (Spr* centerfreq) / (330.0f);
}

float Organ_Get(struct  Organ_t  *osc)
{
	osc->mPhase += osc->mPhaseIncrement;
	while (osc->mPhase >= 1.0)
	{
		osc->mPhase -= 1.0;
	}
	float M1 = 6.283f;
	float L1 = 1.0;
	float M2 = M1 * 2;
	float L2 = 1.0;
	float M3 = M1 * 4;
	float L3 = 1.0;
	float M4 = M1 * 8;
	float L4 = 1.0;

	osc->mPhase2 += osc->mPhaseIncrement2;
	while (osc->mPhase2 >= 1.0)
	{
		osc->mPhase2 -= 1.0;
	}
	float P2 = osc->mPhase2 * 6.283f;
	float Aoff = sin(P2)*osc->spread;
	float Boff = sin(P2 + 6.283 / 3)*osc->spread;
	float Coff = sin(P2 + (6.283*2.0) / 3)*osc->spread;

	float AP = osc->mPhase + Aoff;
	float BP = osc->mPhase + Boff;
	float CP = osc->mPhase + Coff;
	float total = L1 + L2 + L3 + L4;
	float A = sin(AP * M1) * L1 + sin(AP * M2) * L2 + sin(AP * M3) * L3 + sin(AP * M4) * L4;
	float B = sin(BP * M1) * L1 + sin(BP * M2) * L2 + sin(BP * M3) * L3 + sin(BP * M4) * L4;
	float C = sin(CP * M1) * L1 + sin(CP * M2) * L2 + sin(CP * M3) * L3 + sin(CP * M4) * L4;
	return (A + B + C) / (total *3);
}
