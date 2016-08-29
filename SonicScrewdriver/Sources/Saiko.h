#pragma once

#define SAIKO_MAXTICK 96
struct Tick
{
	float vel;
	int note;
	int accent ;
};

struct PatternTarget
{
	struct Tick Ticks[SAIKO_MAXTICK];
	int TPB;
	int Length;
};


extern void PatternGen_Goa(struct PatternTarget *T, int Length);
extern void PatternGen_Flat(struct PatternTarget *T, int Length);
extern void PatternGen_Psych(struct PatternTarget *T, int Length);

extern void PatternGen_Zeph(struct PatternTarget *T, int stepsperbeat, int beats, int fullcycles);


extern void  ZRANDOMSEED(unsigned int seed);
