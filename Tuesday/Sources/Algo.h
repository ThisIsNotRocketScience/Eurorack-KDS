#ifndef TUESDAY_ALGO_H
#define TUESDAY_ALGO_H

#include <stdint.h>
#include "Tuesday.h"

#ifndef __max
#define __max(a,b) (((a)>(b))?(a):(b))
#endif

typedef struct ScaledNote
{
	int32_t oct;
	int32_t note;
} ScaledNote;

#define NOTE(aoct, anote) { SN.note = anote;SN.oct = aoct;};
#define NOTEOFF() { SN.note = TUESDAY_NOTEOFF;SN.oct = 0;};

#define ALGO_ENABLE_SLIDES 16
#define ALGO_ENABLE_LENGTHS 32

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
	
	ALGO_WOBBLE,
	ALGO_CHIPARP1,
	ALGO_CHIPARP2,

	ALGO_SNH,
	// classic saiko things, reimagined
	ALGO_SAIKO_CLASSIC,
	ALGO_SAIKO_LEAD,

	ALGO_SCALEWALKER,
	ALGO_TOOEASY,
	ALGO_RANDOM,
	__BUILTIN_ALGO_COUNT
} TUESDAY_ALGO;

#define ALGO_COUNT 16

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
	uint32_t LastVal;
	int32_t Target;
	int32_t Current;
	int32_t CurrentDelta;
	struct EURORACK_SVF Filt;
} PatternStruct_Algo_SNH; 

typedef struct PatternStruct_Algo_Chip1
{
	struct Tuesday_RandomGen R;
	uint32_t ChordSeed;
	unsigned char Base;
	unsigned char Dir:1;
} PatternStruct_Algo_Chip1;

typedef struct PatternStruct_Algo_Chip2
{
	struct Tuesday_RandomGen R;
	uint32_t ChordSeed;
	unsigned char len;
	unsigned char offset;
	unsigned char chordscaler;
	unsigned char TimeMult;
	unsigned char DeadTime;
	unsigned char idx;
	unsigned char dir;
	unsigned char ChordLen;
} PatternStruct_Algo_Chip2;

typedef struct PatternStruct_Algo_ScaleWalk
{
	struct Tuesday_RandomGen R;
	unsigned char WalkLen;
	unsigned char Current;
} PatternStruct_Algo_ScaleWalk;

typedef struct PatternStruct_Algo_TooEasy
{
	struct Tuesday_RandomGen R;
	unsigned char WalkLen;
	unsigned char Current;
	unsigned char Pattern[16];
} PatternStruct_Algo_TooEasy; 

typedef struct Tuesday_PatternFuncSpecific
{
	union
	{
		struct PatternStruct_Algo_GENERIC GENERIC;
		struct PatternStruct_Algo_Stomper Stomper;
		struct PatternStruct_Algo_Markov Markov;
		struct PatternStruct_Algo_Test Test;
		struct PatternStruct_Algo_Wobble Wobble;
		struct PatternStruct_Algo_SNH SNH;
		struct PatternStruct_Algo_Chip1 Chip1;
		struct PatternStruct_Algo_Chip2 Chip2;
		struct PatternStruct_Algo_ScaleWalk ScaleWalk;
		struct PatternStruct_Algo_TooEasy TooEasy;
	};
	struct Tuesday_RandomGen ExtraRandom;
} Tuesday_PatternFuncSpecific;

int ScaleToNote( ScaledNote *SN,  Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S);

typedef void(*GenFuncPtr)( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *PS, int I,  Tuesday_Tick_t *Output);
typedef void(*InitFuncPtr)( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *Output);
typedef void(*PatternInitFuncPtr)( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_PatternContainer *PT);

void NoInit( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *Output);
void NoPattern( Tuesday_PatternGen *T, Tuesday_Params *P, Tuesday_Settings *S, Tuesday_RandomGen *R, Tuesday_PatternFuncSpecific *PS, int I,  Tuesday_Tick_t *Output);
void NoPatternInit( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_PatternContainer *PT);

void Pattern_Rotate( Tuesday_PatternContainer *T, int first, int length, int rotate);
void Pattern_Reverse( Tuesday_PatternContainer *T, int first, int length);
void Pattern_Transpose( Tuesday_PatternContainer *T, int first, int length, int transpose);

void DefaultTick(Tuesday_Tick_t *Out);
void RandomSlideAndLength(Tuesday_Tick_t *Out,  Tuesday_RandomGen *R);

int Tuesday_Rand(struct Tuesday_RandomGen *R);
uint8_t Tuesday_BoolChance( Tuesday_RandomGen *R);
uint8_t Tuesday_RandByte( Tuesday_RandomGen *R);

typedef struct PatternFunctions
{
	GenFuncPtr Gen;
	InitFuncPtr Init;
	PatternInitFuncPtr PatternInit;
	uint8_t Dither;
} PatternFunctions;

uint8_t Tuesday_PercChance(struct Tuesday_RandomGen *R, uint8_t perc);

void Algo_Saiko_Lead_Gen( Tuesday_PatternGen *T,  Tuesday_Params *P,  Tuesday_Settings *S,  Tuesday_RandomGen *R,  Tuesday_PatternFuncSpecific *PS, int I,  Tuesday_Tick_t *Output);



#endif
