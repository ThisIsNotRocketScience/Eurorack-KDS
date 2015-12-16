#pragma once

#include <stdint.h>
#include "fixed.hpp"
#include "intrinsic.h"
#include "svf_tables.h"

static fixed_t<int32_t, 5> v0z = 0;
static fixed_t<int32_t, 4> v1z = 0;
static fixed_t<int32_t, 6> v2z = 0;
static fixed_t<int32_t, 5> v0z_right = 0;
static fixed_t<int32_t, 4> v1z_right = 0;
static fixed_t<int32_t, 6> v2z_right = 0;

static inline int32_t svf_stereo(int32_t* outl,
								 int32_t* outr,
								 int32_t sample,
								 int32_t sample_right,
								 int32_t cutoff,
								 int32_t resonance,
								 int32_t gainparam,
								 int32_t model,
								 int32_t moder)
{
	const fixed_t<int32_t, 6> gain = (gainparam << 10) + 0x800000;

	const int tableindex = ((cutoff & 0xFF00) >> 7) + 0x100;
	const int tablefract = cutoff & 0xFF;
	const int32_t g_bi = g_table[tableindex] +
			((g_table[tableindex+1] * tablefract) >> 5); // /2
	const int32_t k_bi = (0x10000-resonance - 2000) * 11500;

	const int32_t bottomtmp = (0x10000000 + smmul(g_bi, (g_bi + k_bi)));
	const uint32_t one_hi = 0x04000000;
	const fixed_t<int32_t, 8+2> ootmp = __UDIV64_16(0, one_hi, bottomtmp);

	const fixed_t<int32_t, 1> g(g_bi);
	const fixed_t<int32_t, 3> k(k_bi);

	// g2pk = k + 2*g_bi
	const auto g2pk = g + (k>>1);

	const auto v0 = fixed_t<int32_t, -1> (sample) * gain;
	const auto v0_right = fixed_t<int32_t, -1> (sample_right) * gain;

	const auto tmp1 = mac(v2z, g2pk, v1z);
	const auto tmp2 = v0 + v0z - tmp1;

	const auto tmp1_right = mac(v2z_right, g2pk, v1z_right);
	const auto tmp2_right = v0_right + v0z_right - tmp1_right;

	v0z = v0; // /8
	v0z_right = v0_right; // /8

	const auto tmp3 = tmp2 * ootmp;
	const auto tmp3_right = tmp2_right * ootmp;

	const auto v1 = mac(v1z, g, leftscale<7+8>(tmp3));
	const auto v1_right = mac(v1z_right, g, leftscale<7+8>(tmp3_right));

	const auto tmp5 = v1 + v1z;
	const auto tmp5_right = v1_right + v1z_right;

	const auto v2 = mac(v2z, g, tmp5);
	const auto v2_right = mac(v2z_right, g, tmp5_right);

	// distortion:
	//double v1a = v1*v1 * 0.0625;
	//double v1b = v1a*v1 * 2.0;
	//v1 = v1 - 8.0 * (v1a + v1b) * g;

	const auto v1a = v1 * v1;
	const auto v1b = v1a * v1;
	const auto v1_dist = (scaleshift<4> (v1a, -4) + scaleshift<3> (v1b, -3)) * g * 16;

	const auto v1a_right = v1_right * v1_right;
	const auto v1b_right = v1a_right * v1_right;
	const auto v1_dist_right = (scaleshift<4> (v1a_right, -4) + scaleshift<3> (v1b_right, -3)) * g * 16;

	v1z = v1 - v1_dist;
	v1z_right = v1_right - v1_dist_right;

	const auto v2a = v2 * v2;
	const auto v2b = v2a * v2;
	const auto v2_dist = (scaleshift<4> (v2a, -4) + scaleshift<3> (v2b, -3)) * g * 16;

	const auto v2a_right = v2_right * v2_right;
	const auto v2b_right = v2a_right * v2_right;
	const auto v2_dist_right = (scaleshift<4> (v2a_right, -4) + scaleshift<3> (v2b_right, -3)) * g * 16;

	v2z = v2 - v2_dist;
	v2z_right = v2_right - v2_dist_right;

	fixed_t<int32_t, 0> out_left;
	fixed_t<int32_t, 0> out_right;
	switch (model) {
	default:
	case 0:
		out_left = v2;
		break;
	case 1:
		out_left = v1;
		break;
	case 2:
		out_left = v0 - (v2 + k * v1);
		break;
	}
	switch (moder) {
	default:
	case 0:
		out_right = v2_right;
		break;
	case 1:
		out_right = v1_right;
		break;
	case 2:
		out_right = (v0_right<<1) - (v2_right + k * v1_right);
	}

	*outl = (out_left + out_left).get();
	*outr = (out_right + out_right).get();
}
