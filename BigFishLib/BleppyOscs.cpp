#include "BleppyOscs.h"
#include <math.h>
#include <stdlib.h>

#define LERP(F,A,B) ((B-A)*F+A)
#define LERPINT(F,A,B, FBITS ) (((B-A)*F>>FBITS) + A)

extern float *blep;
extern int32_t *intblep;

inline float downrampfrom1(float factor, float input)
{
	return 1 + ((input - 1)*factor);
}
void HyperCalculate_Spread(struct HyperSet_t *set, float spread, float size)
{
	float OrigLevel = 1.0;

	float fExtraSaws = size * 6;
	int nExtraSaws = (int)__min(floor(fExtraSaws) + 1, 6);
	if (fExtraSaws <= 0.000001f) nExtraSaws = 0;
	float fFracSaw = (float)(fExtraSaws - floor(fExtraSaws));
	float fSaws = 1 + fExtraSaws;
	int NewnSaws = nExtraSaws + 1;

	float NewSawLevel[MAXHYPER];

	float NewLevel = (OrigLevel*OrigLevel) / (downrampfrom1(-0.1f, fSaws) * 2);
	for (int i = 0; i<NewnSaws; i++)
	{
		NewSawLevel[i] = 1;
	};

	if (fabs(fFracSaw)>0.00001)
	{
		float FractLevel = fFracSaw*fFracSaw;
		NewSawLevel[NewnSaws - 1] = FractLevel;
	}
	float total = 0;

	for (int i = 0; i<NewnSaws; i++)
	{
		total += NewSawLevel[i];
	}
	for (int i = 0; i<NewnSaws; i++)
	{
		NewSawLevel[i] *= (1 / total)*NewLevel;
	}

	float Level = NewLevel;
	for (int i = 0; i<NewnSaws; i++)
	{
		set->Level[i] = NewSawLevel[i];
	};
	set->Active = NewnSaws;

	set->Freq[0] = 1;
	for (int i = 1; i<set->Active; i++)
	{
		float Detune = ((((float)i) / 2.0f))*(spread * (1.0f / 6.0f))*(0.5f / (fSaws*0.5f));
		if (i & 2)
		{
			set->Freq[i] = powf(2.f, Detune);
		}
		else
		{
			set->Freq[i] = powf(2.f, -Detune);
		};
	};
}

void AddBlep(float *circbuffer, int index, float scale, float crosstime)
{

	float tempIndex = (crosstime*32.0f);
	float tempFraction = tempIndex - (float)floor(tempIndex);
	for (int i = 0; i < 47; i++)
	{
		circbuffer[(index + i) % 48] += scale * (float)(1.0f - LERP(tempFraction, blep[(int)floor(tempIndex)], blep[(int)ceil(tempIndex)]));
		tempIndex += 32;
	}


}

void AddBlep_Int(int32_t *circbuffer, int index, int32_t scale, int32_t crosstime)
{
	int32_t tempIndex = (crosstime*32.0f);
	int32_t tempFraction = tempIndex & 0xffffff;

	tempIndex >>= 24;

	for (int i = 0; i < 47; i++)
	{
		int64_t R = ((1 << 24) - LERPINT(tempFraction, intblep[tempIndex], intblep[tempIndex + 1], 24));
		R *= scale;
		R >>= 24;
		circbuffer[(index + i) % 48] += int(R);
		
		tempIndex += 32;
	}
}


void WaveBlepOsc_Init(struct WaveBlep_t *osc)
{
	osc->index = 0;
	osc->mMasterPhase = 0;
	osc->mSlavePhase = 0;
	osc->OutVal = 0;
	osc->PrevOut = 0;
	osc->mPhaseIncrement = 0.1f;
	osc->waveindex = 0;
	for (int i = 0; i < 48; i++)
	{
		osc->circularBuffer[i] = 0;
		osc->circularBufferInt[i] = 0;
	}
}
void WaveBlepOsc_Update(struct WaveBlep_t *osc, int samplerate, float centerfreq, float size, float spread)
{
	float C = centerfreq / (float)samplerate;
	osc->mPhaseIncrement = C;
	osc->speedmul = (2 + (size * (6.0f / 65536.0f)));
	osc->mPhaseIncrement2 = osc->mPhaseIncrement * osc->speedmul;
	size /= 32768;
	spread /= 32768;
	osc->wavea[0] = sinf(size);
	osc->wavea[1] = sinf(size*1.1);
	osc->wavea[2] = sinf(size*1.2);
	osc->wavea[3] = cosf(spread*3.10);
	osc->wavea[4] = cosf(spread*2.4);
	osc->wavea[5] = cosf(size*1.1230);
	osc->wavea[6] = cosf(spread*5.20);
	osc->wavea[7] = sinf(size * .510 + spread*0.6120);
	osc->waveb[0] = sinf(size*1.4);
	osc->waveb[1] = sinf(size*1.5);
	osc->waveb[2] = sinf(size*1.7);
	osc->waveb[3] = cosf(spread*5.10);
	osc->waveb[4] = cosf(spread*3.4);
	osc->waveb[5] = cosf(size*2.4230);
	osc->waveb[6] = cosf(spread*2.20);
	osc->waveb[7] = sinf(size * 1.510 + spread*2.6120);
	int max = osc->speedmul -2;
//	osc->wavea[max] = osc->wavea[0];
	//osc->waveb[max] = osc->wavea[0];
//	osc->wavea[max+1] = osc->wavea[0];
//	osc->waveb[max+1] = osc->wavea[0];

	for (int i = 0; i < 8; i++) osc->waveb[i]  -= osc->wavea[i];
}

float WaveBlepOsc_Get(struct  WaveBlep_t *osc)
{
	osc->mMasterPhase += osc->mPhaseIncrement;
	osc->mSlavePhase += osc->mPhaseIncrement2;
	osc->index = (osc->index + 1) % 48;
	while (osc->mMasterPhase >= 1.0)
	{
		osc->mMasterPhase -= 1.0;
		float olddelta = osc->waveb[osc->waveindex];
		osc->mSlavePhase = osc->mMasterPhase * osc->speedmul;
		osc->waveindex = 0;
		float newOutVal = osc->wavea[0];
		float exactCrossTime = 1.0f - ((osc->mPhaseIncrement - osc->mMasterPhase) / osc->mPhaseIncrement);
		float deltaoutval = newOutVal - (osc->OutVal + (exactCrossTime)* osc->mPhaseIncrement2 * olddelta);
	//	osc->OutVal = newOutVal;
//		osc->OutVal = osc->wavea[osc->waveindex] - osc->waveb[osc->waveindex] * (osc->mSlavePhase - osc->mPhaseIncrement2);;
		AddBlep(osc->circularBuffer, osc->index, -deltaoutval, exactCrossTime);
	}



	while (osc->mSlavePhase >= 1.0)
	{
//		float M = (1 - osc->mMasterPhase) / osc->mPhaseIncrement;
	//	if (M > 1)
		{
			osc->mSlavePhase -= 1.0;
			float olddelta = osc->waveb[osc->waveindex];
			osc->waveindex = (osc->waveindex + 1) & 7;
			float newOutVal = osc->wavea[osc->waveindex];
			float exactCrossTime = 1.0f - ((osc->mPhaseIncrement2 - osc->mSlavePhase) / osc->mPhaseIncrement2);
			float deltaoutval = newOutVal - (osc->OutVal + (exactCrossTime) * osc->mPhaseIncrement2 * olddelta);
			//osc->OutVal = osc->wavea[osc->waveindex] - osc->waveb[osc->waveindex] * (osc->mSlavePhase - osc->mPhaseIncrement2);;

			AddBlep(osc->circularBuffer, osc->index, -deltaoutval, exactCrossTime);
		}
	}
	osc->PrevOut = osc->OutVal;
	osc->OutVal = osc->wavea[osc->waveindex] + osc->waveb[osc->waveindex] * osc->mSlavePhase;
	osc->circularBuffer[osc->index] += osc->OutVal;
	float output = osc->circularBuffer[osc->index];
	osc->circularBuffer[osc->index] = 0.0f;
	return output ;
}

void VosimOsc_Init(struct VosimBlep_t *osc)
{
	osc->index = 0;
	osc->mPhase = 0;
	osc->mPhase2 = 0;
	osc->Sign = -1000;
	osc->VosimStep = 10;
	osc->mPhaseIncrement = 0.1f;
	for (int i = 0; i < 48; i++)
	{
		osc->circularBuffer[i] = 0;
	}
}
void VosimOsc_Update(struct VosimBlep_t *osc, int samplerate, float centerfreq, float size, float spread)
{
	float C = centerfreq / (float)samplerate;
	osc->mPhaseIncrement = C;
	osc->mPhaseIncrement2 = ((400 + 18000 * size) / (float)samplerate)*6.283f;
}
float VosimOsc_GetO(struct  VosimBlep_t *osc)
{
	osc->mPhase += osc->mPhaseIncrement;
	osc->index = (osc->index + 1) % 48;

	while (osc->mPhase >= 1.0)
	{
		osc->mPhase -= 1.0;
		float exactCrossTime = 1.0f - ((osc->mPhaseIncrement - osc->mPhase) / osc->mPhaseIncrement);
		float NewVosim = osc->VosimStep - 32768 / 16;
		if (NewVosim <= 0) NewVosim = 32768;
		if (osc->Sign>0)
		{
			osc->Sign = -1;

			float	step = NewVosim + osc->VosimStep;
			AddBlep(osc->circularBuffer, osc->index, step, exactCrossTime);
			osc->OutVal = -NewVosim;
		}
		else
		{
			float	step = NewVosim + osc->VosimStep;

			AddBlep(osc->circularBuffer, osc->index, -step, exactCrossTime);
			osc->Sign = 1;
			osc->OutVal = NewVosim;
		}

		osc->VosimStep = NewVosim;


	}

	osc->circularBuffer[osc->index] += osc->OutVal;
	float output = osc->circularBuffer[osc->index];
	osc->circularBuffer[osc->index] = 0.0f;

	return output;
}
float VosimOsc_Get(struct  VosimBlep_t *osc)
{
	osc->mPhase += osc->mPhaseIncrement;
	osc->mPhase2 += osc->mPhaseIncrement2;
	osc->index = (osc->index + 1) % 48;
	while (osc->mPhase2 >= 3.1415 * 2)
	{
		osc->mPhase2 -= 3.1415*2.0;
	}

	while (osc->mPhase >= 1.0)
	{
		osc->mPhase -= 1.0;
		float exactCrossTime = 1.0f - ((osc->mPhaseIncrement - osc->mPhase) / osc->mPhaseIncrement);
		AddBlep(osc->circularBuffer, osc->index, 1, exactCrossTime);
	}

	osc->circularBuffer[osc->index] += (osc->mPhase - osc->mPhaseIncrement);
	float output = osc->circularBuffer[osc->index];
	osc->circularBuffer[osc->index] = 0.0f;
	return -output * sinf(osc->mPhase2);
}

void HyperOsc_Init(struct HyperOsc_t *osc)
{
	osc->index = 0;
	for (int i = 0; i < MAXHYPER; i++)
	{
		osc->mPhase[i] = 0;
		osc->mPhaseIncrement[i] = 0.1f;
	}
	for (int i = 0; i < 48; i++)
	{
		osc->circularBuffer[i] = 0;
	}
}
void HyperOsc_Update(struct HyperOsc_t *osc, int samplerate, float centerfreq, float size, float spread)
{
	float C = centerfreq / (float)samplerate;

	HyperCalculate_Spread(&osc->HyperSet, spread, size);

	for (int i = 0; i < osc->HyperSet.Active; i++)
	{
		osc->mPhaseIncrement[i] = C * osc->HyperSet.Freq[i];;
	}
}
float HyperOsc_Get(struct HyperOsc_t *osc)
{
	osc->index = (osc->index + 1) % 48;
	for (int o = 0; o < osc->HyperSet.Active; o++)
	{
		osc->mPhase[o] += osc->mPhaseIncrement[o];
		while (osc->mPhase[o] >= 1.0f)
		{
			osc->mPhase[o] -= 1.0f;
			float exactCrossTime = 1.0f - ((osc->mPhaseIncrement[o] - osc->mPhase[o]) / osc->mPhaseIncrement[o]);
			AddBlep(osc->circularBuffer, osc->index, osc->HyperSet.Level[o] * 2, exactCrossTime);
		}

		osc->circularBuffer[osc->index] += ((osc->mPhase[o] - osc->mPhaseIncrement[o]) * 2 - 1.0f) * osc->HyperSet.Level[o];
	}

	float output = osc->circularBuffer[osc->index];
	osc->circularBuffer[osc->index] = 0.0f;
	return output;
}

void HyperPulse_Init(struct HyperPulse_t *osc)
{
	osc->index = 0;

	for (int i = 0; i < MAXHYPER; i++)
	{
		osc->Sign[i] = -1;
		osc->mPhase[i] = 0;
		osc->mPhaseIncrement[i] = 0.1f;
	}
	for (int i = 0; i < 48; i++)
	{
		osc->circularBuffer[i] = 0;
	}
}
void HyperPulse_Update(struct HyperPulse_t  *osc, int samplerate, float centerfreq, float size, float spread)
{
	float C = (centerfreq * 2) / (float)samplerate;
	HyperCalculate_Spread(&osc->HyperSet, spread, size);


	for (int i = 0; i < osc->HyperSet.Active; i++)
	{
		osc->mPhaseIncrement[i] = C * osc->HyperSet.Freq[i];;
	}
}
float HyperPulse_Get(struct HyperPulse_t *osc)
{
	osc->index = (osc->index + 1) % 48;
	for (int o = 0; o < osc->HyperSet.Active; o++)
	{
		osc->mPhase[o] += osc->mPhaseIncrement[o];
		while (osc->mPhase[o] >= 1.0f)
		{
			osc->mPhase[o] -= 1.0f;
			float exactCrossTime = 1.0f - ((osc->mPhaseIncrement[o] - osc->mPhase[o]) / osc->mPhaseIncrement[o]);
			if (osc->Sign[o]>0)
			{
				AddBlep(osc->circularBuffer, osc->index, 2 * osc->HyperSet.Level[o], exactCrossTime);
				osc->Sign[o] = -1;
			}
			else
			{
				AddBlep(osc->circularBuffer, osc->index, -2 * osc->HyperSet.Level[o], exactCrossTime);
				osc->Sign[o] = 1;
			}
		}

		osc->circularBuffer[osc->index] += osc->Sign[o] * osc->HyperSet.Level[o];
	}

	float output = osc->circularBuffer[osc->index];
	osc->circularBuffer[osc->index] = 0.0f;
	return output;
}


void MinBlepOsc_Init(struct MinBlepOsc_t *osc)
{
	osc->index = 0;
	osc->mPhase = 0;
	osc->Sign = -1;
	osc->mPhaseIncrement = 0.1f;
	for (int i = 0; i < 48; i++)
	{
		osc->circularBuffer[i] = 0;
	}
}
void MinBlepOsc_Update(struct MinBlepOsc_t *osc, int samplerate, float centerfreq, float size, float spread)
{
	float C = centerfreq / (float)samplerate;
	osc->mPhaseIncrement = C;
}
float MinBlepOsc_Get(struct  MinBlepOsc_t *osc)
{
	osc->mPhase += osc->mPhaseIncrement;
	osc->index = (osc->index + 1) % 48;

	while (osc->mPhase >= 1.0)
	{
		osc->mPhase -= 1.0;
		float exactCrossTime = 1.0f - ((osc->mPhaseIncrement - osc->mPhase) / osc->mPhaseIncrement);
		AddBlep(osc->circularBuffer, osc->index, 1, exactCrossTime);
	}

	osc->circularBuffer[osc->index] += (osc->mPhase - osc->mPhaseIncrement);
	float output = osc->circularBuffer[osc->index];
	osc->circularBuffer[osc->index] = 0.0f;
	return output * 2.0 - 1.0;
}
float MinBlepOsc_GetPulse(struct  MinBlepOsc_t *osc)
{
	osc->mPhase += osc->mPhaseIncrement;
	osc->index = (osc->index + 1) % 48;

	while (osc->mPhase >= 1.0)
	{
		osc->mPhase -= 1.0;
		float exactCrossTime = 1.0f - ((osc->mPhaseIncrement - osc->mPhase) / osc->mPhaseIncrement);

		if (osc->Sign>0)
		{
			AddBlep(osc->circularBuffer, osc->index, 2, exactCrossTime);
			osc->Sign = -1;
		}
		else
		{
			AddBlep(osc->circularBuffer, osc->index, -2, exactCrossTime);
			osc->Sign = 1;
		}


	}

	osc->circularBuffer[osc->index] += osc->Sign;
	float output = osc->circularBuffer[osc->index];
	osc->circularBuffer[osc->index] = 0.0f;
	return output;
}


