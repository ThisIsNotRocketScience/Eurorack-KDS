#pragma once

#include <stdint.h>
#include "fixed.hpp"
#include "intrinsic.h"
#include "svf_tables.h"

static fixed_t<int32_t, 4> v0z = 0;
static fixed_t<int32_t, 4> v1z = 0;
static fixed_t<int32_t, 4> v2z = 0;
static fixed_t<int32_t, 4> v0z_right = 0;
static fixed_t<int32_t, 4> v1z_right = 0;
static fixed_t<int32_t, 4> v2z_right = 0;

static inline int32_t svf_stereo(int32_t* outl,
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
	int32_t k_bi = (0x10000-resonance - 2500) * 17000;

	int32_t bottomtmp = (0x10000000 + smull32_hi(g_bi, (g_bi + k_bi)));
	const uint32_t one_hi = 0x04000000;
	fixed_t<int32_t, 2> ootmp = __UDIV64_24(0, one_hi, bottomtmp) << 8;

	fixed_t<int32_t, 1> g(g_bi);
	fixed_t<int32_t, 2> k(k_bi);

	// g2pk = k + 2*g_bi
	auto g2pk = (k>>1) + g;

	const fixed_t<int32_t, 0> v0 = smull32_hi(sample, gain);
	const fixed_t<int32_t, 0> v0_right = smull32_hi(sample_right, gain);

	auto tmp1 = g2pk * v1z; // /2 * /16 * /2 = /64

	auto tmp1_right = g2pk * v1z_right; // /2 * /16 * /2 = /64

	auto v0_scaled = rightscale<5>(v0);
	auto v0_scaled_right = rightscale<5>(v0_right);

	auto tmp2 = v0_scaled + v0z;
	tmp2 = tmp2 - tmp1;
	tmp2 = tmp2 - v2z;

	auto tmp2_right = v0_scaled_right + v0z_right;
	tmp2_right = tmp2_right - tmp1_right;
	tmp2_right = tmp2_right - v2z_right;

	auto tmp3 = tmp2 * ootmp;
	auto tmp3_right = tmp2_right * ootmp;

	auto tmp4 = g * tmp3;
	auto tmp4_right = g * tmp3_right;

	auto v1 = v1z + tmp4;
	auto v1_right = v1z_right + tmp4_right;

	auto tmp5 = v1 + v1z;
	auto tmp5_right = v1_right + v1z_right;

	auto tmp6 = g * tmp5;
	auto tmp6_right = g * tmp5_right;

	auto v2 = v2z + tmp6;
	auto v2_right = v2z_right + tmp6_right;

	// distortion:
	//double v1a = -v1*v1 * 0.0625;
	//double v1b = v1a*v1 * 2.0;
	//v1 = v1 + 8.0 * (v1a + v1b) * g;

	auto v1a = (-v1) * v1;
	auto v1b = v1a * v1;
	auto v1_dist = (scaleshift<4> (v1b, -4) + scaleshift<3> (v1b, -3)) * g * 8;
	v1 = v1 + v1_dist;

	auto v1a_right = (-v1_right) * v1_right;
	auto v1b_right = v1a_right * v1_right;
	auto v1_dist_right = (scaleshift<4> (v1b_right, -4) + scaleshift<3> (v1b_right, -3)) * g * 8;

	v0z = v0_scaled; // /8
	v1z = __SSAT_NOSHIFT((v1 + v1_dist).get(), 30);

	v0z_right = v0_scaled_right; // /8
	v1z_right = __SSAT_NOSHIFT((v1_right + v1_dist_right).get(), 30);


	auto v2a = (-v2) * v2;
	auto v2b = v2a * v2;
	auto v2_dist = (scaleshift<4> (v2b, -4) + scaleshift<3> (v2b, -3)) * g * 8;
	//v2_dist = v2_dist + v2_dist;
	//v2_dist = v2_dist + v2_dist;
	//v2_dist = v2_dist + v2_dist;
	v2z = __SSAT_NOSHIFT((v2 + v2_dist).get(), 30);

	auto v2a_right = (-v2_right) * v2_right;
	auto v2b_right = v2a_right * v2_right;
	auto v2_dist_right = (scaleshift<4> (v2b_right, -4) + scaleshift<3> (v2b_right, -3)) * g * 8;
	//v2_dist_right = v2_dist_right + v2_dist_right;
	//v2_dist_right = v2_dist_right + v2_dist_right;
	//v2_dist_right = v2_dist_right + v2_dist_right;
	v2z_right = __SSAT_NOSHIFT((v2_right + v2_dist_right).get(), 30);

	fixed_t<int32_t, 0> out_left = v2;
	fixed_t<int32_t, 0> out_right = v2_right;

	*outl = __SSAT_NOSHIFT(out_left.get(), 31) << 1;
	*outr = __SSAT_NOSHIFT(out_right.get(), 31) << 1;
}
