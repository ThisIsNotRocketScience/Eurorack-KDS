#pragma once

#ifndef BIGFISH
#define BIGFISH

#include <stdint.h>
#define MAXFISHBUFFER 32

#include "BigFishDefs.h"
#include "ADSREnvelope.h"
#include "PlatinumClip.h"
#include "BleppyOscs.h"
#include "OrganOsc.h"

#ifndef __min
#define __min(a,b) (((a)<(b))?(a):(b))
#endif



typedef struct Inertia_t
{
	uint32_t Target;
	uint32_t Current;
	uint16_t Speed;
} Inertia_t;




#define FILTERBITS 24

class Filter
{
public:
	int32_t A1, A2, B0, B1, B2;
	int32_t X1, X2, Y2, Y1;
	double m_fB2;
	double m_fB0;
	double m_fB1;
	double m_fA1;
	double m_fA2;
	float fx1, fx2, fy1, fy2;
	Filter()
	{
		Reset();
	}
	void Reset()
	{
		X2 = X1 = Y1 = Y2 = 0;
		A1 = 0; A2 = 0; B0 = 0; B1 = 0; B2 = 0;
		A1p = 0; A2p = 0; B0p = 0; B1p = 0; B2p = 0;

		fx1 = fx2 = fy1 = fy2 = 0;
	}

	inline void zoelzerLPF(double fc, double Q, double odsr, double gain = 1.0)
	{
		float omega = (float)(3.141592654f*fc*odsr);
		float omega2 = omega*omega;
		//		float K=omega+omega2*omega*(1.0/2.5)+omega2*omega2*omega2*omega*(1.0/4);
		float K = (float)(omega + omega2*omega2*omega*(1.0 / 1.45));
		float KK = K*K;
		float QK = (float)(Q*(KK + 1));
		float iQK = 1.0f / (QK + K);
		float inv = (float)(Q*iQK);
		m_fB2 = (float)(gain*KK*inv);
		m_fB0 = m_fB2;
		m_fB1 = (float)(2 * m_fB0);
		m_fA1 = (float)(2 * (KK - 1)*inv);
		m_fA2 = (float)((QK - K)*iQK);

		A1 = int(m_fA1 * (double)(1 << FILTERBITS));
		A2 = int(m_fA2 * (double)(1 << FILTERBITS));
		B0 = int(m_fB0 * (double)(1 << FILTERBITS));
		B1 = int(m_fB1 * (double)(1 << FILTERBITS));
		B2 = int(m_fB2 * (double)(1 << FILTERBITS));

	}

	void rbjBPF(float fc, float bw, float odsr)
	{
		float omega = (float)(2 * 3.141592654f*fc * odsr);
		float sn = (float)sin(omega);
		float cs = (float)cos(omega);
		float alpha = (float)(sn / sinh(log(2.0f) / 2 * bw*omega / sn));

		float inv = (float)(1.0 / (1.0 + alpha));

		m_fB0 = (float)(inv*alpha);
		m_fB1 = (float)0;
		m_fB2 = (float)(-inv*alpha);
		m_fA1 = (float)(-2 * cs*inv);
		m_fA2 = (float)((1 - alpha)*inv);


		A1 = int(m_fA1 * (double)(1 << FILTERBITS));
		A2 = int(m_fA2 * (double)(1 << FILTERBITS));
		B0 = int(m_fB0 * (double)(1 << FILTERBITS));
		B1 = int(m_fB1 * (double)(1 << FILTERBITS));
		B2 = int(m_fB2 * (double)(1 << FILTERBITS));
	}


	void rbjHPF(double fc, double Q, double odsr, double gain = 1.0)
	{
		float omega = (float)(2 * 3.141592654f*fc * odsr);
		//    float sn=sin(omega);
		//    float cs=cos(omega);
		float sn = (float)(omega + omega*omega*omega*(1.0 / 6.0) + omega*omega*omega*omega*omega*(1.0 / 120));
		float cs = (float)(1 - omega*omega*(1.0 / 2.0) + omega*omega*omega*omega*(1.0 / 24));
		float alpha = (float)(sn / (2 * Q));

		float inv = (float)(1.0 / (1.0 + alpha));

		m_fB0 = (float)(gain*inv*(1 + cs) / 2);
		m_fB1 = (float)(gain*-inv*(1 + cs));
		m_fB2 = (float)(gain*inv*(1 + cs) / 2);
		m_fA1 = (float)(-2 * cs*inv);
		m_fA2 = (float)((1 - alpha)*inv);

		A1 = int(m_fA1 * (double)(1 << FILTERBITS));
		A2 = int(m_fA2 * (double)(1 << FILTERBITS));
		B0 = int(m_fB0 * (double)(1 << FILTERBITS));
		B1 = int(m_fB1 * (double)(1 << FILTERBITS));
		B2 = int(m_fB2 * (double)(1 << FILTERBITS));
	}
	void rbjBPF_withgain(float fc, float Q, float odsr, float gain = 1.0)
	{
		float omega = (float)(2 * 3.141592654f*fc * odsr);
		//    float sn=sin(omega);
		//    float cs=cos(omega);
		float sn = (float)(omega + omega*omega*omega*(1.0 / 6.0) + omega*omega*omega*omega*omega*(1.0 / 120));
		float cs = (float)(1 - omega*omega*(1.0 / 2.0) + omega*omega*omega*omega*(1.0 / 24));
		float alpha = (float)(sn / (2 * Q));

		float inv = (float)(1.0 / (1.0 + alpha));

		m_fB0 = (float)(gain*inv*alpha);
		m_fB1 = (float)(0);
		m_fB2 = (float)(-gain*inv*alpha);
		m_fA1 = (float)(-2 * cs*inv);
		m_fA2 = (float)((1 - alpha)*inv);

		A1 = int(m_fA1 * (double)(1 << FILTERBITS));
		A2 = int(m_fA2 * (double)(1 << FILTERBITS));
		B0 = int(m_fB0 * (double)(1 << FILTERBITS));
		B1 = int(m_fB1 * (double)(1 << FILTERBITS));
		B2 = int(m_fB2 * (double)(1 << FILTERBITS));
	}
	void rbjBRF(float fc, float Q, float odsr, float gain = 1.0)
	{
		float omega = (float)(2 * 3.141592654f*fc * odsr);
		//    float sn=sin(omega);
		//    float cs=cos(omega);
		float sn = (float)(omega + omega*omega*omega*(1.0 / 6.0) + omega*omega*omega*omega*omega*(1.0 / 120));
		float cs = (float)(1 - omega*omega*(1.0 / 2.0) + omega*omega*omega*omega*(1.0 / 24));
		float alpha = (float)(sn / (2 * Q));

		float inv = (float)(1.0 / (1.0 + alpha));

		m_fB0 = (float)(gain*inv);
		m_fB1 = (float)(-gain*inv * 2 * cs);
		m_fB2 = (float)(gain*inv);
		m_fA1 = (float)(-2 * cs*inv);
		m_fA2 = (float)((1 - alpha)*inv);

		A1 = int(m_fA1 * (double)(1 << FILTERBITS));
		A2 = int(m_fA2 * (double)(1 << FILTERBITS));
		B0 = int(m_fB0 * (double)(1 << FILTERBITS));
		B1 = int(m_fB1 * (double)(1 << FILTERBITS));
		B2 = int(m_fB2 * (double)(1 << FILTERBITS));
	}
	inline void rbjLPF(double fc, double Q, double esr, double gain = 1.0)
	{
		float omega = (float)(2 * 3.141592654f *fc / esr);
		float sn = (float)sin(omega);
		float cs = (float)cos(omega);
		float alpha = (float)(sn / (2 * Q));



		float m_fA0inv = 1.0f / (1 + alpha);

		m_fB0 = (1 - cs) / 2;
		m_fB1 = 1 - cs;
		m_fB2 = (1 - cs) / 2;
		m_fA1 = -2 * cs;
		m_fA2 = 1 - alpha;

		m_fB0 *= m_fA0inv;
		m_fB1 *= m_fA0inv;
		m_fB2 *= m_fA0inv;
		m_fA1 *= m_fA0inv;
		m_fA2 *= m_fA0inv;


		A1 = int(m_fA1 * (double)(1 << FILTERBITS));
		A2 = int(m_fA2 * (double)(1 << FILTERBITS));
		B0 = int(m_fB0 * (double)(1 << FILTERBITS));
		B1 = int(m_fB1 * (double)(1 << FILTERBITS));
		B2 = int(m_fB2 * (double)(1 << FILTERBITS));

	}

	void ClearException()
	{
		if (fy1 != fy1) fy1 = 0;
		if (fy2 != fy2) fy2 = 0;
	}

	void Process(float *input, float *to, int length)
	{
		for (int i = 0; i < length; i++)
		{
			int in = (int)*input++;// * double(1 << 8);

			float o = (float)in;
			float n = (float)( o*m_fB0 + fx1*m_fB1 + fx2*m_fB2 - fy1*m_fA1 - fy2*m_fA2 ) ;
			fy2 = fy1;
			fy1 = n;
			fx2 = fx1;
			fx1 = o;
			*to++ = n;
		}
	}

	float FilterSingle(float in)
	{
		

		float o = (float)in;
		float n = (float)(o*m_fB0 + fx1*m_fB1 + fx2*m_fB2 - fy1*m_fA1 - fy2*m_fA2);
		fy2 = fy1;
		fy1 = n;
		fx2 = fx1;
		fx1 = o;
		return n;
	}
	
	int32_t B0p, B1p, B2p, A1p, A2p;
	
	void Process(int32_t *input, int32_t *to, int length)
	{
		int B0inc = (B0 - B0p) / length;
		int B1inc = (B1 - B1p) / length;
		int B2inc = (B2 - B2p) / length;
		int A1inc = (A1 - A1p) / length;
		int A2inc = (A2 - A2p) / length;
		
		


		int b0 = B0p;
		int b1 = B1p;
		int b2 = B2p;
		int a1 = A1p;
		int a2 = A2p;
		int y1 = Y1;
		int y2 = Y2;
		int x1 = X1;
		int x2 = X2;

	/*	if (input == NULL) {
			for (int i = 0; i < length; i++) {
				int64_t out = int64_t(b1) * int64_t(x1) + int64_t(b2) * int64_t(x2) - int64_t(a1) * int64_t(y1) - int64_t(a2) * int64_t(y2);
				int out32 = int(out >> 24);
				x2 = x1;
				x1 = 0;
				y2 = y1;
				y1 = out32;
				*to++ = out32;

				b0 += B0inc;
				b1 += B1inc;
				b2 += B2inc;
				a1 += A1inc;
				a2 += A2inc;
			}

			Y1 = y1;
			Y2 = y2;
			X1 = x1;
			X2 = x2;

			 B0= b0 ;
			 B1 = b1  ;
			 B2 = b2  ;
			 A1 = a1  ;
			 A2 = a2  ;
			return;
		}*/

		for (int i = 0; i < length; i++) {
			int in = *input++ << 6;// * double(1 << 8);

			int64_t out = int64_t(b0) * int64_t(in)
				+ int64_t(b1) * int64_t(x1)
				+ int64_t(b2) * int64_t(x2)
				- int64_t(a1) * int64_t(y1)
				- int64_t(a2) * int64_t(y2);

			int out32 = int(out >> (FILTERBITS));
			x2 = x1;
			x1 = in;
			y2 = y1;
			y1 = out32;

			*to++ = out32 >> 6;// * (1.0 / double(1 << 8));


			b0 += B0inc;
			b1 += B1inc;
			b2 += B2inc;
			a1 += A1inc;
			a2 += A2inc;
		}

		Y1 = y1;
		Y2 = y2;
		X1 = x1;
		X2 = x2;
		B0p = b0;
		B1p = b1;
		B2p = b2;
		A1p = a1;
		A2p = a2;
	}
};

__inline float zOSSquashDenormals(float v)
{
	return (v>-(1.0 / 1048576) && v<(1.0 / 1048576)) ? 0.f : v;
}


class cwaveguide
{
public:
#define MAX_WG_DELAY 20
	float buffer[MAX_WG_DELAY];
	cwaveguide()
	{
		clear();
		F.rbjHPF(40, 0.7, 44100, 1.0);
		F2.rbjLPF(4000, 0.7, 44100, 1.0);
	}

	void Safety()
	{
		for (int s = 0; s<MAX_WG_DELAY; s++)
		{
			//	if (isnan(buffer[s]) )			buffer[s]=0;
		};
	};

	~cwaveguide()
	{		
	};

	void clear()
	{
		counter = 0;
		for (int s = 0; s<MAX_WG_DELAY; s++)  buffer[s] = 0;
	}

	Filter F;
	Filter F2;
	inline float feed(float const in, float const feedback, double  delay)
	{
		if (delay <= 0)
		{
		}
		else if (delay > MAX_WG_DELAY - 1) delay = MAX_WG_DELAY;
		
		// calculate delay offset
		double back = (double)counter - delay;

		// clip lookback buffer-bound
		while (back<0.0) back += MAX_WG_DELAY;
		while (back>MAX_WG_DELAY) back -= MAX_WG_DELAY;


		// compute interpolation left-floor
		int const index0 = (int)floor(back);

		// compute interpolation right-floor
		int index_1 = index0 - 1;
		int index1 = index0 + 1;
		int index2 = index0 + 2;

		// clip interp. buffer-bound
		if (index_1<0)index_1 = MAX_WG_DELAY - 1;
		if (index1 >= MAX_WG_DELAY)index1 = 0;
		if (index2 >= MAX_WG_DELAY)index2 = 0;

		// get neighbourgh samples
		float const y_1 = buffer[index_1];
		float const y0 = buffer[index0];
		float const y1 = buffer[index1];
		float const y2 = buffer[index2];

		// compute interpolation x
		float const x = (float)back - (float)index0;

		// calculate
		float const c0 = y0;
		float const c1 = 0.5f*(y1 - y_1);
		float const c2 = y_1 - 2.5f*y0 + 2.0f*y1 - 0.5f*y2;
		float const c3 = 0.5f*(y2 - y_1) + 1.5f*(y0 - y1);

		float const output = ((c3*x + c2)*x + c1)*x + c0;

		// add to delay buffer
		buffer[counter] = zOSSquashDenormals(in + F2.FilterSingle(F.FilterSingle(output*feedback)));

		// increment delay counter
		counter++;

		// clip delay counter
		if (counter >= MAX_WG_DELAY)
			counter = 0;

		// return output
		return output;
	}
	int counter;
};
//#define RANGECHECKS
#ifdef RANGECHECKS
typedef struct RangeChecker_t
{
	int64_t min;
	int64_t max;
	int64_t valuecount;
	int32_t bits;
	bool signedvalues;
} RangeChecker_t;

void InitRange(RangeChecker_t *R);
void UpdateRange(RangeChecker_t *R, int64_t inv);
void UpdateRangeBuffer(RangeChecker_t *R, int32_t *buffer, int len);

#endif

typedef struct BigFish_t
{

	int SampleRate;
	float ODSR;
	float CenterFreq;
	int32_t PitchInput; // (1<<14) = 1 octave

	uint16_t Parameters[__PARAMCOUNT]; // prenormalized to 16bit.
	uint16_t Leds[__LEDCOUNT];
	uint8_t Gates[__GATECOUNT];
	uint8_t PreviousGates[__GATECOUNT];

	Filter filters[10];

	VosimBlep_t Vosim;
	HyperOsc_t HyperSawOsc;
	HyperPulse_t HyperPulseOsc;
	MinBlepOsc_t SawOsc[3];
	MinBlepOsc_t PulseOsc[3];
	WaveBlep_t WaveOsc[3];
	Organ_t Organ;
	int ChordVoicesActive;
	double FormantMemory[10];
	float ChordPitchTable[3];

	Inertia_t Accent;
	struct ADSR_Envelope_t AmpEnvelope;
	struct ADSR_Envelope_t FilterEnvelope; // s and r set to 0 for plain AD envelope. 
	cwaveguide Resonator[5];
	PlatinumClip Clipper;

#ifdef RANGECHECKS
	RangeChecker_t PreFilter;
	RangeChecker_t PostFilter;
	RangeChecker_t PostDrive;
	RangeChecker_t HyperSawRange;
	RangeChecker_t OrganRange;
	RangeChecker_t HyperPulseRange;
	RangeChecker_t WaveBlepRange;
	RangeChecker_t SuperFishRange;
#endif
} BigFish_t;

typedef struct SteppedResult_t
{
	uint8_t index;
	uint8_t fractional;
} SteppedResult_t;

void GetSteppedResult(uint16_t param, uint8_t steps, SteppedResult_t *out);

void BigFish_Init(struct BigFish_t *fish, int samplerate);
void BigFish_Update(struct BigFish_t *fish);
void BigFish_CheckGates(struct BigFish_t *fish);
inline void BigFish_GenerateBlock(struct BigFish_t *fish, int32_t *filterInput, int32_t *bufferOSCOUT, int32_t *bufferMAIN, int len);


float *GenerateMinBLEP(int zeroCrossings, int overSampling);

#endif //  __cpp


