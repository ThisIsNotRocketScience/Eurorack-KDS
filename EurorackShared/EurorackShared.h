#ifndef EURORACKCALIBRATION
#define EURORACKCALIBRATION

#include <stdint.h>

#define EURORACK_MAX_ADC 6
#define EURORACK_MAX_DAC 2

struct EuroRack_ADC_Calibration
{
	int16_t offset;
	int16_t scale;
};

struct EuroRack_DAC_Calibration
{
	int16_t offset;
	int16_t scale;
};

struct EuroRack_Calibration
{
	struct EuroRack_ADC_Calibration ADC[6];
	struct EuroRack_DAC_Calibration DAC[2];
};

void EuroRack_InitCalibration();
uint8_t CalibratedADC(int adcchannel, uint32_t Input);
uint16_t CalibratedDAC(int dacchannel, uint32_t Input);

struct denoise_state_t
{
	int counter;
	int down;
	int pressed;
	int released;
	int lastcounter;
};

int denoise(int sw_down, struct denoise_state_t *state);




#endif