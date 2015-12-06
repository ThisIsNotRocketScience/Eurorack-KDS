
#include "GoldfishGlobals.h"

#ifndef TABLES
#define TABLES
#include "DistortionTable_1.h"
#include "DistortionTable_2.h"
#endif

FISHTYPE Fish;
pc Dist;




void GoldfishSetup()
{



	for (int i = 0; i < MAXFISH; i++)
	{
		Fish.Event(ALL_NOTES_OFF, 0, 0);

		Fish.Event(CUTOFF, 40, 0);
		Fish.Event(RESONANCE, 90, 0);
		Fish.Event(FENV, 80, 0);
		Fish.Event(FDEC, 80, 0);
		Fish.Event(WAVEFORM, 0, 0);
		Fish.Event(GLIDE, 4, 0);
	}

}


