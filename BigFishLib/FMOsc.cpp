
#include "BigFish.h" 

void FMOsc_Init(FMOsc_t *osc)
{

}

void FMOsc_Update(FMOsc_t *osc, float odsr, float centerfreq, int size, int spread)
{
	float cfodsr = centerfreq * odsr ;
	
	BigFish_SteppedResult_t sr;
	
	float mul[8] = {1,pow(2.0, 2.0/12.0),pow(2.0, 4.0 / 12.0), pow(2.0, 5.0 / 12.0), pow(2.0, 7.0 / 12.0), pow(2.0, 9.0 / 12.0), pow(2.0, 12 / 12.0), pow(2.0, 12 / 12.0)};
	
	BigFish_GetSteppedResult(spread, 7, &sr);

	float ratio = BigFish_GetInterpolatedResultFloat(mul, &sr);
	
	osc->dp1 = (unsigned long)((65536.f*65536.f) * (cfodsr));;
	osc->dp2 = (unsigned long)((65536.f*65536.f) * (cfodsr * ratio));
	osc->Amt = size>>2;
}

extern int16_t FixedSin(uint32_t const phase);

int32_t FMOsc_Get(FMOsc_t *osc)
{
	osc->p2 += osc->dp2;
	osc->p1 += osc->dp1 + FixedSin(osc->p2)* osc->Amt;
	return FixedSin(osc->p1);
}