#include "dpw_osc.h"

#include "intrinsic.h"
#include "dpw_pitchtables.h"

int32_t dpw_sawtooth(int32_t pitch, struct dpw_sawtooth_state_t* state)
{
	pitch += 0x8000;

	if (pitch < 0) pitch = 0;
	if (pitch > 65535) pitch = 65535;
	uint32_t phaseadd = umull32_hi(phaseadd_hi[(pitch & 0xFF00) >> 8], phaseadd_lo[pitch & 0xFF]);
	uint32_t phase = state->phase;
	phase += phaseadd;
	int32_t value = smull32_hi(phase, phase);

	int32_t level =  ((1UL << 31UL)-1) / (phaseadd>>14);
	if (level > 0x7FFFFFF) {
		level = 0x7FFFFFF;
	}

	int32_t diff = value - state->lastvalue;
	state->lastvalue = value;
	state->phase = phase;

	return smull32_hi(diff, level << 4) << 9;
}

int32_t dpw_pulse(int32_t pitch, uint32_t pulsewidth, struct dpw_pulse_state_t* state)
{
	static uint32_t last_pulsewidth = 0;
	pitch += 0x8000;
	if (pitch < 0) pitch = 0;
	if (pitch > 65535) pitch = 65535;

	uint32_t phaseadd = umull32_hi(phaseadd_hi[(pitch & 0xFF00) >> 8], phaseadd_lo[pitch & 0xFF]);
	uint32_t phase = state->phase;
	phase += phaseadd;
	uint32_t phase2 = phase + pulsewidth;
	int32_t value1 = smull32_hi(phase, phase);
	int32_t value2 = smull32_hi(phase2, phase2);

	// multiplier lsb bits zero to limit max level
	int32_t level1 = (0x70000000) / (phaseadd>>14);
	int32_t level2 = (0x70000000) / ((phaseadd+(((pulsewidth>>1)-(last_pulsewidth>>1))<<1))>>14);
	last_pulsewidth = pulsewidth;
	if (level1 > 0x7FFFFFF) {
		level1 = 0x7FFFFFF;
	}
	if (level2 > 0x7FFFFFF) {
		level2 = 0x7FFFFFF;
	}

	int32_t diff1 = value1 - state->lastvalue1;
	int32_t diff2 = value2 - state->lastvalue2;

	int32_t saw1 = smull32_hi(diff1, level1 << 3);
	int32_t saw2 = smull32_hi(diff2, level2 << 3);

	state->phase = phase;
	state->lastvalue1 = value1;
	state->lastvalue2 = value2;

	return (saw1 - saw2) << 14;
}

