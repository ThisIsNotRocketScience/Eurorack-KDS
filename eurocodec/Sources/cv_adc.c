#include "cv_adc.h"

#include "Cpu.h"
#include "AD1.h"
#include "DA1.h"
#include "WAIT1.h"

void cv_adc_init()
{
	cv_adc_recalibrate();
}

static float cv_adc_calibration_scale;
static float adc_calibration_scale;

float cv_adc_recalibrate()
{
	AD1_CancelMeasurement(AD1_DeviceData);

	AD1_SelectSampleGroup(AD1_DeviceData, 1);

	AD1_StartSingleMeasurement(AD1_DeviceData);
	WAIT1_Waitus(10);
	AD1_StartSingleMeasurement(AD1_DeviceData);
	WAIT1_Waitus(10);
	int32_t vref_per_vcc = 0;
	AD1_GetMeasuredValues(AD1_DeviceData, &vref_per_vcc);

	AD1_SelectSampleGroup(AD1_DeviceData, 0);
	AD1_StartLoopMeasurement(AD1_DeviceData);

	adc_calibration_scale = (2.0f / 1.2f) * (float)vref_per_vcc;

	cv_adc_calibration_scale = (5.0f * 1.2f) / (float)vref_per_vcc;
}

float cv_adc_voltage()
{
	int32_t adcvalue = 0;
	AD1_GetMeasuredValues(AD1_DeviceData, &adcvalue);

	float result = (float)adcvalue * cv_adc_calibration_scale - 1.2f;
	return result;
}

void dac_set_voltage(float v)
{
	  uint32_t intvalue = (uint32_t)(v * adc_calibration_scale);
	  DA1_SetValue(DA1_DeviceData, intvalue);
}

float adc_voltage(int channel)
{
	return (float)adcvalues[channel] * adc_calibration_scale;
}
