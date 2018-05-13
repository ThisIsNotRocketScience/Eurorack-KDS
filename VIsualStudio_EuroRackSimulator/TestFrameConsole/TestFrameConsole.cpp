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
	unsigned char frac = (fade << 3) & 0xff;
	//if (frac && (frac < 255)) frac += 1;
	int I = fade >> 5;
	return ((V[I] >> 8) *(256 - frac) + (V[I + 1] >> 8) * frac);
}
unsigned long TestF(int inp)
{
	return LERP16_t(FreqLerpb, inp);

}

enum
{
	ADC_MODULATION,
	ADC_PHASING,
	ADC_SHAPE,
	ADC_SPEED,
	ADC_AMTNORMAL,
	ADC_AMTPHASED,
	ADC_Count
};

int main(int argc, char **argv)
{
	Wobbler2_LFO_t LFO2Static;
	Wobbler2_Settings LFO2Settings;
	Wobbler2_Params LFO2Params;

	Wobbler2_LoadSettings(&LFO2Settings, &LFO2Params);
	uint16_t adcchannels[ADC_Count];
	adcchannels[ADC_MODULATION] = 0x8000;
	adcchannels[ADC_PHASING] = 0;
	adcchannels[ADC_SHAPE] = 0x8000;
	adcchannels[ADC_SPEED] = 0x8000;
	adcchannels[ADC_MODULATION] = 0x8000;
	adcchannels[ADC_AMTNORMAL] = 0x8000;
	adcchannels[ADC_AMTPHASED] = 0x8000;

	

	Wobbler2_Init(&LFO2Static);

	
	LFO2Static.Mod = ~adcchannels[ADC_MODULATION];
	LFO2Static.Shape = ~adcchannels[ADC_SHAPE];
	LFO2Static.Phasing = (adcchannels[ADC_PHASING]) >> 4;
	LFO2Static.Speed = ((0xffff - adcchannels[ADC_SPEED]) >> 7);
	LFO2Static.SpeedOrig = ((0xffff - adcchannels[ADC_SPEED]));
	LFO2Static.Amount1 = ((adcchannels[ADC_AMTNORMAL]) >> 1) - (1 << 14);
	LFO2Static.Amount2 = ((adcchannels[ADC_AMTPHASED]) >> 1) - (1 << 14);
	
	
	for (int i = 0; i < 50000; i++)
	{
		if (i % 1000 == 0)
		{
			Wobbler2_SyncPulse(&LFO2Static);
		}
		Wobbler2_Get(&LFO2Static, &LFO2Params);
	}

	/*

	if (0)
	{

		for (int i = 0; i < 17; i++)
		{
			uint32_t V = 0xffffffff;
			V /= 2500; // 1hZ
			double F = (0.1 * pow(2.0, (((double)i / (double)(15)) * 6.64386)));

			uint32_t V2 = V * F;

			FreqLerpb[i] = V2;
		}


		for (int i = 0; i < 512; i++)
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
	}
	*/
	{
		float sintab[2048];
		float dsintab[2048];
		int sr = 44100;
		float mindelay = sr * 0.018f;
		float chorusdepth = sr * 0.003f;
		int mindelayafterprocess = (int)((chorusdepth + mindelay) * 2);
		for (int i = 0; i < 2048; i++)
		{
			sintab[i] = mindelay + (float)sin((float)i*6.283f / 2048.f)*chorusdepth;
		}
		for (int i = 0; i < 2048; i++)
		{
			dsintab[i] = sintab[(i + 1) & 2047] - sintab[i];
		}
		FILE *F = fopen("SinTabs.h", "wb+");
		
		fprintf(F, "float const sintab[2048] = {");
		int Len = 2048;
		for (int i = 0; i < Len; i++)
		{
			fprintf(F, "%f", sintab[i]);
			if (i < Len - 1)fprintf(F, ", ");
		};

		fprintf(F, "}\n\n");
		fprintf(F, "float const dsintab[2048] = {");
		
		for (int i = 0; i < Len; i++)
		{
			fprintf(F, "%f", dsintab[i]);
			if (i < Len - 1) fprintf(F, ", ");
		};

		fprintf(F, "}\n\n");
		fclose(F);
	}
	
	printf("done..\n");
	char R[2];
	gets_s(R, 2);




}