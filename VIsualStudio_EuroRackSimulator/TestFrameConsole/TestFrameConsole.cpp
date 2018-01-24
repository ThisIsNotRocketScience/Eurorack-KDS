#include "../../WobblerV2/Sources/Wobbler2.h"
#include <stdio.h>
#include <math.h>


unsigned long FreqLerpb[17] = {
	53687, // 0,1 -> 107374000
	85088, // 0,15848936110389 -> 170176000
	134855, // 0,251188775831191 -> 269710000
	213732, // 0,398107485979536 -> 427464000
	338743, // 0,630958011035723 -> 677486000
	536871, // 1,00000132052433 -> 1073742000
	850884, // 1,58489570392946 -> 1701768000
	1348561, // 2,5118910753208 -> 2697122000
	2137326, // 3,98108011690155 -> 4274652000
	3387434, // 6,30958844231125 -> 2479900704
	5368723, // 10,0000264105039 -> 2147511408
	5368723, // 10,0000264105039 -> 2147511408
	5368723, // 10,0000264105039 -> 2147511408
	5368723, // 10,0000264105039 -> 2147511408
	5368723, // 10,0000264105039 -> 2147511408
	5368723, // 10,0000264105039 -> 2147511408
	5368723 // 10,0000264105039 -> 2147511408
};

// 4 bit table
// 9 bit input

unsigned long LERP16_t(unsigned long *V, int fade)
{
	unsigned char frac = (fade<<3) & 0xff;
	//if (frac && (frac < 255)) frac += 1;
	int I = fade >> 5;
	return ((V[I] >> 8) *(256 - frac) + (V[I + 1] >> 8) * frac);
}
unsigned long TestF(int inp)
{
	return LERP16_t(FreqLerpb, inp);

}
int main(int argc, char **argv)
{
	Wobbler2_LFO_t LFO2Static;
	Wobbler2_Settings LFO2Settings;
	Wobbler2_LFO_t LFO2Running;
	Wobbler2_Params LFO2Params;
	for (int i = 0; i < 17; i++)
	{
		uint32_t V = 0xffffffff;
		V /= 2500; // 1hZ
		double F = (0.1 * pow(2.0, (((double)i / (double)(15)) * 6.64386)));

		uint32_t V2 = V * F;

		FreqLerpb[i] = V2;
	}

	Wobbler2_Init(&LFO2Static);

	for (int i = 0; i < 512; i++ )
	{
		unsigned long DPold = Wobbler2_LFORange2(i, 0);
		unsigned long DPnew = Wobbler2_MakeFreq(i);
		unsigned long DPtest = TestF(i);
		printf("%d: \told %d \tnew %d\ttest %d\n", i, DPold, DPnew, DPtest);

	};

	FILE *F = fopen("FreqLerp.h", "wb+");
	int Len = 17;
	fprintf(F, "#define FREQLERPLEN %d\n\n", Len);
	fprintf(F, "unsigned long FreqLerp[FREQLERPLEN] = {");

	for (int i = 0; i < Len; i++)
	{
		fprintf(F, "0x%x", FreqLerpb[i]);
		if (i < Len - 1)fprintf(F, ", ");
	};

	fprintf(F, "}\n\n");
	fclose(F);
	char R [2];
	gets_s(R,2 );
}