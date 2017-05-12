#pragma once
#ifndef PLATINUMCLIP
#define PLATINUMCLIP

#include <vector>

#include "RBJfilter.h"
extern const int32_t  clean_lookup[];
extern const int32_t  fuzz_lookup[];

#define MINIMUM 0.001

struct EqParam
{
	double freq;
	double q;
	double gain;
};

static const EqParam eqdefaultsA[] = {
	{ 63.0, 0.68, -4.375 },
	{ 160.0, 0.8, -7.74 },
	{ 400.0, 0.64, -3.365 },
	{ 1000.0, 0.62, -2.358 },
	{ 2500.0, 0.61, -1.682 }
};

static const EqParam eqdefaultsB[] = {
	{ 63.0, 0.68, 4.373 },
	{ 160.0, 0.68, 4.373 },
	{ 1000.0, 0.62, 2.017 }
};


template<int L, const int32_t COEFF[], int LEN, int SHIFT> class Interpolator {
public:
	int32_t* process(int32_t *input, int length) {
		if (input == NULL) {
			memset(&_buffer[0], 0, length * sizeof(int));
			return &_buffer[0];
		}

		for (int i = 0; i < length; i++) {
			int32_t x = input[i] << SHIFT;

			int32_t frac = x & 0xFFFFFF;
			int32_t pos = int32_t((signed char)((x & 0xFF000000) >> 24)) + (LEN/2);

			int value =
				COEFF[pos * 4] +
				int32_t
				((int64_t(frac) * (
				int64_t(COEFF[pos * 4 + 1] +
				int32_t((int64_t(frac) * (
				int64_t(COEFF[pos * 4 + 2] +
				int32_t((int64_t(frac) * int64_t(COEFF[pos * 4 + 3])) >> 24))
				)) >> 26))
				)) >> 30);

			_buffer[i] = value;
		}

		return &_buffer[0];
	}

private:
	int32_t _buffer[L];
};


template<typename F, int L> class PeakingFilter {
public:
	PeakingFilter() {
		_sr = 44100.0;
		_freq = 1000.0;
		_q = 0.707;
		_gain = 1.0;
		reset();
		recalc();
	}

	void set_samplerate(double rate) {
		_sr = rate;
		recalc();
	}

	void set_freq(double freq) {
		_freq = freq;
		recalc();
	}

	void set_q(double q) {
		_q = q;
		recalc();
	}

	void set_gain(double gain) {
		_gain = gain;
		recalc();
	}

	void reset() {
		_x1 = 0;
		_x2 = 0;
		_y1 = 0;
		_y2 = 0;
	}

	bool quiet() {
		if (!_y1 && !_y2)
			return true;
		return false;
	}

	F* process(F* input, int length) {

		int b0 = _b0;
		int b1 = _b1;
		int b2 = _b2;
		int a1 = _a1;
		int a2 = _a2;
		int y1 = _y1;
		int y2 = _y2;
		int x1 = _x1;
		int x2 = _x2;

		if (input == NULL) {
			for (int i = 0; i < length; i++) {
				int64_t out = int64_t(b1) * int64_t(x1) + int64_t(b2) * int64_t(x2) + int64_t(a1) * int64_t(y1) + int64_t(a2) * int64_t(y2);
				int out32 = int(out >> 24);
				x2 = x1;
				x1 = 0;
				y2 = y1;
				y1 = out32;
			}

			_y1 = y1;
			_y2 = y2;
			_x1 = x1;
			_x2 = x2;

			return NULL;
		}

		for (int i = 0; i < length; i++) {
			int in = input[i];// * double(1 << 8);

			int64_t out = int64_t(b0) * int64_t(in) + int64_t(b1) * int64_t(x1) + int64_t(b2) * int64_t(x2) + int64_t(a1) * int64_t(y1) + int64_t(a2) * int64_t(y2);
			int out32 = int(out >> 24);
			x2 = x1;
			x1 = in;
			y2 = y1;
			y1 = out32;

			_buffer[i] = out32;// * (1.0 / double(1 << 8));
		}

		_y1 = y1;
		_y2 = y2;
		_x1 = x1;
		_x2 = x2;

		return &_buffer[0];
	}

protected:
	double _sr;
	double _freq;
	double _q;
	double _gain;
	int _a1;
	int _a2;
	int _b0;
	int _b1;
	int _b2;
	int _y1;
	int _y2;
	int _x1;
	int _x2;
	F _buffer[L];

	void recalc(void) {
		double b0, b1, b2, a1, a2;
		dsp::RbjFilter::rbjcompute(_freq / _sr, _q, _gain, 5,
			&b0, &b1, &b2, &a1, &a2);
		_b0 = int(b0 * double(1 << 24));
		_b1 = int(b1 * double(1 << 24));
		_b2 = int(b2 * double(1 << 24));
		_a1 = int(a1 * double(1 << 24));
		_a2 = int(a2 * double(1 << 24));
	}
};

class PlatinumClip
{
public:


	void Init(int samplerate)
	{
		int i;

		_active = false;

		for (i = 0; i < 5; i++) {
			eqA[i].set_samplerate(samplerate);
			eqA[i].set_freq(eqdefaultsA[i].freq);
			eqA[i].set_q(eqdefaultsA[i].q);
			eqA[i].set_gain(pow(10.0, eqdefaultsA[i].gain / 20.0));
		}

		for (i = 0; i < 3; i++) 
		{
			eqB[i].set_samplerate(samplerate);
			eqB[i].set_freq(eqdefaultsB[i].freq);
			eqB[i].set_q(eqdefaultsB[i].q);
			eqB[i].set_gain(pow(10.0, eqdefaultsB[i].gain / 20.0));
		}

		reset();
	}

	void SetPush(unsigned char npush)
	{
		push = npush;
		Recalc();
	}

	void SetType(unsigned char type)
	{
		if (type == 0) {
				fuzz = false;
		}
		else
		{ 
			fuzz = true;
		};
		Recalc();
	}

	void Recalc()
	{
		if (fuzz) {
			float gain = powf(10.0f, (push * (1.0f / 128.0f) * 8.5f + 9.5f) / 20.0f);
			_gain = int(96.0f * gain);
			_postgain = int(-1024.0f * 0.28f * powf(gain, -1.0f));
		}
		else {
			float gain = powf(10.0f, (push * push * (1.0f / 16384.0f) * 32.0f + 2.0f) / 20.0f);
			_gain = int(256.0f * gain);
			_postgain = int(256.0f * 0.8f * powf(gain, -0.75f));
		}
	}


	void reset()
	{
		int i;

		for (i = 0; i < 5; i++) {
			eqA[i].reset();
		}

		for (i = 0; i < 3; i++) {
			eqB[i].reset();
		}
	}

	bool IntProcess(int32_t *samplebuf, int n)
	{
		bool active = false;
		int i, k;
		static const int hardclipgain = 55; // /32
	
		// pre-eq
		for (k = 0; k < 5; k++)
		{
			int32_t* r = eqA[k].process(samplebuf, n);
			memcpy(samplebuf, r, n * sizeof(int));
		}

		// hardclip
		for (i = 0; i < n; i++)
		{
			samplebuf[i] = (samplebuf[i] * hardclipgain) >> 5;
			if (samplebuf[i] < -(1 << 23)) {
				samplebuf[i] = -(1 << 23);
			}
			if (samplebuf[i] > (1 << 23)-1) {
				samplebuf[i] = (1 << 23)-1;
			}
		}

		// boost
		int gain = _gain;
		for (i = 0; i < n; i++) {
			int64_t res = int64_t(samplebuf[i]) * int64_t(gain);
			samplebuf[i] = int(res >> 8);
		}

		if (fuzz) 
		{
			for (i = 0; i < n; i++)
			{
				if (samplebuf[i] < -(1 << 23)) {
					samplebuf[i] = -(1 << 23);
				}
				if (samplebuf[i] > (1 << 23)-1) {
					samplebuf[i] = (1 << 23)-1;
				}
			}
			int32_t* r = fuzzdistort.process(samplebuf, n);
			int postgain = _postgain;
			for (i = 0; i < n; i++) {
				samplebuf[i] = (int64_t(r[i]) * int64_t(postgain)) >> 6;
			}
		}
		else {
			// clean
			int postgain = _postgain;

			int32_t* r = cleandistort.process(samplebuf, n);
			for (i = 0; i < n; i++) {
				samplebuf[i] = (int64_t(r[i]) * postgain) >> 8;
			}

		}

		// post-eq
		for (k = 0; k < 3; k++) {
			int32_t* r = eqB[k].process(samplebuf, n);
			memcpy(samplebuf, r, n * sizeof(int));

			for (i = 0; i < n; i++) {
				if (samplebuf[i] > (1<<23)-1) samplebuf[i] = (1<<23)-1;
				else if (samplebuf[i] < -(1<<23)) samplebuf[i] = -(1<<23);
			}
		}

		if (!active) 
		{
			for (i = 0; i < n; i++)
			{
				if (samplebuf[i] != 0)
				{
					active = true;
					break;
				}
			}
		}

		return active;
	}
	


	bool bypass;
	int push;
	bool fuzz;

	int _gain; // 1.0 .. 64.0 /256 !!!!
	int _postgain; // /256

	bool _active;

	
	PeakingFilter<int32_t, 32> eqA[5];
	PeakingFilter<int32_t, 32>  eqB[3];

	Interpolator<32, clean_lookup, 256, 0> cleandistort;
	Interpolator<32, fuzz_lookup, 256, 3> fuzzdistort;

	

};





#endif
