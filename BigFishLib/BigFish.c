#include "BigFish.h"
#include "bleptables.h"
#include <math.h>


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
		for (int i = 0; i < 47; i++)
		{
			float tempIndex = (exactCrossTime*32.0f) + (i*32.0f);
			float tempFraction = tempIndex - (float)floor(tempIndex);
			osc->circularBuffer[(osc->index + i) % 48] += (float)(1.0f - LERP(tempFraction, mblep_48000[(int)floor(tempIndex)], mblep_48000[(int)ceil(tempIndex)]));
		}
	}

	osc->circularBuffer[osc->index] += (osc->mPhase - osc->mPhaseIncrement);
	float output = osc->circularBuffer[osc->index];
	osc->circularBuffer[osc->index] = 0.0f;
	return output;
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
			for (int i = 0; i < 47; i++)
			{
				float tempIndex = (exactCrossTime*32.0f) + (i*32.0f);
				float tempFraction = tempIndex - (float)floor(tempIndex);
				osc->circularBuffer[(osc->index + i) % 48] += (float)(1.0 - LERP(tempFraction, mblep_48000[(int)floor(tempIndex)], mblep_48000[(int)ceil(tempIndex)]));
			}
		}

		osc->circularBuffer[osc->index] += (osc->mPhase[o] - osc->mPhaseIncrement[o]);
	}

	float output = osc->circularBuffer[osc->index];
	osc->circularBuffer[osc->index] = 0.0f;
	return output;
}

void HyperOsc_Update(struct HyperOsc_t *osc)
{

}

void ADSR_Trigger(struct ADSR_Envelope_t *adsr)
{

}

void ADSR_Release(struct ADSR_Envelope_t *adsr)
{

}


void BigFish_GateOn(struct BigFish_t *fish)
{
	// trigger main env
	// trigger filter env
	ADSR_Trigger(&fish->AmpEnvelope);
	ADSR_Trigger(&fish->FilterEnvelope);
}

void BigFish_GateOff(struct BigFish_t *fish)
{
	ADSR_Release(&fish->AmpEnvelope);
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


void BigFish_Init(struct BigFish_t *fish)
{
	Inertia_Init(&fish->Accent);
	HyperOsc_Init(&fish->HyperPulseOsc);
	HyperOsc_Init(&fish->HyperSawOsc);
}

#define CHECKGATE(f, g, on,off) if (f->Gates[g] != f->PreviousGates[g]){f->PreviousGates[g] = f->Gates[g]; if(f->Gates[g]) on(f);else off(f);};

void BigFish_Update(struct BigFish_t *fish)
{
	CHECKGATE(fish, FISHGATE_ACCENT, BigFish_AccentOn, BigFish_AccentOff);
	CHECKGATE(fish, FISHGATE_GATE, BigFish_GateOn, BigFish_GateOff);
}

void HyperSaw(struct BigFish_t *fish, int32_t *buffer, int len)
{
}

void HyperPulse(struct BigFish_t *fish, int32_t *buffer, int len)
{
}

void SuperFish(struct BigFish_t *fish, int32_t *buffer, int len)
{
}

void FM(struct BigFish_t *fish, int32_t *buffer, int len)
{
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


void BigFish_GenerateBlock(struct BigFish_t *fish, int32_t *buffer, int len)
{
	int shapemul = (fish->Parameters[OSC_SHAPE] * 5 * 256) / 65536;
	int32_t A[MAXFISHBUFFER];
	int32_t B[MAXFISHBUFFER];

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
		int32_t icrossfade = 255 - crossfade;
		for (int i = 0; i < L; i++)
		{
			*buffer++ = (B[i] * crossfade + A[i] * icrossfade)/ 256;
		}
	}

}