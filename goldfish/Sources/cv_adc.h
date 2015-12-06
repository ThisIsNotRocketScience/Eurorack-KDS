#pragma once

#include <stdint.h>

void cv_adc_init();
float cv_adc_recalibrate();

float cv_adc_voltage();
int32_t cv_adc_value();

void dac_set_voltage(float v);

float adc_voltage(int channel);
uint16_t adc_value(int channel);
