#pragma once
#ifndef ORGANOSCS
#define ORGANOSCS

#include<stdint.h>
#include "BigFishDefs.h"


typedef struct Organ_t
{
	float mPhase;
	float mPhase2;
	float mPhaseIncrement;
	float mPhaseIncrement2;
	float spread;
} Organ_t;

void Organ_Init(struct Organ_t *osc);
void Organ_Update(struct Organ_t *osc, int samplerate, float centerfreq, float size, float spread);
float Organ_Get(struct  Organ_t *osc);


#endif