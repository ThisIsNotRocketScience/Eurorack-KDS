#include "dsp_main.h"

static float value = 0.0;

void dsp_work(uint32_t *outl, uint32_t *outr, uint32_t inl, uint32_t inr)
{
    value = value + (1000.0f/187500.0f);
    if (value > 1.0) value = 0.0;
    *outl = 0x80000000 + (int32_t)(value * (float)(1ULL<<31ULL));
    *outr = *outl;
}
