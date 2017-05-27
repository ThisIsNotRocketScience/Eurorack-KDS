#pragma once
#ifndef ORGANOSCS
#define ORGANOSCS

#include<stdint.h>
#include "BigFishDefs.h"


typedef struct Organ_t
{
	uint32_t mPhase;
	uint32_t mPhase2;
	uint32_t mPhaseIncrement;
	uint32_t mPhaseIncrement2;
	uint32_t spread;
} Organ_t;

void Organ_Init(struct Organ_t *osc);
void Organ_Update(struct Organ_t *osc, float odsr, float centerfreq, float size, float spread);
float Organ_Get(struct  Organ_t *osc);


#endif