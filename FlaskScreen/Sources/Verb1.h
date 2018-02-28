#ifndef VERB1
#define VERB1
#include <stdint.h>

#include "waveguide.h"
typedef struct Verb1
{
	WaveGuide_t Line1;
	WaveGuideShort_t Line2;
	WaveGuideShort_t Line3;
	float new1;
		float delay1;
	float delay2;
	float delay3;
	int T;
} Verb1;
extern void InitVerb1(Verb1 *V);
extern void ProcessVerb1(Verb1 *V, int32_t *in, int32_t *out);

#endif
