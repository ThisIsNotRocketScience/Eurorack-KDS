#pragma once

#ifndef BIGFISH
#define BIGFISH

#include <stdint.h>
#define MAXFISHBUFFER 32


#include "ADSREnvelope.h"
#include "PlatinumClip.h"

#ifndef __min
#define __min(a,b) (((a)<(b))?(a):(b))
#endif


enum
{
	// pitch related
	PITCH_FINE,
	PITCH_COARSE,
	PITCH_CHORD,

	// osc related
	OSC_SHAPE,
	OSC_SIZE,
	OSC_SPREAD,

	// amp env related
	AMP_ATTACK,
	AMP_DECAY,
	AMP_SUSTAIN,
	AMP_RELEASE,

	// filter
	FILTER_TYPE,

	FILTER_CUTOFF,
	FILTER_RESONANCE,
	FILTER_PEAKS,
	FILTER_DRIFT,
	FILTER_KEYTRACK,
	FILTER_ENVELOPE,
	FILTER_DRIVE,


	// filter env related
	FILTER_ATTACK,
	FILTER_DECAY,
	FILTER_ACCENT,

	AMP_VELOCITY,

	__PARAMCOUNT
};

enum
{
	FILTERTYPE_LP,
	FILTERTYPE_HP,
	FILTERTYPE_VOCAL,
	FILTERTYPE_BP,
	FILTERTYPE_BR,
	__FILTERTYPE_COUNT
};

enum
{
	FILTERKEYTRACK_NEG2,
	FILTERKEYTRACK_NEG1HALF,
	FILTERKEYTRACK_NEG1,
	FILTERKEYTRACK_NEGHALF,
	FILTERKEYTRACK_OFF,
	FILTERKEYTRACK_POSHALF,
	FILTERKEYTRACK_POS1,
	FILTERKEYTRACK_POS1HALF,
	FILTERKEYTRACK_POS2,
	__FILTERKEYTRACK_COUNT
};

enum
{
	FISHGATE_GATE,
	FISHGATE_ACCENT,
	__GATECOUNT
};

enum
{
	LED_OSCMODE_1,
	LED_OSCMODE_2,
	LED_OSCMODE_3,
	LED_OSCMODE_4,
	LED_OSCMODE_5,
	LED_OSCMODE_6,
	LED_OSCMODE_7,

	LED_FILTERMODE_1,
	LED_FILTERMODE_2,
	LED_FILTERMODE_3,
	LED_FILTERMODE_4,

	__LEDCOUNT
};

enum
{
	ENV_IDLE,
	ENV_ATTACK,
	ENV_DECAY,
	ENV_SUSTAIN,
	ENV_RELEASE
};

typedef struct Inertia_t
{
	uint32_t Target;
	uint32_t Current;
	uint16_t Speed;
} Inertia_t;

#define MAXHYPER 7

typedef struct MinBlepOsc_t
{
	float mPhase;
	float mPhaseIncrement;
	int index;
	float circularBuffer[48];

} MinBlepOsc_t;

typedef struct HyperOsc_t
{
	float mPhase[MAXHYPER];
	float mPhaseIncrement[MAXHYPER];
	int index;
	float circularBuffer[48];
} HyperOsc_t;


typedef struct BigFish_t
{
	
	int SampleRate;
	float CenterFreq;
	int32_t PitchInput; // (1<<14) = 1 octave

	uint16_t Parameters[__PARAMCOUNT]; // prenormalized to 16bit.
	uint16_t Leds[__LEDCOUNT];
	uint8_t Gates[__GATECOUNT];
	uint8_t PreviousGates[__GATECOUNT];

	HyperOsc_t HyperSawOsc;
	HyperOsc_t HyperPulseOsc;
	MinBlepOsc_t NormalOsc;

	Inertia_t Accent;
	struct ADSR_Envelope_t AmpEnvelope;
	struct ADSR_Envelope_t FilterEnvelope; // s and r set to 0 for plain AD envelope. 

	PlatinumClip Clipper;
} BigFish_t;

#ifdef  __cplusplus
extern "C"
{
#endif

	void BigFish_Init(struct BigFish_t *fish, int samplerate);
	void BigFish_Update(struct BigFish_t *fish);
	void BigFish_CheckGates(struct BigFish_t *fish);
	void BigFish_GenerateBlock(struct BigFish_t *fish, int32_t *bufferOSCOUT, int32_t *bufferMAIN, int len);

	void HyperOsc_Init(struct HyperOsc_t * osc);
	float HyperOsc_Get(struct HyperOsc_t * osc);

	void MinBlepOsc_Init(struct MinBlepOsc_t * osc);
	float MinBlepOsc_Get(struct MinBlepOsc_t * osc);

#ifdef  __cplusplus
}
#endif



#endif //  __cpp


