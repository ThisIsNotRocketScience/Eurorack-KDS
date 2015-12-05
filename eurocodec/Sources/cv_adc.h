#pragma once

void cv_adc_init();
float cv_adc_recalibrate();

float cv_adc_voltage();
void dac_set_voltage(float v);
