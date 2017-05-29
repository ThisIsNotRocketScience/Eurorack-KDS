#pragma once
#ifndef BLEPPYOSCS
#define BLEPPYOSCS

#include<stdint.h>
#include "BigFishDefs.h"

#define MAXHYPER 4

typedef struct WaveBlep_t
{
	float mMasterPhase;
	float mSlavePhase;
	float mPhaseIncrement;
	float mPhaseIncrement2;
	float OutVal;
	float PrevOut;
	float speedmul;
	int index;
	int waveindex;

	float wavea[9];
	float waveb[9];

	float circularBuffer[64];
	int32_t circularBufferInt[64];
} WaveBlep_t;

typedef struct VosimBlep_t
{
	float mPhase;
	float mPhase2;
	float mPhaseIncrement;
	float mPhaseIncrement2;
	float OutVal;
	int index;
	int VosimStep;
	int32_t Sign;
	float circularBuffer[64];
} VosimBlep_t;

typedef struct MinBlepOsc_t
{
	uint32_t mPhase;
	uint32_t mLastPhase;
	uint32_t mPhaseIncrement;
	int index;
	int32_t Sign;
	int32_t circularBuffer[64];
} MinBlepOsc_t;

typedef struct HyperSet_t
{
	float Level[MAXHYPER];
	int32_t iLevel[MAXHYPER];
	float Freq[MAXHYPER];
	int Active;

} HyperSet_t;

typedef struct HyperOsc_t
{
	uint32_t mPhase[MAXHYPER];
	uint32_t mLastPhase[MAXHYPER];

	uint32_t mPhaseIncrement[MAXHYPER];
	int index;
	int32_t circularBuffer[64];
	struct HyperSet_t HyperSet;
} HyperOsc_t;

typedef struct HyperPulse_t
{
	uint32_t mPhase[MAXHYPER];
	uint32_t mPhaseIncrement[MAXHYPER];
	uint32_t mLastPhase[MAXHYPER];
	int32_t Sign[MAXHYPER];
	int index;
	int32_t circularBuffer[64];
	struct HyperSet_t HyperSet;
} HyperPulse_t;





void AddBlep(float *circbuffer, int index, float scale, float crosstime);
void HyperCalculate_Spread(struct HyperSet_t *set, float spread, float size);

void VosimOsc_Init(struct VosimBlep_t *osc);
void VosimOsc_Update(struct VosimBlep_t *osc, float odsr, float centerfreq, float size, float spread);
float VosimOsc_Get(struct  VosimBlep_t *osc);

void HyperOsc_Init(struct HyperOsc_t *osc);
void HyperOsc_Update(struct HyperOsc_t *osc, float odsr, float centerfreq, float size, float spread);
int32_t HyperOsc_Get(struct HyperOsc_t *osc);

void HyperPulse_Init(struct HyperPulse_t *osc);
void HyperPulse_Update(struct HyperPulse_t  *osc, float odsr, float centerfreq, float size, float spread);
int32_t HyperPulse_Get(struct HyperPulse_t *osc);

void MinBlepOsc_Init(struct MinBlepOsc_t *osc);
void MinBlepOsc_Update(struct MinBlepOsc_t *osc, float odsr, float centerfreq, float size, float spread);
int32_t MinBlepOsc_Get(struct  MinBlepOsc_t *osc);
int32_t MinBlepOsc_GetPulse(struct  MinBlepOsc_t *osc);


void WaveBlepOsc_Init(struct WaveBlep_t *osc);
void WaveBlepOsc_Update(struct WaveBlep_t *osc, float odsr, float centerfreq, float size, float spread);
float WaveBlepOsc_Get(struct  WaveBlep_t *osc);



#endif