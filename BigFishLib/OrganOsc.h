#pragma once
#ifndef ORGANOSCS
#define ORGANOSCS

#include<stdint.h>
#include "BigFishDefs.h"


typedef struct OrganVoice_t
{
	uint32_t mPhase;
	uint32_t mPhaseIncrement;
	uint32_t spread;
} OrganVoice_t;

typedef struct Organ_t
{
	uint32_t mPhase2;
	uint32_t mPhaseIncrement2;
	int32_t Aoff;
	int32_t Boff;
	int32_t Coff;
	OrganVoice_t Voices[3];
} Organ_t;


void Organ_Init(struct Organ_t *osc, float odsr);
void Organ_Update(struct OrganVoice_t *osc, float odsr, float centerfreq, float size, float spread);
int32_t Organ_Get(struct  Organ_t  *osc, struct  OrganVoice_t *voice);
int32_t Organ_GetMain(struct  Organ_t  *osc);


#endif