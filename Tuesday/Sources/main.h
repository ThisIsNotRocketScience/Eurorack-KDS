#pragma once
#ifndef MAINH
#define MAINH

extern void DoClock(int state);
extern int measured;
extern int adcchannels[6];
extern void doTimer();
extern void PatternReset();

extern volatile int i2csending;
extern volatile int i2creceiving;
#endif
