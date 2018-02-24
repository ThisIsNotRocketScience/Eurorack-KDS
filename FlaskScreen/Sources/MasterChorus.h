#ifndef MASTERCHORUSEFFECT
#define MASTERCHORUSEFFECT

#include <stdint.h>
#include <math.h>

//__inline int zOSF2I(double d) { return (int)d;};
#include "waveguide.h"

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


typedef struct Flanger_t
{
	unsigned int phase;
	unsigned int dphase;
	int sr;
	WaveGuide_t Line1;
	WaveGuide_t Line2;
	float mindelay;
	float chorusdepth;
	float Feedback;
	int mindelayafterprocess;
	int monoIn;
	float mChorusMix;
	unsigned int mChorusShift;
} Flanger_t;

extern void InitChorus(StereoChorus_t *ch);
extern void ProcessChorus(StereoChorus_t *ch,int32_t *in, int32_t *out);


extern void Chorus_SetMix(StereoChorus_t *ch, float I);
extern void Chorus_SetSpeed(StereoChorus_t *ch, float I);
extern void Chorus_SetShift(StereoChorus_t *ch, float I);


extern void InitFlanger(Flanger_t *fl);
extern void Flanger_SetMix(Flanger_t *fl, float I);
extern void Flanger_SetSpeed(Flanger_t *fl, float I);
extern void Flanger_SetShift(Flanger_t *fl, float I);
extern void Flanger_SetFeedback(Flanger_t *fl, float I);
extern void ProcessFlanger(Flanger_t *fl,int32_t *in, int32_t *out);


#endif
