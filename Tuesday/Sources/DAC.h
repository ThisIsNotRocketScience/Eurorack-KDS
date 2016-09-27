#pragma once
#ifndef SPIDAC
#define SPIDAC

extern void DAC_Write(int channel, int value);
extern void DAC_Done();
extern unsigned char DACSENDDONE;
#endif
