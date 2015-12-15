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

static inline int32_t svf_stereo(int32_t* outl,
								 int32_t* outr,
								 int32_t sample,
								 int32_t sample_right,
								 int32_t cutoff,
								 int32_t resonance,
								 int32_t gain)
{
	gain = (gain << 12) - (gain << 10) + 0x04000000;
	int tableindex = ((cutoff) >> 8) + 0x80;
	int tablefract = cutoff & 0xFF;
	int32_t g_bi = g_table[tableindex] +
			((((g_table[tableindex+1]-g_table[tableindex])>>3) * tablefract) >> 5); // /2
	int32_t k = (0x10000-resonance - 1500) * 17000;

	int32_t bottomtmp = (0x10000000 + smull32_hi(g_bi, (g_bi + k)));
	const uint32_t one_hi = 0x04000000;
	uint32_t ootmp = __UDIV64_24(0, one_hi, bottomtmp) << 8;

	// g2pk = k + 2*g_bi
	int32_t g2pk = __QDADD(k, g_bi); // /2

	const int32_t v0 = smull32_hi(sample, gain);
	const int32_t v0_right = smull32_hi(sample_right, gain);

	int32_t tmp1 = smull32_hi(g2pk, v1z); // /2 * /16 * /2 = /64

	int32_t tmp1_right = smull32_hi(g2pk, v1z_right); // /2 * /16 * /2 = /64

	int32_t v0_scaled = v0 >> 3; // /8
	int32_t v0_scaled_right = v0_right >> 3; // /8

	//int32_t tmp2 = v0_scaled + v0z - (tmp1<<3) - v2z; // /8
	int32_t tmp2 = __QADD(v0_scaled, v0z);
	tmp2 = __QSUB(tmp2, tmp1<<3);
	tmp2 = __QSUB(tmp2, v2z); // /8

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

	int32_t v1a = smull32_hi(-v1, v1) << 3;
	int32_t v1b = smull32_hi(v1a, v1) << 3;
	v1 = __QADD(v1, (smull32_hi(v1a + v1b, g_bi) << 5));

	int32_t v1a_right = smull32_hi(-v1_right, v1_right) << 3;
	int32_t v1b_right = smull32_hi(v1a_right, v1_right) << 3;
	v1_right = __QADD(v1_right, (smull32_hi(v1a_right + v1b_right, g_bi) << 5));

	v0z = v0_scaled; // /8
	v1z = v1; // /16

	v0z_right = v0_scaled_right; // /8
	v1z_right = v1_right; // /16

	// v2, v2z /8
	v2z = __SSAT_NOSHIFT(v2, 28);
	v2z_right = __SSAT_NOSHIFT(v2_right, 28);

	*outl = __SSAT_LSL(v2, 31, 3) << 1;
	*outr = __SSAT_LSL(v2_right, 31, 3) << 1;
}

};
#include "fixed.h"
extern "C" {

static fixed_t<int32_t, 2> v0z_f = 0;
static fixed_t<int32_t, 3> v1z_f = 0;
static fixed_t<int32_t, 3> v2z_f = 0;
static fixed_t<int32_t, 2> v0z_right_f = 0;
static fixed_t<int32_t, 3> v1z_right_f = 0;
static fixed_t<int32_t, 3> v2z_right_f = 0;

static inline int32_t svf_stereo_templ(int32_t* outl,
								 int32_t* outr,
								 int32_t sample,
								 int32_t sample_right,
								 int32_t cutoff,
								 int32_t resonance,
								 int32_t gain)
{
	gain = (gain << 14) - (gain << 12) + 0x10000000;
	int tableindex = ((cutoff) >> 8) + 0x80;
	int tablefract = cutoff & 0xFF;
	int32_t g_bi = g_table[tableindex] +
			((((g_table[tableindex+1]-g_table[tableindex])>>3) * tablefract) >> 5); // /2
	int32_t k_bi = (0x10000-resonance - 1500) * 17000;

	int32_t bottomtmp = (0x10000000 + smull32_hi(g_bi, (g_bi + k_bi)));
	const uint32_t one_hi = 0x04000000;
	fixed_t<int32_t, 2> ootmp = __UDIV64_24(0, one_hi, bottomtmp) << 8;

	fixed_t<int32_t, 1> g(g_bi);
	fixed_t<int32_t, 2> k(k_bi);

	// g2pk = k + 2*g_bi
	auto g2pk = (k>>1) + g;

	const fixed_t<int32_t, -1> v0 = smull32_hi(sample, gain);
	const fixed_t<int32_t, -1> v0_right = smull32_hi(sample_right, gain);

	auto tmp1 = g2pk * v1z_f; // /2 * /16 * /2 = /64

	auto tmp1_right = g2pk * v1z_right_f; // /2 * /16 * /2 = /64

	auto v0_scaled = rightscale<3>(v0);
	auto v0_scaled_right = rightscale<3>(v0_right);

	auto tmp2 = v0_scaled + v0z_f;
	tmp2 = tmp2 - tmp1;
	tmp2 = tmp2 - v2z_f;

	auto tmp2_right = v0_scaled_right + v0z_right_f;
	tmp2_right = tmp2_right - tmp1_right;
	tmp2_right = tmp2_right - v2z_right_f;

	auto tmp3 = tmp2 * ootmp;
	auto tmp3_right = tmp2_right * ootmp;

	auto tmp4 = g * tmp3;
	auto tmp4_right = g * tmp3_right;

	auto v1 = v1z_f + tmp4;
	auto v1_right = v1z_right_f + tmp4_right;

	auto tmp5 = v1 + v1z_f;
	auto tmp5_right = v1_right + v1z_right_f;

	auto tmp6 = g * tmp5;
	auto tmp6_right = g * tmp5_right;

	auto v2 = v2z_f + tmp6;
	auto v2_right = v2z_right_f + tmp6_right;

	// distortion:
	//double v1a = -v1*v1 * 0.0625;
	//double v1b = v1a*v1 * 2.0;
	//v1 = v1 + 8.0 * (v1a + v1b) * g;

	auto v1a = (-v1) * v1;
	auto v1b = v1a * v1;
	v1 = v1 + (scaleshift<4> (v1b, -4) + scaleshift<3> (v1b, -3)) * g * 4;

	auto v1a_right = (-v1_right) * v1_right;
	auto v1b_right = v1a_right * v1_right;
	v1_right = v1_right + (scaleshift<4> (v1b_right, -4) + scaleshift<3> (v1b_right, -3)) * g * 4;

	v0z_f = v0_scaled; // /8
	v1z_f = v1; // /16

	v0z_right_f = v0_scaled_right; // /8
	v1z_right_f = v1_right; // /16

	// v2, v2z /8
	v2z_f = __SSAT_NOSHIFT(v2.get(), 28);
	v2z_right_f = __SSAT_NOSHIFT(v2_right.get(), 28);

	*outl = __SSAT_LSL(v2.get(), 29, 3) << 3;
	*outr = __SSAT_LSL(v2_right.get(), 29, 3) << 3;
}
