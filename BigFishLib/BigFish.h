#pragma once

#ifndef BIGFISH
#define BIGFISH

#include <stdint.h>
#define MAXFISHBUFFER 32

#ifndef __min
#define __min(a,b) (((a)<(b))?(a):(b))
#endif


enum
{
	// pitch related
	PITCH_FINE,
	PITCH_COURSE,
	PITCH_CHORD,

	// osc related
	OSC_SHAPE,
	OSC_SIZE,
	OSC_SPREAD,

	// amp env related
	AMP_VELOCITY,
	AMP_ATTACK,
	AMP_DECAY,
	AMP_SUSTAIN,
	AMP_RELEASE,

	// filter
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

	__PARAMCOUNT
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

typedef struct ADSR_Envelope_t
{
	uint8_t State;
} ADSR_Envelope_t;

typedef struct BigFish_t
{
	uint16_t Parameters[__PARAMCOUNT]; // prenormalized to 16bit.
	uint16_t Leds[__LEDCOUNT];
	uint8_t Gates[__GATECOUNT];
	uint8_t PreviousGates[__GATECOUNT];

	HyperOsc_t HyperSawOsc;
	HyperOsc_t HyperPulseOsc;
	MinBlepOsc_t NormalOsc;

	Inertia_t Accent;
	ADSR_Envelope_t AmpEnvelope;
	ADSR_Envelope_t FilterEnvelope; // s and r set to 0 for plain AD envelope. 
} BigFish_t;

#ifdef  __cplusplus
extern "C"
{
#endif

	void BigFish_Init(struct BigFish_t *fish);
	void BigFish_Update(struct BigFish_t *fish);
	void BigFish_GenerateBlock(struct BigFish_t *fish, int32_t *buffer, int len);
	
	void HyperOsc_Init(struct HyperOsc_t * osc);
	float HyperOsc_Get(struct HyperOsc_t * osc);

	void MinBlepOsc_Init(struct MinBlepOsc_t * osc);
	float MinBlepOsc_Get(struct MinBlepOsc_t * osc);

#ifdef  __cplusplus
}
#endif



#endif //  __cpp


