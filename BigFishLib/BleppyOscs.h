#pragma once
#ifndef BLEPPYOSCS
#define BLEPPYOSCS

#include<stdint.h>
#include "BigFishDefs.h"
#include "BleppyOscs.h"

#define MAXHYPER 7

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

	float wavea[8];
	float waveb[8];

	float circularBuffer[48];
	int32_t circularBufferInt[48];
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
	float circularBuffer[48];
} VosimBlep_t;

typedef struct MinBlepOsc_t
{
	float mPhase;
	float mPhaseIncrement;
	int index;
	signed char Sign;
	float circularBuffer[48];

} MinBlepOsc_t;

typedef struct HyperSet_t
{
	float Level[MAXHYPER];
	float Freq[MAXHYPER];
	int Active;

} HyperSet_t;

typedef struct HyperOsc_t
{
	float mPhase[MAXHYPER];
	float mPhaseIncrement[MAXHYPER];
	int index;
	float circularBuffer[48];
	struct HyperSet_t HyperSet;
} HyperOsc_t;

typedef struct HyperPulse_t
{
	float mPhase[MAXHYPER];
	float mPhaseIncrement[MAXHYPER];
	signed char Sign[MAXHYPER];
	int index;
	float circularBuffer[48];
	struct HyperSet_t HyperSet;
} HyperPulse_t;



extern float *blep;
extern int32_t *intblep;


void AddBlep(float *circbuffer, int index, float scale, float crosstime);
void HyperCalculate_Spread(struct HyperSet_t *set, float spread, float size);

void VosimOsc_Init(struct VosimBlep_t *osc);
void VosimOsc_Update(struct VosimBlep_t *osc, int samplerate, float centerfreq, float size, float spread);
float VosimOsc_Get(struct  VosimBlep_t *osc);

void HyperOsc_Init(struct HyperOsc_t *osc);
void HyperOsc_Update(struct HyperOsc_t *osc, int samplerate, float centerfreq, float size, float spread);
float HyperOsc_Get(struct HyperOsc_t *osc);

void HyperPulse_Init(struct HyperPulse_t *osc);
void HyperPulse_Update(struct HyperPulse_t  *osc, int samplerate, float centerfreq, float size, float spread);
float HyperPulse_Get(struct HyperPulse_t *osc);

void MinBlepOsc_Init(struct MinBlepOsc_t *osc);
void MinBlepOsc_Update(struct MinBlepOsc_t *osc, int samplerate, float centerfreq, float size, float spread);
float MinBlepOsc_Get(struct  MinBlepOsc_t *osc);
float MinBlepOsc_GetPulse(struct  MinBlepOsc_t *osc);


void WaveBlepOsc_Init(struct WaveBlep_t *osc);
void WaveBlepOsc_Update(struct WaveBlep_t *osc, int samplerate, float centerfreq, float size, float spread);
float WaveBlepOsc_Get(struct  WaveBlep_t *osc);



#endif