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
