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

#include <stdio.h>

#include <stdint.h>

int main(int argc, char **Argv)
{
	float *blep = GenerateMinBLEP(24, 32);
	for (int i = 0; i < 48 * 32; i++)
	{
		blep[i] = 1.0f - blep[i];
	}
	{
		FILE *A = fopen("bleptable.h", "w+");
		if (A)
		{
			fprintf(A, "const float blep[48*32*2] = {\n\t");
			for (int i = 0; i < 48 * 32; i++)
			{
				fprintf(A, "%ff, %ff,", blep[i], blep[i + 1] - blep[i]);
				if (i % 16 == 15) fprintf(A, "\n\t");
			}
			fprintf(A, "};\n");
			fclose(A);
		}
	}
	{
		FILE *A = fopen("intbleptable.h", "w+");
		if (A)
		{
			fprintf(A, "const int32_t intblep[48*32*2] = {\n\t");
			for (int i = 0; i < 48 * 32; i++)
			{
				int32_t intblepA = (int32_t)(blep[i] * (float)(1 << 15));
				int32_t intblepB = (int32_t)(blep[i+1] * (float)(1 << 15));
				int32_t intblepDelta = intblepB - intblepA;
				fprintf(A, "%d, %d,", intblepA, intblepDelta);
				if (i % 16 == 15) fprintf(A, "\n\t");
			}
			fprintf(A, "};\n");
			fclose(A);
		}
	}


#define SINETABLE_BITS	8
#define SINETABLE_SIZE	(1 << SINETABLE_BITS)
	

	FILE *B = fopen("sintable.h", "w+");
	if (B)
	{
		fprintf(B, "#define SINETABLE_BITS %d\n", SINETABLE_BITS);
		fprintf(B, "#define SINETABLE_SIZE	(1 << SINETABLE_BITS)\n");
		fprintf(B, "int16_t const SineTable[2 * SINETABLE_SIZE + 2] = {\n\t");
		for (int i = 0; i < SINETABLE_SIZE + 1; i++)
		{
			float P = (i * PI * 2.0) / (float)(SINETABLE_SIZE);
			float P2 = ((i+1) * PI * 2.0) / (float)(SINETABLE_SIZE);
			fprintf(B, "%d,%d, ", (int)(sinf(P)*32767.0), (int)(sinf(P)*32767.0 - sinf(P)*32767.0));
			if (i % 16 == 15) fprintf(B, "\n\t");
		}
		fprintf(B, "};\n");
		fclose(B);
	}

	FILE *C = fopen("freqtable.h", "w+");
	if (C)
	{
		
		
		fprintf(C, "float const PowTab[129*2] = {\n\t");
		float P = 0;
		float P2 = 0;
		for (int i = 0; i < 128 + 1; i++)
		{
			P = powf(2.0, (i - 69) / 12.0f);
			P2 = powf(2.0, ((i+1) - 69) / 12.0f);

			fprintf(C, "%f,%f, ", P, P2-P);
			if (i % 16 == 15) fprintf(C, "\n\t");
		}
		fprintf(C, "%f, %f};\n", P,P2-P);
		fclose(C);
	}


	for (int i = 0; i < SINETABLE_SIZE + 1; i++)
	{
		
	}
	return 0;
}