#ifndef ADSRENVELOPE
#define ADSRENVELOPE

#include <stdint.h>

#define ENVFIXEDBITS 16

#define ENVFRACMASK ((1<<ENVFIXEDBITS) - 1)

#define ENVFIXED(x) ((int32_t)( x * (float)(1<<ENVFIXEDBITS) )-1)

#define ENVSTATE_IDLE 0
#define ENVSTATE_ATTACK 1
#define ENVSTATE_DECAY 2
#define ENVSTATE_SUSTAIN 3
#define ENVSTATE_RELEASE 4

#define ENVMODE_TRIGGER 0
#define ENVMODE_GATE 1
#define ENVMODE_LOOP 2

#define GATE_ATTACKEND 3
#define GATE_DECAYEND 2
#define GATE_RELEASESTART 1
#define GATE_RELEASEEND 0

#define GATE_COUNTDOWN 30;

enum ENVTABLETYPES
{
	ENVTABLE_EXP,
	ENVTABLE_LOG,
	__ENVTABLE_COUNT
};

typedef struct ADSR_Envelope_t
{
	unsigned char Mode;
	unsigned char Speed;

	uint16_t A;
	uint16_t D;
	uint16_t S;
	uint16_t R;

	int32_t Aconv;
	int32_t Dconv;
	int32_t Sconv;
	int32_t Rconv;

	uint16_t Curvature;
	unsigned char AttackCurve;

	unsigned char TriggerState;
	
	int32_t LinearOutput;
	int32_t CurvedOutput;

	int State;
	
	unsigned char Gates[5];
	unsigned char StateLeds[14];

	int32_t AttackStart;
	int32_t DecayStart;
	int32_t ReleaseStart;
	int32_t ReleaseStartCurved;
	
	int32_t AttackProgress;
	int32_t DecayProgress;
	int32_t ReleaseProgress;


	int32_t Current;
	int32_t CurrentTarget;
} ADSR_Envelope_t;

int ADSR_Get(struct ADSR_Envelope_t *Env);
int ADSR_GetCurved(struct ADSR_Envelope_t *Env);
void ADSR_Init(struct ADSR_Envelope_t *Env, int Mode, int Speed, int AttackTable);
void ADSR_Trigger(struct ADSR_Envelope_t *Env, unsigned char N);
void ADSR_BuildTable();
void ADSR_Update(struct ADSR_Envelope_t *Env, int SampleRate);

uint32_t GetExpTable(uint32_t inp, int table);

#endif