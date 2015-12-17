#pragma once

#include <stdint.h>
#include "AD1.h"

void cv_adc_init();
float cv_adc_recalibrate();

float cv_adc_voltage();
//int32_t cv_adc_value();

extern uint16_t adcvalues[];

extern int32_t cv_adc_current;

static inline int32_t cv_adc_value()
{
	//if (*(uint8_t*)AD1_DeviceData) {
	//	value = ADC_PDD_GetResultValueRaw(ADC1_BASE_PTR, 0);
	//}
	return cv_adc_current;

	//int32_t adcvalue;
	//AD1_GetMeasuredValues(AD1_DeviceData, &adcvalue);
	//return adcvalue >> 16;
}

void dac_set_voltage(float v);

float adc_voltage(int channel);

static inline uint16_t adc_value(int channel)
{
	return adcvalues[channel];
}
