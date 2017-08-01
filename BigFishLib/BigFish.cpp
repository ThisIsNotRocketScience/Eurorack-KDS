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

#ifdef WIN32
BigFishGlobals_t globals
#else
const BigFishGlobals_t globals
#endif
= { 1.0f / 44100.0f, 44100.0f, 44100, -(2.0f* PI / 44100.0f)*0.5f , 2.0f* PI / 44100.0f };

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

float BigFish_GetInterpolatedResultFloat(float *table, BigFish_SteppedResult_t *inp)
{
	float F = inp->fractional *(1.0f / 256.0f);
	float IF = 1.0f - F;
	return table[inp->index] * IF + table[inp->index + 1] * F;
}

void BigFish_GetSteppedResult(uint16_t param, uint8_t steps, BigFish_SteppedResult_t *out)
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

void VocalFilter_Init(VocalFilter_t *filt)
{
	filt->Aspiration = 0;
	filt->Bypass = 0;
	filt->Frication = 0;
	filt->hpb1 = 0;
	filt->hpb2 = 0;
	filt->lastin2 = 0;
	filt->Voicing = 0;
	for (int i = 0; i < 7; i++) 
	{
		filt->res[i].p1 = 0;
		filt->res[i].p2 = 0;
	}
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
	VocalFilter_Init(&fish->VocalFilter);
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
	int32_t *outbuffer = buffer;
	int inlen = len;

	int activeoscs = fish->ChordVoicesActive;
	for (int i = 0; i < activeoscs; i++)
	{
		FMOsc_Update(&fish->FMOsc[i], fish->ODSR, fish->CenterFreq * fish->ChordPitchTable[i], size, fish->Parameters[OSC_SPREAD]);
	}
	if (activeoscs > 1)
	{
		while (len--)
		{
			int32_t A = (FMOsc_Get(&fish->FMOsc[0]));
			for (int i = 1; i < activeoscs; i++)
			{
				A += (FMOsc_Get(&fish->FMOsc[i]));

			}
			*buffer++ = A >> 1;
		}
	}
	else
	{
		while (len--)
		{
			int32_t A = (FMOsc_Get(&fish->FMOsc[0]));

			*buffer++ = (A);
		}

	}

#ifdef RANGECHECKS
	UpdateRangeBuffer(&fish->FMRange, outbuffer, inlen);
#endif
}

void Grain(struct BigFish_t *fish, int32_t *buffer, int32_t *bufferB, int len, int32_t size)
{

	int32_t *outbuffer = buffer;
	int inlen = len;

	int activeoscs = fish->ChordVoicesActive;
	for (int i = 0; i < activeoscs; i++)
	{
		WaveBlepOsc_Update(&fish->WaveOsc[i], fish->ODSR, fish->CenterFreq * fish->ChordPitchTable[i], size, fish->Parameters[OSC_SPREAD]);
	}
	if (activeoscs > 1)
	{
		while (len--)
		{
			int32_t A = (WaveBlepOsc_Get(&fish->WaveOsc[0]) );
			for (int i = 1; i < activeoscs; i++)
			{
				A += (WaveBlepOsc_Get(&fish->WaveOsc[i]));
				
			}
			*buffer++ = A >> 1;
	}
}
	else
	{
		while (len--)
		{
			int32_t A = (WaveBlepOsc_Get(&fish->WaveOsc[0]) );

			*buffer++ = (A) ;
		}

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

float const PowTab[130 * 2] = {
	0.018581,0.001105, 0.019686,0.001171, 0.020857,0.001240, 0.022097,0.001314, 0.023411,0.001392, 0.024803,0.001475, 0.026278,0.001563, 0.027841,0.001655, 0.029496,0.001754, 0.031250,0.001858, 0.033108,0.001969, 0.035077,0.002086, 0.037163,0.002210, 0.039373,0.002341, 0.041714,0.002480, 0.044194,0.002628,
	0.046822,0.002784, 0.049606,0.002950, 0.052556,0.003125, 0.055681,0.003311, 0.058992,0.003508, 0.062500,0.003716, 0.066216,0.003937, 0.070154,0.004172, 0.074325,0.004420, 0.078745,0.004682, 0.083427,0.004961, 0.088388,0.005256, 0.093644,0.005568, 0.099213,0.005899, 0.105112,0.006250, 0.111362,0.006622,
	0.117984,0.007016, 0.125000,0.007433, 0.132433,0.007875, 0.140308,0.008343, 0.148651,0.008839, 0.157490,0.009365, 0.166855,0.009922, 0.176777,0.010512, 0.187288,0.011137, 0.198425,0.011799, 0.210224,0.012501, 0.222725,0.013244, 0.235969,0.014031, 0.250000,0.014866, 0.264866,0.015750, 0.280616,0.016686,
	0.297302,0.017678, 0.314980,0.018730, 0.333710,0.019843, 0.353553,0.021023, 0.374577,0.022273, 0.396850,0.023598, 0.420448,0.025001, 0.445449,0.026488, 0.471937,0.028063, 0.500000,0.029732, 0.529732,0.031500, 0.561231,0.033373, 0.594604,0.035357, 0.629961,0.037459, 0.667420,0.039687, 0.707107,0.042047,
	0.749154,0.044547, 0.793701,0.047196, 0.840896,0.050002, 0.890899,0.052976, 0.943874,0.056126, 1.000000,0.059463, 1.059463,0.062999, 1.122462,0.066745, 1.189207,0.070714, 1.259921,0.074919, 1.334840,0.079374, 1.414214,0.084094, 1.498307,0.089094, 1.587401,0.094392, 1.681793,0.100005, 1.781797,0.105951,
	1.887749,0.112251, 2.000000,0.118926, 2.118926,0.125998, 2.244924,0.133490, 2.378414,0.141428, 2.519842,0.149837, 2.669680,0.158747, 2.828427,0.168187, 2.996614,0.178188, 3.174802,0.188784, 3.363586,0.200009, 3.563595,0.211902, 3.775497,0.224503, 4.000000,0.237852, 4.237852,0.251997, 4.489849,0.266980,
	4.756828,0.282856, 5.039684,0.299676, 5.339360,0.317494, 5.656854,0.336374, 5.993228,0.356377, 6.349605,0.377567, 6.727171,0.400018, 7.127189,0.423806, 7.550995,0.449005, 8.000000,0.475704, 8.475704,0.503993, 8.979697,0.533959, 9.513657,0.565711, 10.079368,0.599352, 10.678720,0.634989, 11.313708,0.672748,
	11.986456,0.712753, 12.699209,0.755134, 13.454343,0.800035, 14.254378,0.847611, 15.101990,0.898010, 16.000000,0.951412, 16.951412,1.007978, 17.959391,1.067923, 19.027313,1.131426, 20.158739,1.198696, 21.357435,1.269981, 22.627417,1.345499, 23.972916,1.425499, 25.398415,1.510271, 26.908686,1.600077, 28.508762,1.695211,
	30.203974,1.796026, 30.203974, 1.796026 };

float GetNotePowTable(float note)
{
	int iNote = floor(note);
	if (iNote < 0) iNote = 0;
	if (iNote > 127) iNote = 127;
	float frac = note - iNote;
	return PowTab[iNote * 2] + PowTab[iNote * 2 + 1] * frac;
};



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

float GetRandFloat()
{
	return rand() *  (1.0/(float)RAND_MAX);
}
float GetRandFloat32()
{
	return ((rand() *  (float)(65636.0f/RAND_MAX) )-32767.0f)*0.2f;
}

struct Phoneme
{
	float f1f, f1b;
	float f2f, f2b;
	float f3f, f3b;
	float fnf;
	float  a_voicing, a_aspiration, a_frication, a_bypass;
	float  a_1, a_2, a_3, a_4, a_n, a_56;
	float  duration, rank;
};

static Phoneme orgphonemes[] =
{
	{ 490,   60, 1480,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 64, 47, 40,-16,-16,  4,  2 }, //   0: a
	{ 790,  130, 1780,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 61, 52, 45,-16,-16,  5,  2 }, //   1: aa
	{ 640,   60, 1600,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 59, 49, 43,-16,-16,  6,  2 }, //   2: ai
	{ 640,   60, 2020,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 56, 52, 45,-16,-16,  6,  2 }, //   3: air
	{ 790,   60,  880,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 63, 43, 36,-16,-16, 15,  2 }, //   4: ar
	{ 490,   60,  820,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 59, 36, 31,-16,-16, 10,  2 }, //   5: aw
	{ 190,   60,  760,   90, 2500,  150,  270, 62,  0,  0,-16, 38,-16,-16,-16,-16,-16, 12, 26 }, //   6: b
	{ 190,   60,  760,   90, 2500,  150,  270, 62,  0,  0,-16, 38, 63, 57, 52,-16,-16,  1, 29 }, //   7: by
	{ 190,   60,  760,   90, 2500,  150,  270, 62,  0,  0,-16,-16,-16,-16,-16,-16,-16,  0, 26 }, //   8: bz
	{ 190,   60, 1780,   90, 2680,  150,  270,  0, 60, 60,-16,-16,-16,-16,-16,-16, 30,  4, 23 }, //   9: ch
	{ 400,   60, 2020,   90, 2560,  150,  270,  0, 60, 60,-16,-16, 45, 56, 45,-16, 20,  8, 18 }, //  10: ci
	{ 190,   60, 1780,   90, 2680,  150,  270, 62,  0,  0,-16, 45,-16,-16,-16,-16, 20,  8, 26 }, //  11: d
	{ 280,   60, 1600,   90, 2560,  150,  270, 36,  0, 60, 54, 43, 45, 40, 42,-16, 30,  4, 20 }, //  12: dh
	{ 280,   60, 1600,   90, 2560,  150,  270, 62,  0,  0,-16, 43, 45, 40, 42,-16, 10,  4, 20 }, //  13: di
	{ 190,   60, 1780,   90, 2680,  150,  270, 62,  0,  0,-16, 52, 52, 49, 59,-16,  0,  1, 29 }, //  14: dy
	{ 190,   60, 1780,   90, 2680,  150,  270, 62,  0,  0,-16, 52, 42, 38, 49,-16,-16,  1, 26 }, //  15: dz
	{ 640,   60, 2020,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 56, 52, 45,-16,-16,  4,  2 }, //  16: e
	{ 250,   60, 2320,   90, 3200,  150,  270, 62,  0,  0,-16, 64, 47, 50, 45,-16,-16,  7,  2 }, //  17: ee
	{ 490,   60, 1480,   90, 2500,  150,  270,  0,  0,  0,-16,-16,-16,-16,-16,-16,-16,  5, 31 }, //  18: end
	{ 580,   60, 1420,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 59, 47, 40,-16,-16, 16,  2 }, //  19: er
	{ 400,   60, 1420,   90, 2560,  150,  270,  0, 32, 54, 54,-16, 14, 14,-16,-16, 30, 12, 18 }, //  20: f
	{ 190,   60, 1480,   90, 2620,  150,  270, 62,  0,  0,-16, 49,-16,-16,-16,-16,-16, 12, 26 }, //  21: g
	{ 190,   60, 1480,   90, 2620,  150,  270, 62,  0,  0,-16, 49, 59, 54, 38,-16,-16,  1, 29 }, //  22: gy
	{ 190,   60, 1480,   90, 2620,  150,  270, 62,  0,  0,-16, 49, 49, 43, 28,-16,-16,  2, 26 }, //  23: gz
	{ 490,   60, 1480,   90, 2500,  150,  270,  0, 60, 60,-16, 49, 50, 40, 36,-16, 10,  5,  9 }, //  24: h
	{ 400,   60, 2080,   90, 2560,  150,  270, 62,  0,  0,-16, 64, 50, 49, 43,-16,-16,  6,  2 }, //  25: i
	{ 310,   60, 2200,   90, 2920,  150,  270, 62,  0,  0,-16, 64, 49, 50, 45,-16,-16,  6,  2 }, //  26: ia
	{ 490,   60, 1480,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 64, 47, 40,-16,-16,  6,  2 }, //  27: ib
	{ 790,   60,  880,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 63, 43, 36,-16,-16,  6,  2 }, //  28: ie
	{ 190,   60, 1780,   90, 2680,  150,  270, 62,  0,  0,-16, 45,-16,-16,-16,-16, 10,  4, 26 }, //  29: j
	{ 280,   60, 2020,   90, 2560,  150,  270, 62,  0,  0,-16, 43, 40, 50, 40,-16,  0,  3, 20 }, //  30: jy
	{ 190,   60, 1480,   90, 2620,  150,  270,  0, 60, 60,-16,-16,-16,-16,-16,-16, 10,  8, 23 }, //  31: k
	{ 190,   60, 1480,   90, 2620,  150,  270,  0, 60, 60,-16,-16, 64, 64, 43,-16, 10,  1, 29 }, //  32: ky
	{ 190,   60, 1480,   90, 2620,  150,  270,  0, 60, 60,-16,-16, 54, 54, 33,-16,  0,  4, 23 }, //  33: kz
	{ 460,   60, 1480,   90, 2500,  150,  270, 62,  0,  0,-16, 50, 40, 40, 35,-16,-16,  8, 11 }, //  34: l
	{ 460,   60,  940,   90, 2500,  150,  270, 62,  0,  0,-16, 50, 40, 40, 35,-16,-16,  8, 11 }, //  35: ll
	{ 480,   40, 1000,  170, 2200,  120,  360, 62,  0,  0,-16, 40, 44, 47,-16, 56,-16,  8, 11 }, //  36: m
	{ 480,   40, 1780,  300, 2620,  260,  450, 62,  0,  0,-16, 49, 49, 49, 34, 56,-16,  8, 11 }, //  37: n
	{ 480,  160,  820,  150, 2800,  100,  360, 52,  0,  0,-16, 34, 44, 49, 14, 56,-16,  8, 11 }, //  38: ng
	{ 610,   60,  880,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 61, 36, 29,-16,-16,  6,  2 }, //  39: o
	{ 490,   60, 1480,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 64, 47, 40,-16,-16,  6,  2 }, //  40: oa
	{ 490,   60,  820,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 59, 36, 31,-16,-16,  6,  2 }, //  41: oi
	{ 370,   60, 1000,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 56, 42, 36,-16,-16,  4,  2 }, //  42: oo
	{ 370,   60, 1000,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 56, 42, 36,-16,-16,  6,  2 }, //  43: oor
	{ 490,   60,  820,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 59, 36, 31,-16,-16,  6,  2 }, //  44: or
	{ 790,   60, 1300,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 61, 49, 42,-16,-16,  6,  2 }, //  45: ou
	{ 370,   60, 1000,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 56, 42, 36,-16,-16,  6,  2 }, //  46: ov
	{ 190,   60,  760,   90, 2500,  150,  270,  0, 60, 60,-16,-16,-16,-16,-16,-16,-16,  8, 23 }, //  47: p
	{ 190,   60,  760,   90, 2500,  150,  270,  0, 60, 60,-16, 38, 63, 57, 52,-16,-16,  1, 29 }, //  48: py
	{ 190,   60,  760,   90, 2500,  150,  270,  0, 60, 60,-16, 38, 52, 47, 42,-16,-16,  2, 23 }, //  49: pz
	{ 490,   60, 1480,   90, 2500,  150,  270,  0,  0,  0,-16,-16,-16,-16,-16,-16,-16,  6, 29 }, //  50: q
	{ 280,   60, 1420,   90, 2560,  150,  270, 62,  0,  0,-16, 43, 54, 50, 47, 40,-16,  0, 30 }, //  51: qq
	{ 490,   60, 1180,   90, 1600,  150,  270, 62,  0,  0,-16, 56, 49, 49,-16,-16,-16, 11, 10 }, //  52: r
	{ 490,   60, 1180,   90, 1600,   70,  270, 50,  0,  0,-16, 56, 49, 49,-16,-16,-16, 10, 10 }, //  53: rx
	{ 400,  200, 1720,   90, 2620,  220,  270,  0, 32, 60,-16,-16, 42, 42, 54, 50, 30, 12, 18 }, //  54: s
	{ 400,   60, 2200,   90, 2560,  150,  270,  0, 60, 60,-16,-16, 45, 56, 45, 40, 10, 12, 18 }, //  55: sh
	{ 190,   60, 1780,   90, 2680,  150,  270,  0, 60, 60,-16,-16,-16,-16,-16, 50, 20,  6, 23 }, //  56: t
	{ 400,   60, 1780,   90, 2680,  150,  270,  0, 60, 60,-16,-16, 40, 42, 36, 40, 10, 15, 18 }, //  57: th
	{ 190,   60, 1780,   90, 2680,  150,  270,  0, 60, 60,-16,-16,-16, 52, 64, 40,-16,  1, 29 }, //  58: ty
	{ 190,   60, 1780,   90, 2680,  150,  270,  0, 60, 60,-16,-16,-16, 42, 54, 30,-16,  2, 23 }, //  59: tz
	{ 700,   60, 1360,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 57, 45, 38,-16,-16,  6,  2 }, //  60: u
	{ 250,   60,  880,   90, 2200,  150,  270, 62,  0,  0,-16, 64, 52, 31, 24,-16,-16,  9,  2 }, //  61: uu
	{ 280,   60, 1420,   90, 2560,  150,  270, 62,  0,  0,-16, 43, 54, 50, 47, 40,-16,  4, 20 }, //  62: v
	{ 190,   60,  760,   90, 2020,  150,  270, 62,  0,  0,-16, 57, 42, 35,-16,-16,-16,  8, 10 }, //  63: w
	{ 190,   60, 1480,   90, 2620,  150,  270,  0, 60, 60,-16,-16, 54, 54, 33, 30,-16, 12, 18 }, //  64: x
	{ 250,   60, 2500,   90, 2980,  150,  270, 62,  0,  0,-16, 64, 47, 52, 45,-16,-16,  7, 10 }, //  65: y
	{ 280,   60, 1720,   90, 2560,  150,  270, 40, 20, 60,-16, 43, 38, 38, 50, 50,-16,  4, 20 }, //  66: z
	{ 280,   60, 2020,   90, 2560,  150,  270, 62,  0,  0,-16, 43, 40, 50, 40, 30, 10,  4, 20 }, //  67: zh
	{ 280,   60, 1720,   90, 2560,  150,  270, 62,  0,  0,-16, 43, 38, 38, 50, 30, 10,  4, 20 }, //  68: zz
};

static float flerp(const float a, const float b, const float x) { return a + x*(b - a); }


static float db2lin(float db1, float db2, float x) { return (float)GetNotePowTable((flerp(db1, db2, x)-35)*2); }

void SetFrame(BigFish_t *fish, const Phoneme &p1s, const Phoneme &p2s, const float x)
{
	static const float f4 = 3200;
	static const float f5 = 4000;
	static const float f6 = 6000;
	static const float bn = 100;
	static const float b4 = 200;
	static const float b5 = 500;
	static const float b6 = 800; 
	static Phoneme p1, p2;

	static const float * const p1f[] = { &p1.fnf,&p1.f1f,&p1.f2f,&p1.f3f,&f4    ,&f5     ,&f6 };
	static const float * const p1b[] = { &bn    ,&p1.f1b,&p1.f2b,&p1.f3b,&b4    ,&b5     ,&b6 };
	static const float * const p1a[] = { &p1.a_n,&p1.a_1,&p1.a_2,&p1.a_3,&p1.a_4,&p1.a_56,&p1.a_56 };

	static const float * const p2f[] = { &p2.fnf,&p2.f1f,&p2.f2f,&p2.f3f,&f4    ,&f5     ,&f6 };
	static const float * const p2b[] = { &bn    ,&p2.f1b,&p2.f2b,&p2.f3b,&b4    ,&b5     ,&b6 };
	static const float * const p2a[] = { &p2.a_n,&p2.a_1,&p2.a_2,&p2.a_3,&p2.a_4,&p2.a_56,&p2.a_56 };

	p1 = p1s;
	p2 = p2s;

	for (int i = 0; i < 7; i++)
		fish->VocalFilter.res[i].set(flerp(*p1f[i], *p2f[i], x)*1.0f, flerp(*p1b[i], *p2b[i], x), db2lin(*p1a[i], *p2a[i], x));

	fish->VocalFilter.Voicing = db2lin(p1.a_voicing, p2.a_voicing, x);
	fish->VocalFilter.Aspiration = db2lin(p1.a_aspiration, p2.a_aspiration, x);
	fish->VocalFilter.Frication= db2lin(p1.a_frication, p2.a_frication, x);
	fish->VocalFilter.Bypass= db2lin(p1.a_bypass, p2.a_bypass, x);
}


void BigFish_VocalFilter(struct BigFish_t *fish, int32_t *bufferin, int32_t *bufferout, int len, float FilterEnvScale, float FilterEnvResult)
{

	float cutoff = 2 * (fish->Parameters[FILTER_CUTOFF] / 65535.0f);
		//cutoff = cutoff + (FilterEnvResult * FilterEnvScale) / 32768.0f;
	//if (cutoff < 0) cutoff = 0; else if (cutoff > 1) cutoff = 1;
	//cutoff *= 5;
	//int IDX1 = floor(cutoff);
	//int IDX2 = IDX1 + 1;
	int F1 = floor((fish->Parameters[FILTER_RESONANCE] * 68 )  / 65535.0f);
	int F2 = floor((fish->Parameters[FILTER_PEAKS] * 68) / 65535.0f);
	int F3 = floor((fish->Parameters[FILTER_DRIFT] * 68) / 65535.0f);
	if (cutoff < 1)
	{
		SetFrame(fish, orgphonemes[F1], orgphonemes[F2], cutoff);
	}
	else
	{
		SetFrame(fish, orgphonemes[F2], orgphonemes[F3], cutoff-1);
	}

	for (int i = 0; i < len; i++)
	{
		float in = bufferin[i];
		// add aspiration noise
		in = in*fish->VocalFilter.Voicing + GetRandFloat32()*fish->VocalFilter.Aspiration;

		// process complete input signal with f1/nasal filters
		float out = fish->VocalFilter.res[0].tick(in) + fish->VocalFilter.res[1].tick(in);


		// differentiate input signal, add frication noise
		float lin = in;
		in = (GetRandFloat32()*fish->VocalFilter.Frication) + in - fish->VocalFilter.lastin2;
		fish->VocalFilter.lastin2 = lin;

		// process diff/fric input signal with f2..f6 and bypass (phase inverted)
		for (int r = 2; r < 7; r++)
		{	
			out = fish->VocalFilter.res[r].tick(in) - out;
		}

		out = in*fish->VocalFilter.Bypass - out;

		// high pass filter
		fish->VocalFilter.hpb1 += 0.012f*(out = out - fish->VocalFilter.hpb1);
		fish->VocalFilter.hpb2 += 0.012f*(out = out - fish->VocalFilter.hpb2);

		// EQ
		//out = fish->VocalFilter.peq1.tick(out) - out;
		bufferout[i] = out;
	}
}




void BigFish_Filter(struct BigFish_t *fish, int32_t *bufferin, int32_t *bufferout, int len)
{
	float cutoffin = fish->Parameters[FILTER_CUTOFF] / 65535.0f;
	float V = 0.1 + 0.9*cutoffin;
	float freq = 20000 * (V*V*V);

	float mul[__FILTERKEYTRACK_COUNT] = { -2.0f,-1.5f, -1.0f,-0.5f,0.0f,0.5f,1.0f,1.5f,2.0f };
	BigFish_SteppedResult_t sr;
	BigFish_GetSteppedResult(fish->Parameters[FILTER_KEYTRACK], __FILTERKEYTRACK_COUNT - 1, &sr);

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
		freq = 440.0f * GetNotePowTable(note);
		freq += fish->CenterFreq * BigFish_GetInterpolatedResultFloat(mul, &sr);

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
	float freq = 440 * GetNotePowTable(Note  + 69);
	fish->CenterFreq = freq;


	int32_t A[MAXFISHBUFFER];
	int32_t B[MAXFISHBUFFER];
	int32_t OSC[MAXFISHBUFFER];
	int32_t AMPENV[MAXFISHBUFFER];
	int32_t AMPED[MAXFISHBUFFER];
	int32_t MAIN[MAXFISHBUFFER];
	typedef void GenFunc(BigFish_t *, int32_t *, int32_t *, int, int32_t);


	BigFish_SteppedResult_t src;
	BigFish_GetSteppedResult(fish->Parameters[PITCH_CHORD], 17, &src);
	
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

	BigFish_SteppedResult_t sr;
	BigFish_GetSteppedResult(fish->Parameters[OSC_SHAPE], 6, &sr);

	int FuncIdx = sr.index;
	int32_t crossfade = sr.fractional;
	int32_t icrossfade = 256 - crossfade;

	ADSR_Update(&fish->AmpEnvelope, fish->SampleRate);

	while (len > 0)
	{
		int L = __min(len, MAXFISHBUFFER);
		len -= L;
		int32_t Size = fish->Parameters[OSC_SIZE];
		
		for (int i = 0; i < L; i++)
		{
			AMPENV[i] = ADSR_GetCurved(&fish->AmpEnvelope);
		}


		if (sr.fractional > 0)
		{

			float frac = sr.fractional * (1.0f / 255.0f);
			float ifrac = 1.0f - frac;
			Funcs[FuncIdx](fish, A, B, L, Size * ifrac );
			Funcs[FuncIdx + 1](fish, B, A, L, Size * frac);

			for (int i = 0; i < L; i++)
			{
				OSC[i] = (((B[i] * crossfade + A[i] * icrossfade) / 256));
				int64_t inter = ((int64_t)OSC[i] * (int64_t)AMPENV[i]);
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
				
				OSC[i] = A[i];
				int64_t inter = ((int64_t)OSC[i] * (int64_t)AMPENV[i]);
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