#pragma once

inline uint32_t umull32_hi(uint32_t a, uint32_t b)
{
	return (uint32_t)(((uint64_t)a * (uint64_t)b) >> 32);
}

inline int32_t smull32_hi(int32_t a, int32_t b)
{
	return (int32_t)(((int64_t)a * (int64_t)b) >> 32);
}

