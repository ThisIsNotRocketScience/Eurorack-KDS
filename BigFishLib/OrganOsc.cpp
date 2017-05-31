#include "OrganOsc.h"
#include <math.h>
#include <stdlib.h>

#ifdef WIN32
#define MEMATTR 
#else
#define MEMATTR __attribute__((section(".data")))
#endif


#define SINETABLE_BITS 8
#define SINETABLE_SIZE	(1 << SINETABLE_BITS)
int16_t SineTable[2 * SINETABLE_SIZE + 2] = {
	0,804, 804,803, 1607,802, 2410,801, 3211,799, 4011,796, 4807,793, 5601,790, 6392,786, 7179,782, 7961,777, 8739,772, 9511,766, 10278,760, 11038,753, 11792,746,
	12539,739, 13278,731, 14009,722, 14732,713, 15446,704, 16150,694, 16845,684, 17530,674, 18204,663, 18867,651, 19519,640, 20159,627, 20787,615, 21402,602, 22004,589, 22594,575,
	23169,561, 23731,547, 24278,532, 24811,517, 25329,502, 25831,486, 26318,471, 26789,454, 27244,438, 27683,421, 28105,404, 28510,387, 28897,370, 29268,352, 29621,334, 29955,316,
	30272,298, 30571,280, 30851,261, 31113,242, 31356,223, 31580,204, 31785,185, 31970,166, 32137,147, 32284,127, 32412,108, 32520,88, 32609,68, 32678,49, 32727,29, 32757,9,
	32767,-9, 32757,-29, 32727,-49, 32678,-68, 32609,-88, 32520,-108, 32412,-127, 32284,-147, 32137,-166, 31970,-185, 31785,-204, 31580,-223, 31356,-242, 31113,-261, 30851,-280, 30571,-298,
	30272,-316, 29955,-334, 29621,-352, 29268,-370, 28897,-387, 28510,-404, 28105,-421, 27683,-438, 27244,-454, 26789,-471, 26318,-486, 25831,-502, 25329,-517, 24811,-532, 24278,-547, 23731,-561,
	23169,-575, 22594,-589, 22004,-602, 21402,-615, 20787,-627, 20159,-640, 19519,-651, 18867,-663, 18204,-674, 17530,-684, 16845,-694, 16150,-704, 15446,-713, 14732,-722, 14009,-731, 13278,-739,
	12539,-746, 11792,-753, 11038,-760, 10278,-766, 9511,-772, 8739,-777, 7961,-782, 7179,-786, 6392,-790, 5601,-793, 4807,-796, 4011,-799, 3211,-801, 2410,-802, 1607,-803, 804,-804,
	0,-804, -804,-803, -1607,-802, -2410,-801, -3211,-799, -4011,-796, -4807,-793, -5601,-790, -6392,-786, -7179,-782, -7961,-777, -8739,-772, -9511,-766, -10278,-760, -11038,-753, -11792,-746,
	-12539,-739, -13278,-731, -14009,-722, -14732,-713, -15446,-704, -16150,-694, -16845,-684, -17530,-674, -18204,-663, -18867,-651, -19519,-640, -20159,-627, -20787,-615, -21402,-602, -22004,-589, -22594,-575,
	-23169,-561, -23731,-547, -24278,-532, -24811,-517, -25329,-502, -25831,-486, -26318,-471, -26789,-454, -27244,-438, -27683,-421, -28105,-404, -28510,-387, -28897,-370, -29268,-352, -29621,-334, -29955,-316,
	-30272,-298, -30571,-280, -30851,-261, -31113,-242, -31356,-223, -31580,-204, -31785,-185, -31970,-166, -32137,-147, -32284,-127, -32412,-108, -32520,-88, -32609,-68, -32678,-49, -32727,-29, -32757,-9,
	-32767,9, -32757,29, -32727,49, -32678,68, -32609,88, -32520,108, -32412,127, -32284,147, -32137,166, -31970,185, -31785,204, -31580,223, -31356,242, -31113,261, -30851,280, -30571,298,
	-30272,316, -29955,334, -29621,352, -29268,370, -28897,387, -28510,404, -28105,421, -27683,438, -27244,454, -26789,471, -26318,486, -25831,502, -25329,517, -24811,532, -24278,547, -23731,561,
	-23169,575, -22594,589, -22004,602, -21402,615, -20787,627, -20159,640, -19519,651, -18867,663, -18204,674, -17530,684, -16845,694, -16150,704, -15446,713, -14732,722, -14009,731, -13278,739,
	-12539,746, -11792,753, -11038,760, -10278,766, -9511,772, -8739,777, -7961,782, -7179,786, -6392,790, -5601,793, -4807,796, -4011,799, -3211,801, -2410,802, -1607,803, -804,804,
	0,804, };


inline int16_t FixedSin(uint32_t const phase)
{
	int i0 = (phase >> (32 - SINETABLE_BITS)) ;
	
	int16_t s0 = SineTable[i0 * 2 ];
	int16_t s1 = SineTable[i0 * 2 + 1];

	uint32_t a = (phase & ((1 << (32 - SINETABLE_BITS)) - 1)) >> ((32 - SINETABLE_BITS) - 16);

	return s0 + ((a * (s1))>>16);
}

extern int32_t SMMLA(int32_t acc, int32_t a, int32_t b);


MEMATTR int32_t SMMLA16(int32_t s0, int32_t s1, int32_t a)
{
	return 	s0 + ((a * (s1)) >> 16);
}


MEMATTR int32_t FourSin(uint32_t const phase0, int const mul1, int const mul2, int const mul3)
{
	//return FixedSin(phase0) + FixedSin(phase0 << 1) + FixedSin(phase0 << 2) + FixedSin(phase0 << 3);

	uint32_t phase1 = phase0 * mul1;
	uint32_t phase2 = phase0 * mul2;
	uint32_t phase3 = phase0 * mul3;

	int i0 = (phase0 >> (32 - SINETABLE_BITS));
	int i1 = (phase1 >> (32 - SINETABLE_BITS));
	int i2 = (phase2 >> (32 - SINETABLE_BITS));
	int i3 = (phase3 >> (32 - SINETABLE_BITS));

	int16_t s0a = SineTable[i0 * 2];
	uint32_t a0 = (phase0 & ((1 << (32 - SINETABLE_BITS)) - 1)) >> ((32 - SINETABLE_BITS) - 16);
	int16_t s0b = SineTable[i0 * 2 + 1];
	int16_t s1a = SineTable[i1 * 2];
	uint32_t Res = SMMLA16(s0a, s0b, a0);
	int16_t s1b = SineTable[i1 * 2 + 1];
	uint32_t a1 = (phase1 & ((1 << (32 - SINETABLE_BITS)) - 1)) >> ((32 - SINETABLE_BITS) - 16);
	Res += SMMLA16(s1a, s1b, a1);
	uint32_t a2 = (phase2 & ((1 << (32 - SINETABLE_BITS)) - 1)) >> ((32 - SINETABLE_BITS) - 16);
	int16_t s2a = SineTable[i2 * 2];
	uint32_t a3 = (phase3 & ((1 << (32 - SINETABLE_BITS)) - 1)) >> ((32 - SINETABLE_BITS) - 16);
	int16_t s2b = SineTable[i2 * 2 + 1];
	Res += SMMLA16(s2a, s2b, a2);
	int16_t s3a = SineTable[i3 * 2];
	int16_t s3b = SineTable[i3 * 2 + 1];


	Res += SMMLA16(s3a, s3b, a3);
	return Res;
}


void Organ_Init(struct Organ_t *osc, float odsr)
{
	osc->mPhase2 = 0;	
	osc->mPhaseIncrement2 = (unsigned long)((65536.f*65536.f) * (3.5f * odsr));

	for (int i = 0; i < 3; i++)
	{
		osc->Voices[i].mPhase = 0;
		osc->Voices[i].mPhaseIncrement = 0;
	}
}


__inline void Organ_Update(struct OrganVoice_t  *osc, float odsr, float centerfreq, float size, float spread)
{
	float Spr = spread * (1.0f / 65536.0f);
	Spr *= Spr;
	osc->spread = (int)((Spr* centerfreq) * (65536.0f / 330.0f));
	uint32_t C =  (unsigned long)((65536.f*65536.f) * (centerfreq  * odsr));
	osc->mPhaseIncrement = C;	
}

MEMATTR int32_t Organ_GetMain(struct  Organ_t  *osc)
{
	osc->mPhase2 += osc->mPhaseIncrement2;
	uint32_t P2 = osc->mPhase2;
	//uint32_t P2F = (uint32_t)(P2 * 65536 * 65536) ;
	osc->Aoff = FixedSin(P2);
	osc->Boff = FixedSin(P2 + (unsigned long)((65536.f*65536.f) * (1.0f / 3.0f)));
	osc->Coff = FixedSin(P2 + (unsigned long)((65536.f*65536.f) * (2.0f / 3.0f)));

	return Organ_Get(osc, &osc->Voices[0]);
}

MEMATTR int32_t Organ_Get(struct  Organ_t  *osc, struct OrganVoice_t *voice)
{
	voice->mPhase += voice->mPhaseIncrement;
	
	uint32_t AP = voice->mPhase + osc->Aoff*voice->spread;
	uint32_t BP = voice->mPhase + osc->Boff*voice->spread;
	uint32_t CP = voice->mPhase + osc->Coff*voice->spread;
	
	int32_t A = FourSin(AP, 2, 4, 8);//  FixedSin(AP) + FixedSin(AP << 1) + FixedSin(AP << 2) + FixedSin(AP << 3);
	int32_t B = FourSin(BP, 2, 4, 8);//  FixedSin(AP) + FixedSin(AP << 1) + FixedSin(AP << 2) + FixedSin(AP << 3);
	int32_t C = FourSin(CP, 2, 4, 8);//  FixedSin(AP) + FixedSin(AP << 1) + FixedSin(AP << 2) + FixedSin(AP << 3);
//	int32_t B = FixedSin(BP ) + FixedSin(BP<<1) + FixedSin(BP<<2) + FixedSin(BP <<3) ;
	//int32_t C = FixedSin(CP ) + FixedSin(CP<<1) + FixedSin(CP<<2) + FixedSin(CP <<3) ;
	return (A + B + C) ;
}
