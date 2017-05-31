#include "BigFish.h"

//#include "bleptables.h"
#include "DistortionTable_1.h"
#include "DistortionTable_2.h"
//#include "bleptables.h"

#ifdef WIN32
#define MEMATTR 
#else
#define MEMATTR __attribute__((section(".data")))
#endif


#include <math.h>

#include "FormantTable.h"

const float chordtable[16][4] = {
		{ powf(2, 0.0f / 12.0f), 0, 0, 0 },
		{ powf(2, -0.1f / 12.0f), powf(2, 0.0f / 12.0f), powf(2, 0.1f / 12.0f), powf(2, 12.05f / 12.0f) }, // Fat Moog
		{ powf(2, -0.1f / 12.0f), powf(2, 0.1f / 12.0f), powf(2, 7.04f / 12.0f), powf(2, 0.1f / 12.0f) }, // Moog 5th
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

float GetInterpolatedResultFloat(float *table, SteppedResult_t *inp)
{
	float F = inp->fractional *(1.0f / 256.0f);
	float IF = 1.0f - F;
	return table[inp->index] * IF + table[inp->index + 1] * F;
}

void GetSteppedResult(uint16_t param, uint8_t steps, SteppedResult_t *out)
{
	//max(floor(x + 0.25), (x + 0.25 - floor(x + 0.25)) * 2 + floor(x + 0.25) - 1)

	uint32_t X = (param * steps)/256;
	X += 64;

	int FloorX = X &0xffffff00;
	int Aside = FloorX;
	int Bside = (X - (FloorX)) * 2 + (FloorX)-256;
	int M = (Aside > Bside) ? Aside : Bside;

	out->index = M >> 8;
	out->fractional = M & 0xff;

}

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

#ifdef RANGECHECKS
	InitRange(&fish->PreFilter);
	InitRange(&fish->PostDrive);
	InitRange(&fish->PostFilter);
	InitRange(&fish->HyperSawRange);
	InitRange(&fish->OrganRange);
	InitRange(&fish->HyperPulseRange);
	InitRange(&fish->WaveBlepRange);
	InitRange(&fish->SuperFishRange);


#endif
	fish->ODSR = 1.0f / (float)samplerate;
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
	for (int i = 0; i < 3; i++)
	{
		MinBlepOsc_Init(&fish->PulseOsc[i]);
		MinBlepOsc_Init(&fish->SawOsc[i]);
		WaveBlepOsc_Init(&fish->WaveOsc[i]);
		
	}
	Organ_Init(&fish->Organ, fish->ODSR);
	HyperPulse_Init(&fish->HyperPulseOsc);
	HyperOsc_Init(&fish->HyperSawOsc);
	ADSR_BuildTable();
	ADSR_Init(&fish->AmpEnvelope, ENVMODE_GATE, 0, ENVTABLE_EXP);
	ADSR_Init(&fish->FilterEnvelope, ENVMODE_TRIGGER, 0, ENVTABLE_LOG);
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

void HyperSaw(struct BigFish_t *fish, int32_t *buffer, int32_t *bufferB, int len, int32_t isize)
{
	int32_t *outbuffer = buffer;
	int inlen = len;
	float spread = (fish->Parameters[OSC_SPREAD]) / 65536.0f;
	float size = isize / 65536.0f;
	HyperOsc_Update(&fish->HyperSawOsc, fish->ODSR, fish->CenterFreq, size, spread);
	while (len--)
	{
		*buffer++ =HyperOsc_Get(&fish->HyperSawOsc);
	}

#ifdef RANGECHECKS
	UpdateRangeBuffer(&fish->HyperSawRange, outbuffer, inlen);
#endif
}

void HyperPulse(struct BigFish_t *fish, int32_t *buffer, int32_t *bufferB, int len, int32_t isize)
{
	int32_t *outbuffer = buffer;
	int inlen = len;
	float spread = (fish->Parameters[OSC_SPREAD] ) / 65536.0f;
	float size = (isize) / 65536.0f;
	HyperPulse_Update(&fish->HyperPulseOsc, fish->ODSR, fish->CenterFreq, size, spread);
	while (len--)
	{
		*buffer++ =HyperPulse_Get(&fish->HyperPulseOsc) ;
	}
#ifdef RANGECHECKS
	UpdateRangeBuffer(&fish->HyperPulseRange, outbuffer, inlen);
#endif
}

void SuperFish(struct BigFish_t *fish, int32_t *buffer, int32_t *bufferB, int len, int32_t isize)
{

	int32_t *outbuffer = buffer;
	int inlen = len;

	int activeoscs = fish->ChordVoicesActive;
	for (int i = 0; i < activeoscs; i++)
	{
		MinBlepOsc_Update(&fish->SawOsc[i], fish->ODSR, fish->CenterFreq * fish->ChordPitchTable[i], isize, fish->Parameters[OSC_SPREAD]);
		MinBlepOsc_Update(&fish->PulseOsc[i], fish->ODSR, fish->CenterFreq * 2.0f * fish->ChordPitchTable[i], isize, fish->Parameters[OSC_SPREAD]);
	}
	if (activeoscs > 1)
	{
		int size = fish->Parameters[OSC_SIZE] / 2;
		int invsize = 32768 - size;
		while (len--)
		{
			int32_t A = (MinBlepOsc_Get(&fish->SawOsc[0]) >> 1) ;
			int32_t B = (MinBlepOsc_GetPulse(&fish->PulseOsc[0]) >> 1) ;
			for (int i = 1; i < activeoscs; i++)
			{
				A += (MinBlepOsc_Get(&fish->SawOsc[i]) >> 1) ;
				B += (MinBlepOsc_GetPulse(&fish->PulseOsc[i]) >> 1);
			}
			*buffer++ = (A *invsize + B *size ) >> 15;
		}
	}
	else
	{
		int size = fish->Parameters[OSC_SIZE] / 2;
		int invsize = 32768 - size;
		while (len--)
		{
			int32_t A = (MinBlepOsc_Get(&fish->SawOsc[0]) >> 1) * invsize;
			int32_t B = (MinBlepOsc_GetPulse(&fish->PulseOsc[0]) >> 1) * size;

			*buffer++ = (A + B) >> 15;
		}

	}

#ifdef RANGECHECKS
	UpdateRangeBuffer(&fish->SuperFishRange, outbuffer, inlen);
#endif
}



void Vosim(struct BigFish_t *fish, int32_t *buffer, int32_t *bufferB, int len, int32_t size)
{
	VosimOsc_Update(&fish->Vosim, fish->ODSR, fish->CenterFreq, size/65536.0f, fish->Parameters[OSC_SPREAD]);
	while (len--)
	{

		*buffer++ = VosimOsc_Get(&fish->Vosim)*32768;
	}
}

void Organ(struct BigFish_t *fish, int32_t *buffer, int32_t *bufferB, int len, int32_t isize)
{
	int32_t *outbuffer = buffer;
	int inlen= len;
	int activeoscs = fish->ChordVoicesActive;
	for (int i = 0; i < activeoscs; i++)
	{
		Organ_Update(&fish->Organ.Voices[i], fish->ODSR, fish->CenterFreq * fish->ChordPitchTable[i], isize * (1.0f / 65536.0f), fish->Parameters[OSC_SPREAD]);
	}
	if (activeoscs > 1)
	{

		while (len--)
		{
			int32_t A = Organ_GetMain(&fish->Organ);

			for (int i = 1; i < activeoscs; i++)
			{
				A += Organ_Get(&fish->Organ, &fish->Organ.Voices[i]);
					
			}
			*buffer++ = A / 12;
		}
	}
	else
	{
		while (len--)
		{

			*buffer++ = Organ_GetMain(&fish->Organ)/4 ;;
		}


	}
#ifdef RANGECHECKS
	UpdateRangeBuffer(&fish->OrganRange, outbuffer, inlen);
#endif

}

void Choir(struct BigFish_t *fish, int32_t *buffer, int32_t *bufferB, int len, int32_t size)
{
	while (len--)
	{
		*buffer++ = rand()%32768 - (32768/2);
	}
}

void Grain(struct BigFish_t *fish, int32_t *buffer, int32_t *bufferB, int len, int32_t size)
{

	int32_t *outbuffer = buffer;
	int inlen = len;


	WaveBlepOsc_Update(&fish->WaveOsc[0], fish->ODSR, fish->CenterFreq, size, fish->Parameters[OSC_SPREAD]);

	while (len--)
	{

		float A = WaveBlepOsc_Get(&fish->WaveOsc[0]);

		*buffer++ = (int)(A*32768.0f);
	}

#ifdef RANGECHECKS
	UpdateRangeBuffer(&fish->WaveBlepRange, outbuffer, inlen);
#endif

}

void Copy(struct BigFish_t *fish, int32_t *buffer, int32_t *bufferB, int len, int32_t size)
{	
	while (len--)
	{
		*buffer++ = *bufferB++;
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

void BigFish_VocalFilter(struct BigFish_t *fish, int32_t *bufferin, int32_t *bufferout, int len, float FilterEnvScale, float FilterEnvResult)
{

	float cutoff = (fish->Parameters[FILTER_CUTOFF] / 65535.0f);
	cutoff = cutoff + (FilterEnvResult * FilterEnvScale) / 32768.0f;
	if (cutoff < 0) cutoff = 0; else if (cutoff > 1) cutoff = 1;
	cutoff *= 5;
	int IDX1 = floor(cutoff);
	int IDX2 = IDX1 + 1;



	int32_t *b = bufferout;

	float IA = cutoff - (IDX1);
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

		fish->filters[i].rbjBPF_withgain(T.Formants[i].Freq, 10, fish->SampleRate, 1.0);
		fish->filters[i].ClearException();
		//		fish->filters[i].rbjHPF(T.Formants[i].Freq, 6 + fish->Parameters[FILTER_RESONANCE]/200.0f, fish->SampleRate);
	}


	float in[32];
	float out[5][32];
	for (int i = 0; i < 32; i++)
	{
		in[i] = bufferin[i];
	};
	//	for (int i = 0; i < 5; i++)
	//	{
	//		fish->filters[i].Process(in, out[i], len);
	//	}
	Total = 1.0f / Total;
	for (int i = 0; i < len; i++)
	{
		float outp = 0;
		for (int j = 0; j < 5; j++)
		{
			outp += fish->Resonator[j].feed(bufferin[i], (fish->Parameters[FILTER_RESONANCE] * 0.999) / 65535.f, fish->SampleRate / T.Formants[j].Freq) * Levels[j];
		}
		//	*b++ = outp;

		bufferout[i] = outp * Total;//
		//				fish->Resonator[0].feed(bufferin[i], (-fish->Parameters[FILTER_RESONANCE] * 0.999) / 65535.f, fish->SampleRate / T.Formants[0].Freq) * Levels[0] +
		//fish->Resonator[1].feed(bufferin[i], (-fish->Parameters[FILTER_RESONANCE] * 0.999) / 65535.f, fish->SampleRate / T.Formants[1].Freq) * Levels[1];
	}
}

void BigFish_Filter(struct BigFish_t *fish, int32_t *bufferin, int32_t *bufferout, int len)
{
	float cutoffin = fish->Parameters[FILTER_CUTOFF] / 65535.0f;
	float V = 0.1 + 0.9*cutoffin;
	float freq = 20000 * (V*V*V);

	float mul[__FILTERKEYTRACK_COUNT] = { -2.0f,-1.5f, -1.0f,-0.5f,0.0f,0.5f,1.0f,1.5f,2.0f };
	SteppedResult_t sr;
	GetSteppedResult(fish->Parameters[FILTER_KEYTRACK], __FILTERKEYTRACK_COUNT - 1, &sr);

	int KeyTrack = sr.index;

	float FilterEnvScale = (fish->Parameters[FILTER_ENVELOPE] - 32768) / 32768.0f;
	if (FilterEnvScale > 0) FilterEnvScale *= FilterEnvScale; else FilterEnvScale *= -FilterEnvScale;

	ADSR_Update(&fish->FilterEnvelope, fish->SampleRate/len);

	int32_t FilterEnvResult = ADSR_GetCurved(&fish->FilterEnvelope);

	if (sr.index != FILTERKEYTRACK_OFF || sr.fractional != 0)
	{
		float note = cutoffin * 127;
		int64_t ENV = int64_t(FilterEnvResult);

		//float adsr = (ENV * FilterEnvScale * 127) / (127 * 65535.0f);
		freq = 440.0 * pow(2.0, (note - 69) / 12);
		freq += fish->CenterFreq * GetInterpolatedResultFloat(mul, &sr);

		float adsr = (ENV * FilterEnvScale * 15000) / 65536.0;
		freq += adsr;

	}
	else
	{		
		int64_t ENV =  int64_t(FilterEnvResult);

		float adsr = (ENV * FilterEnvScale * 15000) / 65536.0;
		freq += adsr;
	}


	if (freq < 30) freq = 30; else if (freq > 20000) freq = 20000;

	int mode = ((int32_t)fish->Parameters[FILTER_TYPE]);

	if (mode == FILTERTYPE_VOCAL)
	{
		BigFish_VocalFilter(fish, bufferin, bufferout, len, FilterEnvScale, FilterEnvResult);
		return;
	}

	float ResonanceParamVal = fish->Parameters[FILTER_RESONANCE] / 65536.0f;
	float fResonance2 = 14 * ResonanceParamVal*ResonanceParamVal*ResonanceParamVal + 5 * ResonanceParamVal + 1.0f;
	fResonance2 *= fResonance2;
	fResonance2 = (float)(1.0 + (fResonance2 - 1.0f)*(DoQScaling(freq*(1.f / 44100.f), 0, 1)));
	float fResonance2NS = fResonance2;
	float const srate = fish->SampleRate;
	float const odsr = fish->ODSR;

	


	float HIGHCOMP = 0.85f;

	switch (mode)
	{
	case FILTERTYPE_LP:
		fish->filters[0].zoelzerLPF(freq, fResonance2, odsr);		
		break;
	case FILTERTYPE_HP:
		fish->filters[0].rbjHPF(freq*HIGHCOMP, fResonance2, odsr);
		break;
	case FILTERTYPE_BP:
		fish->filters[0].rbjBPF(freq, fResonance2*0.25f, odsr);
		break;
	case FILTERTYPE_BR:
		fish->filters[0].rbjBRF(freq, fResonance2*0.25f, odsr); 
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

	int mix = abs((fish->Parameters[FILTER_DRIVE] - 32768))/200;
	if (mix > 256) mix = 256;
	int imix = 256 - mix;
	fish->Clipper.IntProcess(temp, len);

	for (int i = 0; i < len; i++)
	{
		bufferout[i] = (bufferin[i] * imix + (temp[i] / 256)*mix) / 256;
	}
}

inline void BigFish_GenerateBlock(struct BigFish_t *fish,int32_t *input, int32_t *bufferOSCOUT, int32_t *bufferMAIN, int len)
{
	BigFish_Update(fish);

	int32_t Semi = (1 << 14) / 12;

	int32_t Coarse = ((fish->Parameters[PITCH_COARSE] * 48 - 24) / 65535);
	int32_t Fine = fish->Parameters[PITCH_FINE] - 32768;

	int32_t noteindex = fish->PitchInput + Coarse * Semi + ((Fine * Semi) / 32768);
	float Note = (noteindex / (float)Semi) - 69;
	float freq = 440 * powf(2.0f, Note/  12.0f);
	fish->CenterFreq = freq;


	int32_t A[MAXFISHBUFFER];
	int32_t B[MAXFISHBUFFER];
	int32_t OSC[MAXFISHBUFFER];
	int32_t AMPED[MAXFISHBUFFER];
	int32_t MAIN[MAXFISHBUFFER];
	typedef void GenFunc(BigFish_t *, int32_t *, int32_t *, int, int32_t);


	SteppedResult_t src;
	GetSteppedResult(fish->Parameters[PITCH_CHORD], 17, &src);
	
	fish->ChordVoicesActive = 1;
	if (src.index > 0)
	{
		src.index--;
		fish->ChordVoicesActive = 3;
		float sf = src.fractional *(1.0f / 255.0f);
		float isf = 1.0f - sf;
		for (int i = 0; i < 3; i++)
		{
			fish->ChordPitchTable[i] = chordtable[src.index][i] * isf + chordtable[src.index + 1][i] * sf;
		}
	}
	else
	{
		fish->ChordPitchTable[0] = 1.0f;
	}

	//GenFunc* Funcs[8] = {SuperFish,SuperFish,SuperFish,SuperFish,SuperFish,SuperFish,SuperFish,SuperFish};//HyperSaw, HyperPulse, SuperFish, Vosim, Organ, Choir, Grain, Copy};
	GenFunc* Funcs[8] = {HyperSaw, HyperPulse, SuperFish, Vosim, Organ, Choir, Grain, Copy};

	SteppedResult_t sr;
	GetSteppedResult(fish->Parameters[OSC_SHAPE], 6, &sr);

	int FuncIdx = sr.index;
	int32_t crossfade = sr.fractional;
	int32_t icrossfade = 256 - crossfade;

	ADSR_Update(&fish->AmpEnvelope, fish->SampleRate);

	while (len > 0)
	{
		int L = __min(len, MAXFISHBUFFER);
		len -= L;
		int32_t Size = fish->Parameters[OSC_SIZE];

		if (sr.fractional > 0)
		{

			float frac = sr.fractional * (1.0f / 255.0f);
			float ifrac = 1.0 - frac;
			Funcs[FuncIdx](fish, A, B, L, Size * ifrac );
			Funcs[FuncIdx + 1](fish, B, A, L, Size * frac);

			for (int i = 0; i < L; i++)
			{
				int32_t AmpEnvelope = ADSR_GetCurved(&fish->AmpEnvelope);
				OSC[i] = (((B[i] * crossfade + A[i] * icrossfade) / 256));
				int64_t inter = ((int64_t)OSC[i] * (int64_t)AmpEnvelope);
				inter /= (int64_t)65536;
				AMPED[i] = (int)inter;
				*bufferOSCOUT++ = OSC[i];
			}
		}
		else
		{
			Funcs[FuncIdx](fish, A, B, L, Size);
			for (int i = 0; i < L; i++)
			{
				int32_t AmpEnvelope = ADSR_GetCurved(&fish->AmpEnvelope);
				OSC[i] = A[i];
				int64_t inter = ((int64_t)OSC[i] * (int64_t)AmpEnvelope);
				inter /= (int64_t)65536;
				AMPED[i] = (int)inter;
				*bufferOSCOUT++ = OSC[i];
			}
		}

#ifdef RANGECHECKS
		UpdateRangeBuffer(&fish->PreFilter, AMPED, L);
#endif

		BigFish_Filter(fish, AMPED, MAIN, L);

#ifdef RANGECHECKS
		UpdateRangeBuffer(&fish->PostFilter, MAIN, L);
#endif
		BigFish_Drive(fish, MAIN, MAIN, L);

#ifdef RANGECHECKS
		UpdateRangeBuffer(&fish->PostDrive, MAIN, L);
#endif

		for (int i = 0; i < L; i++)
		{
			*bufferMAIN++ = MAIN[i];
		}
	}
}
#ifdef RANGECHECKS
void InitRange(RangeChecker_t *R)
{
	R->bits = 0;
	R->valuecount = 0;
	R->max = 0;
	R->min = 0;
	R->signedvalues = 0;
}

void UpdateRange(RangeChecker_t *R, int64_t inv)
{
	if (R->valuecount == 0)
	{
		R->max = R->min = inv;
	}
	R->valuecount++;
	if (inv > R->max) R->max = inv; else if (inv < R->min) R->min = inv;

	if (R->min < 0) R->signedvalues = true;
}


void UpdateRangeBuffer(RangeChecker_t *R, int32_t *buffer, int len)
{
	for (int i = 0; i < len; i++) UpdateRange(R, buffer[i]);
}
#endif