#pragma once

#include <stdint.h>
//#include "intrinsic.h"

uint32_t umull32_hi(uint32_t a, uint32_t b)
{
	return (uint32_t)(((uint64_t)a * (uint64_t)b) >> 32);
}

int32_t smull32_hi(int32_t a, int32_t b)
{
	return (int32_t)(((int64_t)a * (int64_t)b) >> 32);
}

#include "svf_tables.h"

static int32_t v0z = 0;
static int32_t v1z = 0;
static int32_t v2z = 0;

static inline int32_t svf(int32_t sample, int16_t cutoff, uint16_t resonance)
{
	int tableindex = (cutoff + 0x8000) >> 8;
	int tablefract = cutoff & 0xFF;
	int32_t g_bi = g_table[tableindex] +
			((((g_table[tableindex+1]-g_table[tableindex])>>3) * tablefract) >> 5); // /2
	int32_t k = (0x10000-resonance - 1500) * 17000;

	const uint64_t one = 0x0400000000000000;
	int32_t bottomtmp = (0x10000000 + smull32_hi(g_bi, (g_bi + k)));
	int32_t ootmp = (int32_t)((int32_t)(one / (int64_t)bottomtmp)); // /4

	const int32_t v0 = sample>>3;

	int32_t g2pk = (g_bi>>0) + (k>>1); // /4
	int32_t tmp1 = smull32_hi(g2pk, v1z); // /4 * /16 * /2 = /128

	int32_t v0_scaled = v0>>3; // /8
	int32_t tmp2 = v0_scaled + v0z - (tmp1<<4) - v2z; // /8

	int32_t tmp3 = smull32_hi(tmp2, ootmp); // /8 * /4 * /2 = /64
	int32_t tmp4 = smull32_hi(g_bi, tmp3); // /2 * /64 * /2 = /256

	int32_t v1 = v1z + (tmp4 << 4); // /16

	int32_t tmp5 = v1 + v1z; // /16
	int32_t tmp6 = smull32_hi(g_bi, tmp5) << 1; // /1 * /16 * /2 = /64
	int32_t v2 = v2z + (tmp6 << 3); // /8

	v0z = v0_scaled; // /8
	v1z = v1; // /16

	// v2, v2z /8
	if (v2 < -0x4000000) v2z = -0x4000000;
	else if (v2 > 0x4000000) v2z = 0x4000000;
	else v2z = v2;

	if (v2 <= -0x10000000) return -0x7fffffff;
	else if (v2 >= 0x10000000) return 0x7fffffff;
	else return v2*8;
}
/*
double g = tan(PI * pitch);
double k = _params.Resonance(); // 1/q

float *input = psamples;
float *lp = _lp;
float *hp = _hp;
float *bp = _bp;

double ootmp = 1.0 / (1 + g*(g + k));

int n = numsamples;
while (n--) {
	double v0 = (*input++) * (1.0 / 32768.0);
	double v1 = v1z + g * (v0 + v0z - 2*(g + k)*v1z - 2*v2z) * ootmp;
	double v2 = v2z + g * (v1 + v1z);

	*lp++ = v2 * 32768.0;
	*bp++ = v1 * 32768.0;
	*hp++ = (v0 - k*v1 - v2) * 32768.0;

	// distortion
	double v1a = -v1*v1 * 0.0625;
	double v1b = v1a*v1 * 2.0;
	v1 = v1 + 8.0 * (v1a + v1b) * g;

	v0z = v0;
	v1z = clamp10(v1);
	v2z = clamp10(v2);
}
*/
