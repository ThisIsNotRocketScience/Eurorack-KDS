#include "Verb1.h"
#include "ak4558.h"
extern int32_t Param[4];

extern const float const dsintab[2048];
void SetupL(Verb1 *V)
{
	V->T+=4;
		V->new1= (10+ ((Param[1]* (MAX_WG_DELAY-10))/(65536.0)));
		V->delay1 = V->delay1*0.999 + V->new1 * 0.001;
		V->delay2 = V->delay1 * (.50f/1.18f) + dsintab[(V->T+(2048/2))%2048]*10.32f;
		V->delay3 = V->delay1 * (.50f/1.58f) + dsintab[(V->T+(0))%2048]*10.5;
}

void InitVerb1(Verb1 *V)
{
	V->T = 0;
	WaveGuide_Init(&V->Line1);
	WaveGuideShort_Init(&V->Line2);
	WaveGuideShort_Init(&V->Line3);
	SetupL(V);
	V->delay1 = V->new1;
	SetupL(V);

}

void ProcessVerb1(Verb1 *V, int32_t *in, int32_t *out)
{
	SetupL(V);
	uint16_t  wet= Param[0];
	uint16_t dry = 0xffff-wet;
	uint16_t feedback = Param[2]/2;
	for (int i =0 ;i<AUDIO_BUFFER_SIZE;i++)
	{
		int32_t lin = (*in++)>>16;
		int32_t rin = (*in++)>>16;

		int32_t monoin =  (rin + lin)/2;

		int32_t l1 = WaveGuide_GetI16(&V->Line1, V->delay1);
		int32_t l2 = WaveGuideShort_GetI16(&V->Line2, V->delay3);
		int32_t l2b = WaveGuideShort_GetI16(&V->Line3, V->delay2)
				;
		int32_t l3 = WaveGuideShort_GetI16(&V->Line3, V->delay3);
		int32_t l3b = WaveGuideShort_GetI16(&V->Line2, V->delay2);

		int32_t res = l1 + l2b + l3b;
		int32_t resL = l2 + l2b-l1;
		int32_t resR = l3 + l3b+l1;
		int32_t diff = resR - resL;
		resL -= diff/10;
		resR += diff/10;

		WaveGuide_Push(&V->Line1, (monoin<<16)  +( (-res) * feedback));
		WaveGuideShort_Push(&V->Line2, (rin<<16)   +( (-l2-resL) * feedback));
		WaveGuideShort_Push(&V->Line3, (lin<<16)  +( (-l3 - resR) * feedback));

		*out++ = (monoin * dry) + (resR * wet );
		*out++ = (monoin * dry) + (resL * wet) ;
	}

}
