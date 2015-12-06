#pragma once

#include <stdint.h>
#include "AD1.h"

void cv_adc_init();
float cv_adc_recalibrate();

float cv_adc_voltage();
//int32_t cv_adc_value();

extern uint16_t adcvalues[];

static inline int32_t cv_adc_value()
{
	int32_t adcvalue = 0;
	AD1_GetMeasuredValues(AD1_DeviceData, &adcvalue);

	return adcvalue >> 16;
}

void dac_set_voltage(float v);

float adc_voltage(int channel);

static inline uint16_t adc_value(int channel)
{
	return adcvalues[channel];
}
