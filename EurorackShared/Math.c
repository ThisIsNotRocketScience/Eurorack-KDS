#include <stdint.h>
#include "EurorackShared.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/// A sine approximation via a fourth-order cosine approx.
	/// @param x   angle (with 2^15 units/circle)
	/// @return     Sine value (Q12)
	int32_t isin_S4(int32_t x)
	{
		int c, x2, y;
		static const int qN = 13, qA = 12, B = 19900, C = 3516;

		c = x << (30 - qN);              // Semi-circle info into carry.
		x -= 1 << qN;                 // sine -> cosine calc

		x = x << (31 - qN);              // Mask with PI
		x = x >> (31 - qN);              // Note: SIGNED shift! (to qN)
		x = x*x >> (2 * qN - 14);          // x=x^2 To Q14

		y = B - (x*C >> 14);           // B - x^2*C
		y = (1 << qA) - (x*y >> 16);       // A - x^2*(B-x^2*C)

		return c >= 0 ? y : -y;
	}


	int32_t Sine(uint32_t phase)
	{
		int32_t P = phase >> 17;
		return isin_S4(P) << 17;

	}

	int32_t SawTooth(uint32_t phase)
	{
		return (*(int32_t*)&phase) >> 2;
	}

	int32_t Pulse(uint32_t phase)
	{
		if (phase & 0x80000000)
		{
			return INT32_MIN >> 2;
		}
		else
		{
			return INT32_MAX >> 2;
		}
	}

	int32_t Triangle(uint32_t phase)
	{
		if (phase & 0x80000000)
		{
			return (~(*(int32_t*)&(phase)) - 0x40000000) >> 1;
		}
		else
		{
			return ((*(int32_t*)&(phase)) - 0x40000000) >> 1;
		}
	}

	int32_t LERP(int32_t *V, int total, int fade)
	{
		int T = fade * total;
		unsigned char frac = T & 0xff;
		if (frac & frac < 255) frac += 1;
		int I = T >> 8;
		return ((V[I] >> 8) *(255 - frac) + (V[I + 1] >> 8) * frac);
	}

	int32_t BasicShapes(uint32_t phase, int mod)
	{
		int32_t O[4];
		O[0] = Sine(phase);
		O[1] = SawTooth(phase);
		O[2] = Triangle(phase);
		O[3] = Pulse(phase);
		return LERP(O, 3, mod);
	}

#ifdef __cplusplus
}
#endif
