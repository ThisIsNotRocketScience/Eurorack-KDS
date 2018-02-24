#include "waveguide.h"
#include <math.h>


int32_t WaveGuide_GetDirect(WaveGuide_t *wg,float Pos)
{
	float back=wg->counter-Pos;

	while (back<0.0) back+=MAX_WG_DELAY;
	while (back>MAX_WG_DELAY) back-=MAX_WG_DELAY;

	int const index0=(int)(floor(back));
	return wg->buffer[index0]<<16;
}

__attribute__( ( section(".data") ) )float InterpolateCubic(float x0, float x1, float x2, float x3, float t)
{
	float a0, a1, a2, a3;
	a0 = x3 - x2 - x0 + x1;
	a1 = x0 - x1 - a0;
	a2 = x2 - x0;
	a3 = x1;
	return (a0 * (t * t * t)) + (a1 * (t * t)) + (a2 * t) + (a3);
}

void WaveGuide_Init(WaveGuide_t *wg)
{
	wg->counter=0;
	for(int s=0;s<MAX_WG_DELAY;s++) wg->buffer[s]=0;
}


 float WaveGuide_Get(WaveGuide_t *wg,float Pos)
{
	 int npos =(int)( Pos * 65535.0f);
	int nback = (wg->counter <<16) - npos;

	while (nback < 0) nback += (MAX_WG_DELAY<<16);
	while (nback > (MAX_WG_DELAY<<16)) nback -= (MAX_WG_DELAY<<16);

	int const index0=nback>>16;
	int  index1=index0+1;

	if(index1>=MAX_WG_DELAY) index1-=MAX_WG_DELAY;
		int16_t const y0 = (wg->buffer [index0]);
	int16_t const y1 = (wg->buffer [index1]);

	uint16_t x= (npos &0xffff);
	x = ~x;
	return (y0<<16) + ((y1-y0)*x);
}

inline void WaveGuide_Push(WaveGuide_t *wg, int32_t in)
{
	wg->buffer[wg->counter]=in>>16;
	wg->counter++;
	if(wg->counter>=MAX_WG_DELAY) wg->counter=0;
}

void WaveGuideLong_Init(WaveGuideLong_t *wg)
{
	wg->counter=0;
	for(int s=0;s<MAX_WG_DELAY_LONG;s++) wg->buffer[s]=0;
}


 float WaveGuideLong_Get(WaveGuideLong_t *wg,float Pos)
{
	 int npos =(int)( Pos * 65535.0f);
	int nback = (wg->counter <<16) - npos;

	while (nback < 0) nback += (MAX_WG_DELAY_LONG<<16);
	while (nback > (MAX_WG_DELAY_LONG<<16)) nback -= (MAX_WG_DELAY_LONG<<16);

	int const index0=nback>>16;
	int  index1=index0+1;

	if(index1>=MAX_WG_DELAY_LONG) index1-=MAX_WG_DELAY_LONG;
	int16_t const y0 = (wg->buffer [index0]);
	int16_t const y1 = (wg->buffer [index1]);

	uint16_t x= (npos &0xffff);
	x = ~x;
	return (y0<<16) + ((y1-y0)*x);
}


 int16_t WaveGuideLong_GetI16(WaveGuideLong_t *wg,float Pos)
 {
 	 int npos =(int)( Pos * 65535.0f);
 	int nback = (wg->counter <<16) - npos;

 	while (nback < 0) nback += (MAX_WG_DELAY_LONG<<16);
 	while (nback > (MAX_WG_DELAY_LONG<<16)) nback -= (MAX_WG_DELAY_LONG<<16);

 	int const index0=nback>>16;
 	int  index1=index0+1;

 	if(index1>=MAX_WG_DELAY_LONG) index1-=MAX_WG_DELAY_LONG;
 	int16_t const y0 = (wg->buffer [index0]);
 	int16_t const y1 = (wg->buffer [index1]);

 	uint16_t x= (npos &0xffff);
 	x = ~x;
 	return y0 + (((y1-y0)*x)>>16);
 }

 int16_t WaveGuide_GetI16(WaveGuide_t *wg,float Pos)
 {
 	 int npos =(int)( Pos * 65535.0f);
 	int nback = (wg->counter <<16) - npos;

 	while (nback < 0) nback += (MAX_WG_DELAY<<16);
 	while (nback > (MAX_WG_DELAY<<16)) nback -= (MAX_WG_DELAY<<16);

 	int const index0=nback>>16;
 	int  index1=index0+1;

 	if(index1>=MAX_WG_DELAY) index1-=MAX_WG_DELAY;
 	int16_t const y0 = (wg->buffer [index0]);
 	int16_t const y1 = (wg->buffer [index1]);

 	uint16_t x= (npos &0xffff);
 	x = ~x;
 	return y0 + (((y1-y0)*x)>>16);
 }

 inline void WaveGuideLong_Push(WaveGuideLong_t *wg, int32_t in)
{
	wg->buffer[wg->counter]=in>>16;
	wg->counter++;
	if(wg->counter>=MAX_WG_DELAY_LONG) wg->counter=0;
}
