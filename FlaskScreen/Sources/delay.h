#ifndef DELAYLINCLUDED
#define DELAYLINCLUDED
#pragma once
#include <stdint.h>
#include "waveguide.h"
typedef struct Delay
{
	WaveGuideLong_t Line;
	//int16_t buffer[delaylen] ;

	int delayposleft ;
	int delayposcenter ;
	int delayposright ;
	float delaylenleft ;
	float delaylenright;
	int32_t wet ;
	int32_t dry ;
	int32_t feedback;
	int32_t lin ;
	int32_t rin ;
	int mono ;
} Delay;


typedef struct XDelay
{
	WaveGuide_t LineL;
	WaveGuide_t LineR;

	float CrossOver;
	uint16_t Feedback;
	float delaylenleft ;
	float delaylenright;
	float Wet;
} XDelay;



extern void ProcessDelay(Delay* D, int32_t *in, int32_t *out);
extern void InitDelay(Delay *D);

extern void ProcessXDelay(XDelay* D, int32_t *in, int32_t *out);
extern void InitXDelay(XDelay *D);

#endif
