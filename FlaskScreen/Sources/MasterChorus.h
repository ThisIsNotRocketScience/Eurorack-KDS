#ifndef MASTERCHORUSEFFECT
#define MASTERCHORUSEFFECT

#define MAX_WG_DELAY (10000/2)
#include <stdint.h>
#include <math.h>

__inline int zOSF2I(double d) { return (int)d;};




typedef struct WaveGuide_t
{
	int16_t buffer[MAX_WG_DELAY];
	int counter;
} WaveGuide_t;

typedef struct StereoChorus_t
{
	unsigned int phase;
	unsigned int dphase;
	int sr;
	WaveGuide_t Line1;
	WaveGuide_t Line2;
	float mindelay;
	float chorusdepth;

	int mindelayafterprocess;
	int monoIn;
	float mChorusMix;
	unsigned int mChorusShift;
} StereoChorus_t;


extern void InitChorus(StereoChorus_t *ch);
extern void ProcessChorus(StereoChorus_t *ch,int32_t *in, int32_t *out);


extern void Chorus_SetMix(StereoChorus_t *ch, float I);
extern void Chorus_SetSpeed(StereoChorus_t *ch, float I);
extern void Chorus_SetShift(StereoChorus_t *ch, float I);

#endif
