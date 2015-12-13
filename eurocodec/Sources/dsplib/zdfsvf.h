#pragma once

#include <stdint.h>
#include "intrinsic.h"

#include "svf_tables.h"

static int32_t v0z = 0;
static int32_t v1z = 0;
static int32_t v2z = 0;
static int32_t v0z_right = 0;
static int32_t v1z_right = 0;
static int32_t v2z_right = 0;

#define OPT 1

static inline int32_t svf_stereo(int32_t* outl, int32_t* outr, int32_t sample, int32_t sample_right, int32_t cutoff, int32_t resonance)
{
	int tableindex = ((cutoff) >> 8) + 0x80;
	int tablefract = cutoff & 0xFF;
	int32_t g_bi = g_table[tableindex] +
			((((g_table[tableindex+1]-g_table[tableindex])>>3) * tablefract) >> 5); // /2
	int32_t k = (0x10000-resonance - 500) * 27000;

	int32_t bottomtmp = (0x10000000 + smull32_hi(g_bi, (g_bi + k)));
#if OPT == 0
	const uint64_t one = 0x0400000000000000;
	int32_t ootmp = (int32_t)((int32_t)(one / (int64_t)bottomtmp)); // /4
#else
	const uint32_t one_hi = 0x04000000;
	uint32_t ootmp = __UDIV64(0, one_hi, bottomtmp) << 8;
#endif

#if OPT == 0
	int32_t g2pk = (g_bi<<1) + k; // /2
#else
	int32_t g2pk = __QDADD(k, g_bi); // /2
#endif

	const int32_t v0 = sample>>3;
	const int32_t v0_right = sample_right>>3;

	int32_t tmp1 = smull32_hi(g2pk, v1z); // /2 * /16 * /2 = /64

	int32_t tmp1_right = smull32_hi(g2pk, v1z_right); // /2 * /16 * /2 = /64

	int32_t v0_scaled = v0 >> 2; // /8
	int32_t v0_scaled_right = v0_right >> 2; // /8

	int32_t tmp2 = __QADD(v0_scaled, v0z);
	tmp2 = __QSUB(tmp2, tmp1<<3);
	tmp2 = __QSUB(tmp2, v2z); // /8

	//int32_t tmp2_right = v0_scaled_right + v0z_right - (tmp1_right<<3) - v2z_right; // /8
	int32_t tmp2_right = __QADD(v0_scaled_right, v0z_right);
	tmp2_right = __QSUB(tmp2_right, tmp1_right<<3);
	tmp2_right = __QSUB(tmp2_right, v2z_right); // /8

	int32_t tmp3 = smull32_hi(tmp2, ootmp); // /8 * /4 * /2 = /64
	int32_t tmp3_right = smull32_hi(tmp2_right, ootmp); // /8 * /4 * /2 = /64

	int32_t tmp4 = smull32_hi(g_bi, tmp3); // /2 * /64 * /2 = /256
	int32_t tmp4_right = smull32_hi(g_bi, tmp3_right); // /2 * /64 * /2 = /256

	int32_t v1 = __QADD(v1z, tmp4 << 4); // /16
	int32_t v1_right = __QADD(v1z_right, tmp4_right << 4); // /16

	int32_t tmp5 = __QADD(v1, v1z); // /16
	int32_t tmp5_right = __QADD(v1_right, v1z_right); // /16

	int32_t tmp6 = smull32_hi(g_bi, tmp5); // /1 * /16 * /2 = /64
	int32_t tmp6_right = smull32_hi(g_bi, tmp5_right); // /1 * /16 * /2 = /64

	int32_t v2 = __QADD(v2z, tmp6 << 4); // /8
	int32_t v2_right = __QADD(v2z_right, tmp6_right << 4); // /8

	v0z = v0_scaled; // /8
	v1z = v1; // /16

	v0z_right = v0_scaled_right; // /8
	v1z_right = v1_right; // /16

	// v2, v2z /8
	v2z = __SSAT_NOSHIFT(v2, 28);
	v2z_right = __SSAT_NOSHIFT(v2_right, 28);

	*outl = __SSAT_LSL(v2, 32, 3);
	*outr = __SSAT_LSL(v2_right, 32, 3);
}
