#include "BigFish.h"
#include "BleppyOscs.h"

//#include "bleptables.h"
#include "DistortionTable_1.h"
#include "DistortionTable_2.h"
//#include "bleptables.h"

#include <math.h>

float *blep;
int32_t *intblep;

#include "FormantTable.h"

const float chordtable[16][4] = {
	{ powf(2, 0.0f / 12.0f), 0, 0, 0 },
	{ powf(2, -0.1f / 12.0f), powf(2, 0.0f / 12.0f), powf(2, 0.1f / 12.0f), powf(2, 12.05f / 12.0f) }, // Fat Moog
	{ powf(2, -0.1f / 12.0f), powf(2, 0.0f / 12.0f), powf(2, 7.04f / 12.0f), powf(2, 0.1f / 12.0f) }, // Moog 5th
	{ powf(2, -0.18f / 12.0f), powf(2, -0.07f / 12.0f), powf(2, 0.07f / 12.0f), powf(2, 0.18f / 12.0f) }, // Guru Ps1
	{ powf(2, -0.3f / 12.0f), powf(2, -0.15f / 12.0f), powf(2, 0.15f / 12.0f), powf(2, 0.3f / 12.0f) }, // Guru Ps2
	{ powf(2, -0.1f / 12.0f), powf(2, 0.1f / 12.0f), powf(2, 12.02f / 12.0f), powf(2, 16.07f / 12.0f) }, // VengaBoyz:(

	{ powf(2, 0.0f / 12.0f), powf(2, 4.0f / 12.0f), powf(2, 7.0f / 12.0f), 0 }, // major 4-7
	{ powf(2, 0.0f / 12.0f), powf(2, 3.0f / 12.0f), powf(2, 7.0f / 12.0f), 0 }, // minor 3-7
	{ powf(2, 0.0f / 12.0f), powf(2, 4.0f / 12.0f), powf(2, 8.0f / 12.0f), 0 }, // minor 4-8
	{ powf(2, 0.0f / 12.0f), powf(2, 3.0f / 12.0f), powf(2, 8.0f / 12.0f), 0 }, // major 3-8
	{ powf(2, 0.0f / 12.0f), powf(2, 5.0f / 12.0f), powf(2, 8.0f / 12.0f), 0 }, // minor 5-8
	{ powf(2, 0.0f / 12.0f), powf(2, 5.0f / 12.0f), powf(2, 9.0f / 12.0f), 0 }, // major 5-9
	{ powf(2, 0.0f / 12.0f), powf(2, 5.0f / 12.0f), powf(2, 10.0f / 12.0f), powf(2, 1.0f / 12.0f) }, // minor 1-5-10
	{ powf(2, 0.0f / 12.0f), powf(2, 4.0f / 12.0f), powf(2, 7.0f / 12.0f), powf(2, 11.0f / 12.0f) }, // major 4-7-11
	{ powf(2, 0.0f / 12.0f), powf(2, 3.0f / 12.0f), powf(2, 7.0f / 12.0f), powf(2, 10.0f / 12.0f) }, // minor 3-7-10
	{ powf(2, 0.0f / 12.0f), powf(2, 3.0f / 12.0f), powf(2, 7.0f / 12.0f), powf(2, 9.0f / 12.0f) },  // minor 3-7-9

};



int32_t Inertia_Update(struct Inertia_t *inert)
{
	return inert->Current;
}

void Inertia_SetTarget(struct Inertia_t *inert, int32_t Target, int32_t Speed)
{
	inert->Target = Target;
	inert->Speed = Speed;
}


void BigFish_GateOn(struct BigFish_t *fish)
{
	// trigger main env
	// trigger filter env
	ADSR_Trigger(&fish->AmpEnvelope,1);
	ADSR_Trigger(&fish->FilterEnvelope,1);
}

void BigFish_GateOff(struct BigFish_t *fish)
{
	ADSR_Trigger(&fish->AmpEnvelope, 0);
}

void BigFish_AccentOn(struct BigFish_t *fish)
{
	Inertia_SetTarget(&fish->Accent, 1024, 100);
}

void BigFish_AccentOff(struct BigFish_t *fish)
{
	Inertia_SetTarget(&fish->Accent, 0, 100);
}

void Inertia_Init(struct Inertia_t *inert)
{
	inert->Current = 0;
	inert->Speed = 0;
	inert->Target = 0;
}

float DBToLevel(float db)
{
	return powf(10, (db / 20));
}

void BigFish_Init(struct BigFish_t *fish, int samplerate)
{



	blep = GenerateMinBLEP(24, 32);
	intblep = new int32_t[48 * 32];
	for (int i = 0; i < 48 * 32; i++)
	{
		intblep[i] = int(blep[i] * (1 << 24));
	};

	WaveBlepOsc_Init(&fish->WaveOsc);

	VosimOsc_Init(&fish->Vosim);
	fish->SampleRate = samplerate;
	fish->Clipper.Init(samplerate);	
	fish->PitchInput = 0;
	for (int i = 0; i < 7; i++)
	{
//		fish->filters[i]._mode = 0;
	//	fish->filters[i].set_samplerate(samplerate);
	}

	for (int i = 0; i < 10; i++) fish->FormantMemory[i] = 0;
	Inertia_Init(&fish->Accent);
	MinBlepOsc_Init(&fish->PulseOsc);
	MinBlepOsc_Init(&fish->SawOsc);
	HyperPulse_Init(&fish->HyperPulseOsc);
	HyperOsc_Init(&fish->HyperSawOsc);
	ADSR_BuildTable();
	ADSR_Init(&fish->AmpEnvelope, ENVMODE_GATE, 0);
	ADSR_Init(&fish->FilterEnvelope, ENVMODE_TRIGGER, 0);
}

void ADSR_ApplyParameters(struct ADSR_Envelope_t *adsr, int a, int d, int s, int r)
{
	adsr->A = a >> 6;
	adsr->D = d >> 6;
	adsr->S = s >> 6;
	adsr->R = r >> 6;
}

#define CHECKGATE(f, g, on,off) if (f->Gates[g] != f->PreviousGates[g]){f->PreviousGates[g] = f->Gates[g]; if(f->Gates[g]) on(f);else off(f);};

void BigFish_CheckGates(struct BigFish_t *fish)
{
	CHECKGATE(fish, FISHGATE_ACCENT, BigFish_AccentOn, BigFish_AccentOff);
	CHECKGATE(fish, FISHGATE_GATE, BigFish_GateOn, BigFish_GateOff);
}

void BigFish_Update(struct BigFish_t *fish)
{
	int Drive = fish->Parameters[FILTER_DRIVE];
	if (Drive > 32768)
	{
		Drive = (Drive - 32768);
		fish->Clipper.push = Drive >> 8;
		fish->Clipper.SetType(1);
	}
	else
	{
		fish->Clipper.push = (32768-Drive)>>8;
		fish->Clipper.SetType(0);
	}
	
	BigFish_CheckGates(fish);
	ADSR_ApplyParameters(&fish->AmpEnvelope, fish->Parameters[AMP_ATTACK], fish->Parameters[AMP_DECAY], fish->Parameters[AMP_SUSTAIN], fish->Parameters[AMP_RELEASE]);
	ADSR_ApplyParameters(&fish->FilterEnvelope, fish->Parameters[FILTER_ATTACK], fish->Parameters[FILTER_DECAY], 0,0);
}

void HyperSaw(struct BigFish_t *fish, int32_t *buffer, int len)
{
	float spread = (fish->Parameters[OSC_SPREAD]) / 65536.0f; 
	float size = (fish->Parameters[OSC_SIZE ]) / 65536.0f;
	HyperOsc_Update(&fish->HyperSawOsc, fish->SampleRate, fish->CenterFreq, size, spread);
	while (len--)
	{
		*buffer++ = HyperOsc_Get(&fish->HyperSawOsc) * (32768.0f / MAXHYPER);
	}
}

void HyperPulse(struct BigFish_t *fish, int32_t *buffer, int len)
{
	float spread = (fish->Parameters[OSC_SPREAD] ) / 65536.0f;
	float size = (fish->Parameters[OSC_SIZE]) / 65536.0f;
	HyperPulse_Update(&fish->HyperPulseOsc, fish->SampleRate, fish->CenterFreq, size, spread);
	while (len--)
	{
		*buffer++ = HyperPulse_Get(&fish->HyperPulseOsc) * (32768.0f / MAXHYPER);
	}
}

void SuperFish(struct BigFish_t *fish, int32_t *buffer, int len)
{
	MinBlepOsc_Update(&fish->SawOsc, fish->SampleRate, fish->CenterFreq, fish->Parameters[OSC_SIZE], fish->Parameters[OSC_SPREAD]);
	MinBlepOsc_Update(&fish->PulseOsc, fish->SampleRate, fish->CenterFreq * 2.0, fish->Parameters[OSC_SIZE], fish->Parameters[OSC_SPREAD]);
	float size = fish->Parameters[OSC_SIZE] / 2.0f;
	float invsize = 32768 - size;
	while (len--)
	{

		float A = MinBlepOsc_Get(&fish->SawOsc);
		float B = MinBlepOsc_GetPulse(&fish->PulseOsc);
		*buffer++ = (B * size) + (A*invsize) ;
	}
}



void Vosim(struct BigFish_t *fish, int32_t *buffer, int len)
{
	VosimOsc_Update(&fish->Vosim, fish->SampleRate, fish->CenterFreq, fish->Parameters[OSC_SIZE]/65536.0f, fish->Parameters[OSC_SPREAD]);
	while (len--)
	{

		*buffer++ = VosimOsc_Get(&fish->Vosim)*32768;
	}
}

void Organ(struct BigFish_t *fish, int32_t *buffer, int len)
{
	while (len--)
	{

		*buffer++ = 0;
	}
}

void Choir(struct BigFish_t *fish, int32_t *buffer, int len)
{
	while (len--)
	{
		*buffer++ = rand();
	}
}

void Grain(struct BigFish_t *fish, int32_t *buffer, int len)
{
	WaveBlepOsc_Update(&fish->WaveOsc, fish->SampleRate, fish->CenterFreq, fish->Parameters[OSC_SIZE], fish->Parameters[OSC_SPREAD]);
	
	while (len--)
	{

		float A = WaveBlepOsc_Get(&fish->WaveOsc);
		
		*buffer++ = A*32768.0f;
	}
}


float formant_filter(double *coeffs, double *memory, float in)
{
	float res = (float)(coeffs[0] * (in*(1.0 / 32768.0)) +
		coeffs[1] * memory[0] +
		coeffs[2] * memory[1] +
		coeffs[3] * memory[2] +
		coeffs[4] * memory[3] +
		coeffs[5] * memory[4] +
		coeffs[6] * memory[5] +
		coeffs[7] * memory[6] +
		coeffs[8] * memory[7] +
		coeffs[9] * memory[8] +
		coeffs[10] * memory[9]);

	memory[9] = memory[8];
	memory[8] = memory[7];
	memory[7] = memory[6];
	memory[6] = memory[5];
	memory[5] = memory[4];
	memory[4] = memory[3];
	memory[3] = memory[2];
	memory[2] = memory[1];
	memory[1] = memory[0];
	memory[0] = (double)res;
	return res *  32768.0 * 0.025118864315095794f * 6.0f;
}

static inline float DoQScaling(float c, float note, float fQScaling)
{
	if (fQScaling<0.5f)
	{
		float v1 = __min(1, c / ((16 - 16 * fQScaling)*note));
		v1 *= v1;
		return v1 + (1 - v1)*(2 * fQScaling);
	}
	float v2 = 1;
	if (c<200 / 44100)
	{
		v2 = __max(0.1f, v2*c / (200.f / 44100.f));
		v2 = v2*v2*v2*v2;
	}
	return 1 + (v2 - 1)*(2 * fQScaling - 1);
}


void BigFish_Filter(struct BigFish_t *fish, int32_t *bufferin, int32_t *bufferout, int len)
{
	float cutoffin = fish->Parameters[FILTER_CUTOFF] / 65535.0f;
	float V = 0.1 + 0.9*cutoffin;
	float freq = 20000 * (V*V*V);

	float mul[__FILTERKEYTRACK_COUNT] = { -2.0f,-1.5f, -1.0f,-0.5f,0.0f,0.5f,1.0f,1.5f,2.0f };
	int KeyTrack = ((int32_t)fish->Parameters[FILTER_KEYTRACK] * (__FILTERKEYTRACK_COUNT - 1)) / 65536;

	float FilterEnvScale = (fish->Parameters[FILTER_ENVELOPE] - 32768) / 32768.0f;
	int32_t FilterEnvResult = ADSR_GetCurved(&fish->FilterEnvelope, fish->SampleRate / len);

	if (KeyTrack != FILTERKEYTRACK_OFF)
	{
		float note = cutoffin * 127;
		int64_t ENV = int64_t(FilterEnvResult);
		
		float adsr = (ENV * FilterEnvScale * 127) / 65536.0f;
		freq = 440.0 * pow(2.0, (note - 69) / 12);
		freq += fish->CenterFreq * mul[KeyTrack];
		
	}
	else
	{		
		int64_t ENV =  int64_t(FilterEnvResult);
		
		float adsr = (ENV * FilterEnvScale * 20000) / 65536.0;
		freq += adsr;
	}
	

	if (freq < 30) freq = 30; else if (freq > 20000) freq = 20000;

	int mode = (fish->Parameters[FILTER_TYPE] * (__FILTERTYPE_COUNT - 1)) / 65536;

	if (mode == FILTERTYPE_VOCAL)
	{
		float cutoff = (fish->Parameters[FILTER_CUTOFF] / 65535.0f);
		cutoff = cutoff + (FilterEnvResult * FilterEnvScale) / 32768.0f;
		if (cutoff < 0) cutoff = 0; else if (cutoff > 1) cutoff = 1;
		cutoff *= 5;
		int IDX1 = floor(cutoff);
		int IDX2 = IDX1 +1;



		int32_t *b = bufferout;

		float IA = cutoff - (IDX1 );
		float A = 1.0 - IA;

		IDX1 += 15;
		IDX2 += 15;

		FormantSet_t T;
		float Levels[5];
		float Total = 0;
		for (int i = 0; i < 5; i++)
		{
			T.Formants[i].Freq = Formants[IDX1].Formants[i].Freq * A + Formants[IDX2].Formants[i].Freq * IA;
			Levels[i] = DBToLevel(Formants[IDX1].Formants[i].Ampl * A + Formants[IDX2].Formants[i].Ampl * IA);
			T.Formants[i].Bandwidth = Formants[IDX1].Formants[i].Bandwidth * A + Formants[IDX2].Formants[i].Bandwidth * IA;
			Total += Levels[i];

			float f1 = T.Formants[i].Freq - T.Formants[i].Bandwidth / 2;
			float f2 = T.Formants[i].Freq + T.Formants[i].Bandwidth / 2;

			float oct = log2f(f2 / f1);

			fish->filters[i].rbjBPF(T.Formants[i].Freq, 10, fish->SampleRate,1.0);
			fish->filters[i].ClearException();
//			fish->filters[i].rbjHPF(T.Formants[i].Freq, 6 + fish->Parameters[FILTER_RESONANCE]/200.0f, fish->SampleRate);

		}


		float in[32];
		float out[5][32];
		for (int i = 0; i < 32; i++)
		{
			in[i] = bufferin[i];
		}
		for (int i = 0; i < 5; i++)
		{
		//	fish->filters[i].Process(in, out[i], len);
		}
		
		for (int i = 0; i < len; i++)
		{
			float outp = 0;
			for (int j = 0; j < 5; j++)
			{
				outp += fish->Resonator[j].feed(bufferin[i], (fish->Parameters[FILTER_RESONANCE] * 0.999) / 65535.f, fish->SampleRate / T.Formants[j].Freq) * Levels[j];
			}
		//	*b++ = outp;
			
			bufferout[i] = outp / Total;//
//				fish->Resonator[0].feed(bufferin[i], (-fish->Parameters[FILTER_RESONANCE] * 0.999) / 65535.f, fish->SampleRate / T.Formants[0].Freq) * Levels[0] +
				//fish->Resonator[1].feed(bufferin[i], (-fish->Parameters[FILTER_RESONANCE] * 0.999) / 65535.f, fish->SampleRate / T.Formants[1].Freq) * Levels[1];
		}
		return;
	}
	


	float ResonanceParamVal = fish->Parameters[FILTER_RESONANCE] / 65536.0f;
	float fResonance2 = 14 * ResonanceParamVal*ResonanceParamVal*ResonanceParamVal + 5 * ResonanceParamVal + 1.0f;
	fResonance2 *= fResonance2;
	fResonance2 = (float)(1.0 + (fResonance2 - 1.0f)*(DoQScaling(freq*(1.f / 44100.f), 0, 1)));
	float fResonance2NS = fResonance2;
	float const sr = fish->SampleRate;
	float const odsr = (1.f / sr);

	float out[32];
	float in[32];
	for (int i = 0; i < len; i++)
	{
		in[i] = bufferin[i];
	}

	

	float HIGHCOMP = 0.85;

	switch (mode)
	{
	case FILTERTYPE_LP:
		fish->filters[0].zoelzerLPF(freq, fResonance2, odsr);		
		break;
	case FILTERTYPE_HP:
		fish->filters[0].rbjHPF(freq*HIGHCOMP, fResonance2, sr);
		break;
	case FILTERTYPE_BP:
		fish->filters[0].rbjBPF(freq, fResonance2*0.25f, sr);
		break;
	case FILTERTYPE_BR:
		fish->filters[0].rbjBRF(freq, fResonance2*0.25f, sr); 
		break;

	}
	


//	fish->filters[0].rbjLPF(freq, fResonance2, fish->SampleRate);
	fish->filters[0].Process(bufferin, bufferout, len);
	for (int i = 0; i < len; i++)
	{
		//bufferout[i] = out[i];
	}
	fish->filters[0].ClearException();

}

void BigFish_Drive(struct BigFish_t *fish, int32_t *bufferin, int32_t *bufferout, int len)
{
	int32_t temp[32];
	for (int i = 0; i < len; i++)
	{
		temp[i] = (bufferin[i] * 2560) ;
	}

	int mix = abs((fish->Parameters[FILTER_DRIVE] - 32768))/1000;
	if (mix > 256) mix = 256;
	int imix = 256 - mix;
	fish->Clipper.IntProcess(temp, len);
	for (int i = 0; i < len; i++)
	{
		bufferout[i] = (bufferin[i] * imix + (temp[i] / 256)*mix) / 256;
	}
}


void BigFish_GenerateBlock(struct BigFish_t *fish, int32_t *bufferOSCOUT, int32_t *bufferMAIN, int len)
{
	BigFish_Update(fish);

	int32_t Semi = (1 << 14) / 12;

	int32_t Coarse = ((fish->Parameters[PITCH_COARSE] * 48 - 24) / 65535);
	int32_t Fine = fish->Parameters[PITCH_FINE] - 32768;

	int32_t noteindex = fish->PitchInput + Coarse * Semi + ((Fine * Semi) / 32768);
	float Note = (noteindex / (float)Semi) - 69;
	float freq = 440 * powf(2.0f, Note/  12.0f);
	fish->CenterFreq = freq;
	
	int shapemul = (fish->Parameters[OSC_SHAPE] * 6 * 256) / 65536;
	
	int32_t A[MAXFISHBUFFER];
	int32_t B[MAXFISHBUFFER];
	int32_t OSC[MAXFISHBUFFER];
	int32_t AMPED[MAXFISHBUFFER];
	int32_t MAIN[MAXFISHBUFFER];

	while (len > 0)
	{
		int L = __min(len, MAXFISHBUFFER);
		len -= L;

		switch (shapemul / 256)
		{
		case 0:
			HyperSaw(fish, A, L);
			HyperPulse(fish, B, L);
			break;
		case 1:
			HyperPulse(fish, A, L);
			SuperFish(fish, B, L);
			break;
		case 2:
			SuperFish(fish, A, L);
			Vosim(fish, B, L);
			break;
		case 3:
			Vosim(fish, A, L);
			Organ(fish, B, L);
			break;
		case 4:
			Organ(fish, A, L);
			Choir(fish, B, L);
			break;
		case 5:
			Choir(fish, A, L);
			Grain(fish, B, L);
			break;
		}

		int32_t crossfade = shapemul & 0xff;
		int32_t icrossfade = 256 - crossfade;

		for (int i = 0; i < L; i++)
		{
			int32_t AmpEnvelope = ADSR_GetCurved(&fish->AmpEnvelope, fish->SampleRate);
			OSC[i] = (((B[i] * crossfade + A[i] * icrossfade) / 256));
			AMPED[i] = (OSC[i]*AmpEnvelope) / 65535;
			*bufferOSCOUT++ = OSC[i];
		}

		BigFish_Filter(fish, AMPED, MAIN, L);
		BigFish_Drive(fish, MAIN, MAIN, L);
	
		for (int i = 0; i < L; i++)
		{
			*bufferMAIN++ = MAIN[i];
		}
	}
}