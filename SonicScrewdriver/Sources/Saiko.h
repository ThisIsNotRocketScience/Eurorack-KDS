#pragma once

struct Tick
{
	float vel;
	int note;
	int accent ;
};

struct PatternTarget
{
	struct Tick Ticks[256];
};


extern void PatternGen_Goa(struct PatternTarget *T);
extern void PatternGen_Flat(struct PatternTarget *T);
extern void PatternGen_Psych(struct PatternTarget *T);
extern void  szrand (unsigned int seed);
