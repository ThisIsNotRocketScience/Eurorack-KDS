#ifndef TUESDAY_ALGO_H
#define TUESDAY_ALGO_H

#include <stdint.h>
#include "Tuesday.h"

struct ScaledNote
{
	int32_t oct;
	int32_t note;
};

#define NOTE(aoct, anote) { SN.note = anote;SN.oct = aoct;};
#define NOTEOFF() { SN.note = TUESDAY_NOTEOFF;SN.oct = 0;};

typedef enum
{
	// tests to build:
	// - all octaves
	// - one octave updown
	// - one octave, 12tonal
	// - full range walk
	ALGO_TESTS,

	// german style minimal melodies
	ALGO_TRITRANCE,

	// fishyfishy! with slides! 
	ALGO_STOMPER,

	// Random walking with mr. Markov.
	ALGO_MARKOV,

	//	ALGO_PACHEDECO,
	ALGO_WOBBLE,
	ALGO_CHIPARP1,
	//ALGO_CHIPARP2,

	// classic saiko things, reimagined
	//ALGO_SAIKO_BASS,
	//ALGO_SAIKO_PSY,
	ALGO_SNH,
	ALGO_SAIKO_LEAD,
	__ALGO_COUNT
} TUESDAY_ALGO;

enum STOMPERACTIONS
{
	STOMPER_PAUSE1,
	STOMPER_PAUSE2,
	STOMPER_HIGH1,
	STOMPER_HIGH2,
	STOMPER_LOW1,
	STOMPER_LOW2,
	STOMPER_LOWHI1,
	STOMPER_LOWHI2,
	STOMPER_HILOW1,
	STOMPER_HILOW2,
	STOMPER_SLIDEUP1,
	STOMPER_SLIDEUP2,
	STOMPER_SLIDEDOWN1,
	STOMPER_SLIDEDOWN2,
	STOMPER_ACTIONCOUNT,
	STOMPER_MAKENEW
};

typedef struct PatternStruct_Algo_Stomper
{
	unsigned char LastMode;
	unsigned char CountDown;
	unsigned char LowNote;
	unsigned char HighNote[2];
	signed short LastNote;
	unsigned char LastOctave;

} PatternStruct_Algo_Stomper;

typedef struct PatternStruct_Algo_Markov
{
	int16_t NoteHistory1;
	int16_t NoteHistory3;
	
	uint8_t matrix[8][8][2];
} PatternStruct_Algo_Markov;

typedef struct PatternStruct_Algo_GENERIC
{
	int16_t b1;
	int16_t b2;
	int16_t b3;
	int16_t b4;
} PatternStruct_Algo_GENERIC;

enum TESTMODE
{
	TESTMODE_OCTSWEEPS,
	TESTMODE_SCALEWALKER,
	TESTMODE_MINMAX,
	TESTMODE_COUNT
};

typedef struct PatternStruct_Algo_Test
{
	unsigned char Mode;	
	unsigned char SweepSpeed;
	unsigned char Accent;
	unsigned char Velocity;
	signed short Note;
} PatternStruct_Algo_Test;


typedef struct PatternStruct_Algo_Wobble
{
	uint32_t Phase;
	uint32_t PhaseSpeed;
	uint32_t Phase2;
	uint32_t PhaseSpeed2;
	unsigned char LastWasHigh : 1;
} PatternStruct_Algo_Wobble;


typedef struct PatternStruct_Algo_SNH
{
	uint32_t Phase;
	uint32_t PhaseSpeed;
	struct EURORACK_SVF Filt;
} PatternStruct_Algo_SNH; 

struct Tuesday_PatternFuncSpecific
{
	union
	{
		struct PatternStruct_Algo_GENERIC GENERIC;
		struct PatternStruct_Algo_Stomper Stomper;
		struct PatternStruct_Algo_Markov Markov;
		struct PatternStruct_Algo_Test Test;
		struct PatternStruct_Algo_Wobble Wobble;
		struct PatternStruct_Algo_SNH SNH;
	};
	struct Tuesday_RandomGen ExtraRandom;
};

int ScaleToNote(struct ScaledNote *SN, struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S);

typedef void(*GenFuncPtr)(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *PS, int I, struct Tuesday_Tick *Output);
typedef void(*InitFuncPtr)(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *Output);
typedef void(*PatternInitFuncPtr)(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_PatternContainer *PT);

void NoInit(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *Output);
void NoPattern(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *PS, int I, struct Tuesday_Tick *Output);
void NoPatternInit(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_PatternContainer *PT);

void Pattern_Rotate(struct Tuesday_PatternContainer *T, int first, int length, int rotate);
void Pattern_Reverse(struct Tuesday_PatternContainer *T, int first, int length);
void Pattern_Transpose(struct Tuesday_PatternContainer *T, int first, int length, int transpose);

int Tuesday_Rand(struct Tuesday_RandomGen *R);
uint8_t Tuesday_BoolChance(struct Tuesday_RandomGen *R);
uint8_t Tuesday_RandByte(struct Tuesday_RandomGen *R);

struct PatternFunctions
{
	GenFuncPtr Gen;
	InitFuncPtr Init;
	PatternInitFuncPtr PatternInit;
	uint8_t Dither;
};

uint8_t Tuesday_PercChance(struct Tuesday_RandomGen *R, uint8_t perc);

void Algo_Saiko_Lead_Gen(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *PS, int I, struct Tuesday_Tick *Output);



#endif
