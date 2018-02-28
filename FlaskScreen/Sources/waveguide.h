#ifndef WAVEGUIDES
#define WAVEGUIDES
#include <stdint.h>

#define MAX_WG_DELAY_SHORT (10000/4)
#define MAX_WG_DELAY (10000/2)
#define MAX_WG_DELAY_LONG (10000)

typedef struct WaveGuideLong_t
{
	int16_t buffer[MAX_WG_DELAY_LONG];
	int counter;
} WaveGuideLong_t;

typedef struct WaveGuideShort_t
{
	int16_t buffer[MAX_WG_DELAY_SHORT];
	int counter;
} WaveGuideShort_t;


typedef struct WaveGuide_t
{
	int16_t buffer[MAX_WG_DELAY];
	int counter;
} WaveGuide_t;



extern int32_t WaveGuide_GetDirect(WaveGuide_t *wg,float Pos);
extern void WaveGuide_Init(WaveGuide_t *wg);
extern float WaveGuide_Get(WaveGuide_t *wg,float Pos);
extern void WaveGuide_Push(WaveGuide_t *wg, int32_t in);
extern int16_t WaveGuide_GetI16(WaveGuide_t *wg,float Pos);


extern void WaveGuideLong_Init(WaveGuideLong_t *wg);
extern float WaveGuideLong_Get(WaveGuideLong_t *wg,float Pos);
extern int16_t WaveGuideLong_GetI16(WaveGuideLong_t *wg,float Pos);
extern void WaveGuideLong_Push(WaveGuideLong_t *wg, int32_t in);

extern void WaveGuideShort_Init(WaveGuideShort_t *wg);
extern float WaveGuideShort_Get(WaveGuideShort_t *wg,float Pos);
extern int16_t WaveGuideShort_GetI16(WaveGuideShort_t *wg,float Pos);
extern void WaveGuideShort_Push(WaveGuideShort_t *wg, int32_t in);


#endif
