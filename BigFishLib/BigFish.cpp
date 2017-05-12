#include "BigFish.h"

#include "bleptables.h"
#include "DistortionTable_1.h"
#include "DistortionTable_2.h"
#include "bleptables.h"

#include <math.h>

zoh_sample_t *blep;


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


#define LERP(F,A,B) ((B-A)*F+A)

void MinBlepOsc_Init(struct MinBlepOsc_t *osc)
{
	osc->index = 0;
	osc->mPhase = 0;
	osc->mPhaseIncrement = 0.1f;
	for (int i = 0; i < 48; i++)
	{
		osc->circularBuffer[i] = 0;
	}
}

void HyperOsc_Init(struct HyperOsc_t *osc)
{
	osc->index = 0;
	for (int i = 0; i < MAXHYPER; i++)
	{
		osc->mPhase[i] = 0;
		osc->mPhaseIncrement[i] = 0.1f;
	}
	for (int i = 0; i < 48; i++)
	{
		osc->circularBuffer[i] = 0;
	}
}

float MinBlepOsc_Get(struct  MinBlepOsc_t *osc)
{
	osc->mPhase += osc->mPhaseIncrement;
	osc->index = (osc->index + 1) % 48;

	while (osc->mPhase >= 1.0)
	{
		osc->mPhase -= 1.0;
		float exactCrossTime = 1.0f - ((osc->mPhaseIncrement - osc->mPhase) / osc->mPhaseIncrement);


		float tempIndex = (exactCrossTime*32.0f);
		float tempFraction = tempIndex - (float)floor(tempIndex);
		for (int i = 0; i < 47; i++)
		{
			osc->circularBuffer[(osc->index + i) % 48] += (float)(1.0f - LERP(tempFraction, blep[(int)floor(tempIndex)],blep[(int)ceil(tempIndex)]));
			tempIndex += 32;
		}
	}

	osc->circularBuffer[osc->index] += (osc->mPhase - osc->mPhaseIncrement);
	float output = osc->circularBuffer[osc->index];
	osc->circularBuffer[osc->index] = 0.0f;
	return output * 2.0 - 1.0;
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

float HyperOsc_Get(struct HyperOsc_t *osc)
{
	osc->index = (osc->index + 1) % 48;
	for (int o = 0; o < MAXHYPER; o++)
	{
		osc->mPhase[o] += osc->mPhaseIncrement[o];
		while (osc->mPhase[o] >= 1.0f)
		{
			osc->mPhase[o] -= 1.0f;
			float exactCrossTime = 1.0f - ((osc->mPhaseIncrement[o] - osc->mPhase[o]) / osc->mPhaseIncrement[o]);
			float tempIndex = (exactCrossTime*32.0f);
			float tempFraction = tempIndex - (float)floor(tempIndex);
			for (int i = 0; i < 47; i++)
			{
				osc->circularBuffer[(osc->index + i) % 48] += 2* (float)(1.0 - LERP(tempFraction, blep[(int)floor(tempIndex)], blep[(int)ceil(tempIndex)]));
				tempIndex += 32;
			}
		}

		osc->circularBuffer[osc->index] += (osc->mPhase[o] - osc->mPhaseIncrement[o]) * 2 - 1.0;
	}

	float output = osc->circularBuffer[osc->index];
	osc->circularBuffer[osc->index] = 0.0f;
	return output ;
}

void MinBlepOsc_Update(struct MinBlepOsc_t *osc, int samplerate, float centerfreq, float size, float spread)
{
	float C = centerfreq / (float)samplerate;
	osc->mPhaseIncrement = C;

}

void HyperOsc_Update(struct HyperOsc_t *osc, int samplerate, float centerfreq, float size, float spread)
{
	float C = centerfreq / (float)samplerate;
	osc->mPhaseIncrement[0] = C;
	osc->mPhaseIncrement[1] =( C*(1.00 + spread));
	osc->mPhaseIncrement[2] =( C*(1.00 - spread));
	osc->mPhaseIncrement[3] =( C*(1.00 + spread*1));
	osc->mPhaseIncrement[4] =( C*(1.00 - spread*2));
	osc->mPhaseIncrement[5] =( C*(1.00 + spread*3));
	osc->mPhaseIncrement[6] =( C*(1.00 - spread*3));
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


// MinBLEP Generation Code
// By Daniel Werner
// This Code Is Public Domain

#include <math.h>

#define PI 3.14159265358979f

// SINC Function
inline float SINC(float x)
{
	float pix;

	if (x == 0.0f)
		return 1.0f;
	else
	{
		pix = PI * x;
		return sinf(pix) / pix;
	}
}

// Generate Blackman Window
inline void BlackmanWindow(int n, float *w)
{
	int m = n - 1;
	int i;
	float f1, f2, fm;

	fm = (float)m;
	for (i = 0; i <= m; i++)
	{
		f1 = (2.0f * PI * (float)i) / fm;
		f2 = 2.0f * f1;
		w[i] = 0.42f - (0.5f * cosf(f1)) + (0.08f * cosf(f2));
	}
}

// Discrete Fourier Transform
void DFT(int n, float *realTime, float *imagTime, float *realFreq, float *imagFreq)
{
	int k, i;
	float sr, si, p;

	for (k = 0; k < n; k++)
	{
		realFreq[k] = 0.0f;
		imagFreq[k] = 0.0f;
	}

	for (k = 0; k < n; k++)
		for (i = 0; i < n; i++)
		{
			p = (2.0f * PI * (float)(k * i)) / n;
			sr = cosf(p);
			si = -sinf(p);
			realFreq[k] += (realTime[i] * sr) - (imagTime[i] * si);
			imagFreq[k] += (realTime[i] * si) + (imagTime[i] * sr);
		}
}

// Inverse Discrete Fourier Transform
void InverseDFT(int n, float *realTime, float *imagTime, float *realFreq, float *imagFreq)
{
	int k, i;
	float sr, si, p;

	for (k = 0; k < n; k++)
	{
		realTime[k] = 0.0f;
		imagTime[k] = 0.0f;
	}

	for (k = 0; k < n; k++)
	{
		for (i = 0; i < n; i++)
		{
			p = (2.0f * PI * (float)(k * i)) / n;
			sr = cosf(p);
			si = -sinf(p);
			realTime[k] += (realFreq[i] * sr) + (imagFreq[i] * si);
			imagTime[k] += (realFreq[i] * si) - (imagFreq[i] * sr);
		}
		realTime[k] /= n;
		imagTime[k] /= n;
	}
}

// Complex Absolute Value
inline float cabs(float x, float y)
{
	return sqrtf((x * x) + (y * y));
}

// Complex Exponential
inline void cexp(float x, float y, float *zx, float *zy)
{
	float expx;

	expx = expf(x);
	*zx = expx * cosf(y);
	*zy = expx * sinf(y);
}

// Compute Real Cepstrum Of Signal
void RealCepstrum(int n, float *signal, float *realCepstrum)
{
	float *realTime, *imagTime, *realFreq, *imagFreq;
	int i;

	realTime = new float[n];
	imagTime = new float[n];
	realFreq = new float[n];
	imagFreq = new float[n];

	// Compose Complex FFT Input

	for (i = 0; i < n; i++)
	{
		realTime[i] = signal[i];
		imagTime[i] = 0.0f;
	}

	// Perform DFT

	DFT(n, realTime, imagTime, realFreq, imagFreq);

	// Calculate Log Of Absolute Value

	for (i = 0; i < n; i++)
	{
		realFreq[i] = logf(cabs(realFreq[i], imagFreq[i]));
		imagFreq[i] = 0.0f;
	}

	// Perform Inverse FFT

	InverseDFT(n, realTime, imagTime, realFreq, imagFreq);

	// Output Real Part Of FFT
	for (i = 0; i < n; i++)
		realCepstrum[i] = realTime[i];

	delete realTime;
	delete imagTime;
	delete realFreq;
	delete imagFreq;
}

// Compute Minimum Phase Reconstruction Of Signal
void MinimumPhase(int n, float *realCepstrum, float *minimumPhase)
{
	int i, nd2;
	float *realTime, *imagTime, *realFreq, *imagFreq;

	nd2 = n / 2;
	realTime = new float[n];
	imagTime = new float[n];
	realFreq = new float[n];
	imagFreq = new float[n];

	if ((n % 2) == 1)
	{
		realTime[0] = realCepstrum[0];
		for (i = 1; i < nd2; i++)
			realTime[i] = 2.0f * realCepstrum[i];
		for (i = nd2; i < n; i++)
			realTime[i] = 0.0f;
	}
	else
	{
		realTime[0] = realCepstrum[0];
		for (i = 1; i < nd2; i++)
			realTime[i] = 2.0f * realCepstrum[i];
		realTime[nd2] = realCepstrum[nd2];
		for (i = nd2 + 1; i < n; i++)
			realTime[i] = 0.0f;
	}

	for (i = 0; i < n; i++)
		imagTime[i] = 0.0f;

	DFT(n, realTime, imagTime, realFreq, imagFreq);

	for (i = 0; i < n; i++)
		cexp(realFreq[i], imagFreq[i], &realFreq[i], &imagFreq[i]);

	InverseDFT(n, realTime, imagTime, realFreq, imagFreq);

	for (i = 0; i < n; i++)
		minimumPhase[i] = realTime[i];

	delete realTime;
	delete imagTime;
	delete realFreq;
	delete imagFreq;
}

// Generate MinBLEP And Return It In An Array Of Floating Point Values
float *GenerateMinBLEP(int zeroCrossings, int overSampling)
{
	int i, n;
	float r, a, b;
	float *buffer1, *buffer2, *minBLEP;

	n = (zeroCrossings * 2 * overSampling) + 1;

	buffer1 = new float[n];
	buffer2 = new float[n];

	// Generate Sinc

	a = (float)-zeroCrossings;
	b = (float)zeroCrossings;
	for (i = 0; i < n; i++)
	{
		r = ((float)i) / ((float)(n - 1));
		buffer1[i] = SINC(a + (r * (b - a)));
	}

	// Window Sinc

	BlackmanWindow(n, buffer2);
	for (i = 0; i < n; i++)
		buffer1[i] *= buffer2[i];

	// Minimum Phase Reconstruction

	RealCepstrum(n, buffer1, buffer2);
	MinimumPhase(n, buffer2, buffer1);

	// Integrate Into MinBLEP

	minBLEP = new float[n];
	a = 0.0f;
	for (i = 0; i < n; i++)
	{
		a += buffer1[i];
		minBLEP[i] = a;
	}

	// Normalize
	a = minBLEP[n - 1];
	a = 1.0f / a;
	for (i = 0; i < n; i++)
		minBLEP[i] *= a;

	delete buffer1;
	delete buffer2;
	return minBLEP;
}


void BigFish_Init(struct BigFish_t *fish, int samplerate)
{



	blep = GenerateMinBLEP(24, 32);



	fish->SampleRate = samplerate;
	fish->Clipper.Init(samplerate);	
	fish->PitchInput = 0;
	Inertia_Init(&fish->Accent);
	MinBlepOsc_Init(&fish->NormalOsc);
	HyperOsc_Init(&fish->HyperPulseOsc);
	HyperOsc_Init(&fish->HyperSawOsc);
	ADSR_Init(&fish->AmpEnvelope, ENVMODE_GATE, 0);
	ADSR_Init(&fish->FilterEnvelope, ENVMODE_TRIGGER, 0);
}

#define CHECKGATE(f, g, on,off) if (f->Gates[g] != f->PreviousGates[g]){f->PreviousGates[g] = f->Gates[g]; if(f->Gates[g]) on(f);else off(f);};
void ADSR_ApplyParameters(struct ADSR_Envelope_t *adsr, int a, int d, int s, int r)
{
	adsr->A = a >> 6;
	adsr->D = d >> 6;
	adsr->S = s >> 6;
	adsr->R = r >> 6;
}

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
	float spread = (fish->Parameters[OSC_SPREAD]*0.05) / 65536.0f; 
	float size = (fish->Parameters[OSC_SIZE ]) / 65536.0f;
	HyperOsc_Update(&fish->HyperSawOsc, fish->SampleRate, fish->CenterFreq, size, spread);
	while (len--)
	{
		*buffer++ = HyperOsc_Get(&fish->HyperSawOsc) * 32768.0f / 7;
	}
}

void HyperPulse(struct BigFish_t *fish, int32_t *buffer, int len)
{
	float spread = (fish->Parameters[OSC_SPREAD] * 0.05) / 65536.0f;
	float size = (fish->Parameters[OSC_SIZE]) / 65536.0f;
	HyperOsc_Update(&fish->HyperPulseOsc, fish->SampleRate, fish->CenterFreq, size, spread);
	while (len--)
	{
		*buffer++ = HyperOsc_Get(&fish->HyperPulseOsc);
	}
}

void SuperFish(struct BigFish_t *fish, int32_t *buffer, int len)
{
	MinBlepOsc_Update(&fish->NormalOsc, fish->SampleRate, fish->CenterFreq, fish->Parameters[OSC_SIZE], fish->Parameters[OSC_SPREAD]);
	while (len--)
	{
		*buffer++ = MinBlepOsc_Get(&fish->NormalOsc) * 32768.0f;
	}
}

void FM(struct BigFish_t *fish, int32_t *buffer, int len)
{
	while (len--)
	{
		*buffer++ = 0;
	}
}

void Organ(struct BigFish_t *fish, int32_t *buffer, int len)
{
}

void Choir(struct BigFish_t *fish, int32_t *buffer, int len)
{
}

void Grain(struct BigFish_t *fish, int32_t *buffer, int len)
{
}

void BigFish_Filter(struct BigFish_t *fish, int32_t *bufferin, int32_t *bufferout, int len)
{

	switch (fish->Parameters[FILTER_KEYTRACK])
	{

	}


	switch (fish->Parameters[FILTER_TYPE])
	{
	case FILTERTYPE_LP:
		break;
	case FILTERTYPE_HP:
		break;
	case FILTERTYPE_VOCAL:
		break;
	case FILTERTYPE_BR:
		break;
	case FILTERTYPE_BP:
		break;
	}

	for (int i = 0; i < len; i++)
	{
		*bufferout++ = *bufferin++;
	}
}

void BigFish_Drive(struct BigFish_t *fish, int32_t *bufferin, int32_t *bufferout, int len)
{
	fish->Clipper.IntProcess(bufferin, len);
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
	
	int shapemul = (fish->Parameters[OSC_SHAPE] * 5 * 256) / 65536;
	
	int32_t A[MAXFISHBUFFER];
	int32_t B[MAXFISHBUFFER];
	int32_t OSC[MAXFISHBUFFER];
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
			FM(fish, B, L);
			break;
		case 3:
			FM(fish, A, L);
			Organ(fish, B, L);
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
			int32_t AmpEnvelope = ADSR_Get(&fish->AmpEnvelope, fish->SampleRate);
			OSC[i] = (((B[i] * crossfade + A[i] * icrossfade) / 256) * AmpEnvelope)/32768;
			*bufferOSCOUT++ = OSC[i];
		}

		BigFish_Filter(fish, OSC, MAIN, L);
		//BigFish_Drive(fish, MAIN, MAIN, L);
	
		for (int i = 0; i < L; i++)
		{
			*bufferMAIN++ = MAIN[i];
		}
	}
}