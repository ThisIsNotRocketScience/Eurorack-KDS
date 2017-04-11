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
	int16_t volt_3;
	int16_t volt_1;
};

struct EuroRack_Calibration
{
	struct EuroRack_ADC_Calibration ADC[6];
	struct EuroRack_DAC_Calibration DAC[2];
};


struct denoise_state_t
{
	int counter;
	int down;
	unsigned char pressed:4;
	unsigned char released:4;
	int longpressed;
	int lastcounter;
};

struct EURORACK_SVF
{
	uint16_t Cutoff;
	uint32_t Resonance;
	int32_t lo;
	int32_t mid;
	int32_t hi;
};


#ifdef __cplusplus
extern "C"
{
#endif


	void EuroRack_InitCalibration();
	uint8_t CalibratedADC(int adcchannel, uint32_t Input);
	uint16_t CalibratedDAC(int dacchannel, uint32_t Input);


	void denoise(int sw_down, struct denoise_state_t *state);
	int islongpress(struct denoise_state_t *state);
	int pressed(struct denoise_state_t *state);

	/// A sine approximation via a fourth-order cosine approx.
	/// @param x   angle (with 2^15 units/circle)
	/// @return     Sine value (Q12)
	int32_t isin_S4(int32_t x);
	int32_t LERP(int32_t *V, int total, int fade);

	int32_t Sine(uint32_t phase);
	int32_t SawTooth(uint32_t phase);
	int32_t Pulse(uint32_t phase);
	int32_t Triangle(uint32_t phase);
	int32_t BasicShapes(uint32_t phase, int mod);

	void ResetSVF(struct EURORACK_SVF *filt);
	void SetSVF(struct EURORACK_SVF *filt, uint16_t cut, uint16_t res);
	void ProcessSVF(struct EURORACK_SVF *filt, uint32_t RR);


#ifdef __cplusplus
}
#endif


#endif
