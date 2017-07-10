#pragma once
#ifndef MAINH
#define MAINH

extern void DoClock(int state);
extern volatile int measured;

enum
{
	ADC_MODULATION,
	ADC_PHASING,
	ADC_SHAPE,
	ADC_SPEED,
	ADC_AMTNORMAL,
	ADC_AMTPHASED,
	ADC_Count
};
extern int clockup;
extern int Measure;

extern int timesincelastclocktick;
extern int adcchannels[ADC_Count];
extern void doTimer();
extern void EnvelopeReset();
extern void EnvelopeTrigger(int sw);

extern volatile int i2csending;
extern volatile int i2creceiving;
#endif
