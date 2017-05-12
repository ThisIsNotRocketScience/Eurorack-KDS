#ifndef DSP_RBJFILTER_H
#define DSP_RBJFILTER_H

//#include <math.h>

#define undenormalise(sample) if(((*(unsigned int*)&(sample))&0x7f800000)==0) (sample)=0.0f

namespace dsp {

enum RbjFilterMode {
	LP2,
	HP2,
	BP2,
	BS2,
};

class RbjFilterCoeffs {
public:
	RbjFilterCoeffs() { a1 = a2 = b0 = b1 = b2 = 0; }

	double a1, a2;
	double b0, b1, b2;
};

class RbjFilterStateCoeffs {
public:
	RbjFilterStateCoeffs() { d0 = d1 = 0; }
	double d0, d1;
};

#define RBJ_MAX_STAGES 2

class RbjFilterState {
public:
	int mode;
	RbjFilterStateCoeffs state[RBJ_MAX_STAGES];
};

class RbjFilter {
public:
	RbjFilter() { samplerate = 44100.0; }
	~RbjFilter() { }

	void setsamplerate(double rate) {
		this->samplerate = rate;
	}

	RbjFilterState *reset(RbjFilterMode mode) {
		RbjFilterState *state = new RbjFilterState();
		state->mode = mode;

		for(int i = 0; i < RBJ_MAX_STAGES; i++) {
			state->state[i].d0 = state->state[i].d1 = 0;
		}

		return state;
	}

	float filter(RbjFilterState *state, float sample,
		double f, double q) {

		int stages;
		RbjFilterCoeffs coeffs[RBJ_MAX_STAGES];

		switch(state->mode) {
		case LP2:
			stages = 1;
			rbjcompute(f / samplerate, q, 0, 0, 
				&coeffs[0].b0, &coeffs[0].b1, &coeffs[0].b2,
				&coeffs[0].a1, &coeffs[0].a2);

			break;
		case HP2:
			stages = 1;
			rbjcompute(f / samplerate, q, 0, 1, 
				&coeffs[0].b0, &coeffs[0].b1, &coeffs[0].b2,
				&coeffs[0].a1, &coeffs[0].a2);
			break;
		case BP2:
			stages = 1;
			rbjcompute(f / samplerate, 1.0 / q, 0, 2, 
				&coeffs[0].b0, &coeffs[0].b1, &coeffs[0].b2,
				&coeffs[0].a1, &coeffs[0].a2);
			break;
		case BS2:
			stages = 1;
			rbjcompute(f / samplerate, 1.0 / q, 0, 3, 
				&coeffs[0].b0, &coeffs[0].b1, &coeffs[0].b2,
				&coeffs[0].a1, &coeffs[0].a2);
			break;
		default:
			return 0.0;
			break;
		}

		float in = sample;

		for(int i = 0; i < stages; i++) {
			float out = (float)(coeffs[i].b0 * in +
				state->state[i].d0);

			state->state[i].d0 = coeffs[i].b1 * in +
				coeffs[i].a1 * out + state->state[i].d1;

			state->state[i].d1 = coeffs[i].b2 * in +
				coeffs[i].a2 * out;

			in = out;
		}

		return in;
	}

	static void rbjcompute(double f, double q, double g, int t,
		double *b0, double *b1, double *b2, double *a1, double *a2)
	{
		double omega = 2.f*3.141592654f*f;
		double sn = (float)sin(omega);
		double cs = (float)cos(omega);
		double alpha;
		double _a0;
		double _a1;
		double _a2;
		double _b0;
		double _b1;
		double _b2;

		double A = sqrt(g);
		double S = sqrt(2.0);
		
		if(t < 2)
			alpha = sn / q;
		else if(t == 2 || t == 3 || t == 5)
			alpha = sn * sinh(q * omega/sn);
		else
			alpha = sn/2.0 * sqrt( (A + 1/A)*(1/S - 1) + 2 );

		switch(t) {
		case 0: // LP
			_b0 =  (1 - cs)/2;
			_b1 =   1 - cs;
			_b2 =  (1 - cs)/2;
			_a0 =   1 + alpha;
			_a1 =  -2*cs;
			_a2 =   1 - alpha;
			break;
		case 1: // HP
			_b0 =  (1 + cs)/2;
			_b1 = -(1 + cs);
			_b2 =  (1 + cs)/2;
			_a0 =   1 + alpha;
			_a1 =  -2*cs;
			_a2 =   1 - alpha;
			break;
		case 2: // BP
			_b0 =   alpha;
			_b1 =   0;
			_b2 =  -alpha;
			_a0 =   1 + alpha;
			_a1 =  -2*cs;
			_a2 =   1 - alpha;
			break;
		case 3: // BR
			_b0 =   1;
			_b1 =  -2*cs;
			_b2 =   1;
			_a0 =   1 + alpha;
			_a1 =  -2*cs;
			_a2 =   1 - alpha;
			break;
		case 4: // HIGH SHELF
			_b0 =    A*( (A+1) + (A-1)*cs + 2*sqrt(A)*alpha );
            _b1 = -2*A*( (A-1) + (A+1)*cs                   );
            _b2 =    A*( (A+1) + (A-1)*cs - 2*sqrt(A)*alpha );
            _a0 =        (A+1) - (A-1)*cs + 2*sqrt(A)*alpha;
            _a1 =    2*( (A-1) - (A+1)*cs                   );
            _a2 =        (A+1) - (A-1)*cs - 2*sqrt(A)*alpha;
		default:
		case 5: // PEAKING
            _b0 =   1 + alpha*A;
            _b1 =  -2*cs;
            _b2 =   1 - alpha*A;
            _a0 =   1 + alpha/A;
            _a1 =  -2*cs;
            _a2 =   1 - alpha/A;
		}

		*b0 = _b0 / _a0;
		*b1 = _b1 / _a0;
		*b2 = _b2 / _a0;
		*a1 = -_a1 / _a0;
		*a2 = -_a2 / _a0;
	}

protected:
	double samplerate;
};

};

#endif

