
#include "GoldfishGlobals.h"

#ifndef TABLES
#define TABLES
#include "DistortionTable_1.h"
#include "DistortionTable_2.h"
#endif

FISHTYPE Fish[2];
PlatinumClip Dist[2];

#include "GATE.h"

extern "C"{
#include "../cv_adc.h"
#include "../dsplib/dpw_osc.h"
#include "../dsplib/cvsmooth.h"
#include "../ak4558.h"

extern int dsp_switch_octave ;
extern int dsp_switch_gate ;
extern int dsp_led_octave ;
extern int dsp_led_gate;

extern int gate ;
}

static struct dpw_sawtooth_state_t state1;
static struct dpw_pulse_state_t state2;
static struct cvsmooth_state_t pw_smooth_state;


#include "Goldfish_Interface.h"

extern "C"
{

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
void GoldfishProcess(int32_t *in, int32_t * out, int32_t frames)
{

	int32_t pitch = cv_adc_value() + (adc_value(0) >> 2);

	if (dsp_switch_octave) pitch += 1985; // 1V*.2/3.3V*32768
	pitch += 2*1985; // scale pitch to 1/4 samplerate

	// 0 = cutoff
	// 1 = resonance;
	// 2 = fenv amt  (mid = 0x5500)
	// 3 =  decay (mid = 8192)
	// 4 = drive
	// 5 = tune

	int drvtype = 0;
	int drive = drive- 8192;
	if (drive < 0)
	{
		drive = -drive;
		drvtype = 1;
	}
	int32_t fenvamt =( int32_t ) adc_value(2)- 0x5500;
	int32_t Cut = (adc_value(0)*127) / 0x97a0;
	int32_t Res = (adc_value(1)*127)/ 0xfff0;

		for (int i =0 ;i<2;i++)
	{
		Fish[i].FenvAmt = fenvamt ;


		Fish[i].Event(CUTOFF, Cut, 0);
		Fish[i].Event(RESONANCE, Res, 0);
	}

	if ((dsp_switch_gate || !GATE_GetVal(GATE_DeviceData)) && gate == 0)
	{
		gate = 1500 / 32;

		Fish[0].Event(NOTE_ON, 48 + (dsp_switch_octave) * 12 ,0);
		Fish[1].Event(NOTE_ON, 48 + (dsp_switch_octave) * 12 ,0);

		n++;
	}

	dsp_led_octave = dsp_switch_octave;

	if (gate)
	{
		dsp_led_gate = 1;
		gate--;
	}
	else
	{
		dsp_led_gate = 0;
	}

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
