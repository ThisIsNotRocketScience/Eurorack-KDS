#pragma once
#ifndef MAINH
#define MAINH

extern void DoClock(int state);
extern volatile int measured;

enum
{
	ADC_INX,
	ADC_INY,
	ADC_INTENSITY,
	ADC_TEMPO,
	ADC_Count
};
extern int clockup;
extern int Measure;

extern int timesincelastclocktick;
extern int adcchannels[ADC_Count];
extern void doTimer();
extern void PatternReset(int val);
extern void ExtClockTick(int state);
extern volatile int i2csending;
extern volatile int i2creceiving;
#endif
