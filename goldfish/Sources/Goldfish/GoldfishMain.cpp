
#include "GoldfishGlobals.h"

#ifndef TABLES
#define TABLES
#include "DistortionTable_1.h"
#include "DistortionTable_2.h"
#endif

Goldfish Fish[2];
PlatinumClip Dist[2];

#include "GATE.h"
#include "AD1.h"


extern "C"{
#include "../cv_adc.h"
#include "../dsplib/dpw_osc.h"
#include "../dsplib/cvsmooth.h"
#include "../ak4558.h"

extern int dsp_switch_octave ;
extern int dsp_switch_gate ;
extern int dsp_led_octave ;
extern int dsp_led_gate;
extern int GatePressed;
extern int gate ;
}

static struct dpw_sawtooth_state_t state1;
static struct dpw_pulse_state_t state2;
static struct cvsmooth_state_t pw_smooth_state;


#include "Goldfish_Interface.h"
int Octave = 2;
extern int32_t pitchadcvalue;

int32_t get_cv_adc_value()
{

	return pitchadcvalue >> 16;
}


extern "C"
{

void GoldfishOctavePressed(){
	Octave = (Octave + 1) % 4;
}

void GoldfishInit()
{
	for (int i =0 ;i<2;i++)
	{
		Dist[i].Init(48000);
		Fish[i].m_Dist = &Dist[i];
		Fish[i].Init();

		Fish[i].Event(ALL_NOTES_OFF, 0, 0);
		Fish[i].Event(CUTOFF, 40, 0);
		Fish[i].Event(RESONANCE, 90, 0);
		Fish[i].Event(FENV, 80, 0);
		Fish[i].Event(FDEC, 80, 0);
		Fish[i].Event(GLIDE, 4, 0);
	}

	Fish[0].Event(WAVEFORM, 0, 0);
	Fish[1].Event(WAVEFORM, 127, 127);

}

signed short SawFishBuffer[AUDIO_BUFFER_SIZE];
signed short PulseFishBuffer[AUDIO_BUFFER_SIZE];
int n = 0;

int setdrive = 0;
int setfuzz = 0;
int lastgate = 0;
void GoldfishProcess(int32_t *in, int32_t * out, int32_t frames)
{

	int32_t pitchcv = get_cv_adc_value();

	int32_t pitch = pitchcv + (adc_value(5) >> 4);
	pitch -= 1985*2;
	pitch += Octave * 1985;

//	if (dsp_switch_octave) pitch += 1985; // 1V*.2/3.3V*32768
	pitch += 2*1985; // scale pitch to 1/4 samplerate

	// 0 = cutoff
	// 1 = resonance;
	// 2 = fenv amt  (mid = 0x5500)
	// 3 = decay
	// 4 = drive (mid = 0x8192)
	// 5 = tune

	int drvtype = 0;
	int32_t drive = adc_value(4) - 0x8000;
	int32_t decay = (((int32_t)adc_value(3) * 127))>>16;
	if (!setfuzz && drive < -0x800) {
		drvtype = 1;
	}
	else if (setfuzz && drive > 0x800) {
		drvtype = 0;
	}
	else {
		drvtype = setfuzz;
	}
	if (drvtype) {
		drive = -drive;
	}
	if (drive < 0)
	{
		drive = 0;
	}
	drive = (drive *127)/0x8000;
	drive = drive>>2;
		drive = drive<<2;

	int32_t fenvamt =( int32_t ) adc_value(2)- (int32_t)(0x5500);

	int32_t Cut = (adc_value(0)*127) / 0x97a0;
	int32_t Res = (adc_value(1)*127)/ 0xfff0;

	if (((dsp_switch_gate && lastgate == 0)|| (GATE_GetVal(GATE_DeviceData)  == FALSE) || GatePressed == 1) && gate == 0)
	{
		gate = 1500 / 32;
		GatePressed = 0;
		Fish[0].Event(NOTE_ON, 0,0);
		Fish[1].Event(NOTE_ON, 0,0);


	}
	lastgate = dsp_switch_gate;

	for (int i =0 ;i<2;i++)
	{
		Fish[i].FenvAmt = fenvamt ;


		Fish[i].Event(CUTOFF, Cut, 0);
		Fish[i].Event(FDEC, decay, 0);
		Fish[i].Event(RESONANCE, Res, 0);
		Fish[i].SetPitch(pitch);
	}

	if (setdrive != drive || setfuzz != drvtype)
	{

		for (int i =0 ;i<2;i++)
		{

			if (drvtype)
			{
				Dist[i].fuzz = true;
			}
			else
			{
				Dist[i].fuzz = false;

			}
			Dist[i].SetPush( drive);
		}
		setdrive = drive;
		setfuzz = drvtype;
	}




	dsp_led_octave =  Octave * 64;


	if (gate)		gate--;

	dsp_led_gate = Fish[0].FEnvCurVal >> 24;
	//if (dsp_led_gate >0 )dsp_led_gate += 128;
	if (Fish[0].Render(PulseFishBuffer, frames, &Dist[0]) == false)
	{
		for (int i = 0; i < frames ; i++)
		{
			PulseFishBuffer[i] = 0;
		}

	}

	if (Fish[1].Render(SawFishBuffer, frames, &Dist[1]) == false)
	{
		for (int i = 1; i < frames ; i++)
		{
			SawFishBuffer[i] = 0;
		}

	}


	for (int i =0 ;i<frames;i++)
	{
		*out++ = PulseFishBuffer[i] << 16;
		*out++ = SawFishBuffer[i] << 16;
	}


	uint32_t pulsewidth = cvsmooth(adc_value(2) << 16, &pw_smooth_state);
}


} // extern "C"
