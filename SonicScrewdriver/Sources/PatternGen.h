#pragma once
#include <stdint.h>

#define PATTERNGEN_MAXTICK 128

struct PatternGen_Tick
{
	unsigned char vel;
	unsigned char note;
	unsigned char accent ;
};

struct PatternGen_Target
{
	struct PatternGen_Tick Ticks[PATTERNGEN_MAXTICK];
	unsigned char TPB;
	unsigned char Length;
};

struct PatternGen_Random
{
	long RandomMemory;
};

extern void PatternGen_RandomSeed(struct PatternGen_Random *R, unsigned int seed);

extern void PatternGen_Goa(struct PatternGen_Target *T, struct PatternGen_Random *R, int Length);
extern void PatternGen_Flat(struct PatternGen_Target *T, struct PatternGen_Random *R, int Length);
extern void PatternGen_Psych(struct PatternGen_Target *T, struct PatternGen_Random *R, int Length);
extern void PatternGen_Zeph(struct PatternGen_Target *T, struct PatternGen_Random *R, int stepsperbeat, int beats, int fullcycles);

struct PatternGen_Settings
{
	uint8_t tpboptions[4];
	uint8_t beatoptions[4];
	uint8_t scale[4][12];
	uint8_t scalecount[4];
	uint8_t algooptions[16];
};


struct PatternGen_Params
{
	uint8_t tpbopt;
	uint8_t beatopt;
	uint8_t scale;
	uint8_t algo;

	uint8_t seed1;
	uint8_t seed2;
};

extern void PatternGen_LoadSettings(struct PatternGen_Settings *S, struct PatternGen_Params *P);
extern void PatternGen_Generate(struct PatternGen_Target *T, struct PatternGen_Params *P, struct PatternGen_Settings *S);
