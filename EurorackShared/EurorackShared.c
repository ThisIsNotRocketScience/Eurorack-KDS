#include "EurorackShared.h"

#ifndef WIN32
#include "DAC.c"
#include "Eeprom.c"
#include "LEDS.c"
#endif


struct EuroRack_Calibration MasterCalibration;

void InitDAC(struct EuroRack_DAC_Calibration *DAC)
{
	DAC->offset = 0;
	DAC->scale = 0x100;
}

void InitADC(struct EuroRack_ADC_Calibration *ADC)
{
	ADC->offset = 0;
	ADC->scale = 0x100;
}

void EuroRack_InitCalibration()
{
	for (int i = 0; i < EURORACK_MAX_ADC; i++)
	{
		InitADC(&MasterCalibration.ADC[i]);
	}

	for (int i = 0; i < EURORACK_MAX_DAC; i++)
	{
		InitDAC(&MasterCalibration.DAC[i]);
	}
}

int denoise(int sw_down, struct denoise_state_t *state)
{
	if (sw_down)
		state->counter++;
	else
		state->counter--;
	state->pressed = 0;
	state->released = 0;

	if (state->counter < 2)
	{
		if (state->lastcounter == 2)
		{
			state->pressed = 1;
		}
		state->counter = 1;
		state->down = 1;
	}
	else if (state->counter > 30)
	{
		if (state->lastcounter == 30)
		{
			state->released = 1;
		}
		state->counter = 31;
		state->down = 0;
	}
	state->lastcounter = state->counter;
	return state->pressed;
}

