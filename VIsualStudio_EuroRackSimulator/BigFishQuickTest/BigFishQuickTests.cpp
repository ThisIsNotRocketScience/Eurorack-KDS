#include <stdio.h>

#include "../../BigFishLib/BigFish.cpp"
#include "../../BigFishLib/BleppyOscs.cpp"
#include "../../BigFishLib/OrganOsc.cpp"
#include "../../BigFishLib/ADSREnvelope.cpp"
#include "../../BigFishLib/FMOsc.cpp" 


#include <conio.h>

extern int32_t SMMLA(int32_t acc, int32_t a, int32_t b);
extern void AddBlepFixed(int32_t *circbuffer, int index, int32_t scale, uint32_t crosstime);
extern void AddBlepFixedRef(int32_t *circbuffer, int index, int32_t scale, uint32_t crosstime);
extern void AddBlep(float *fcircbuffer, int findex, float fscale, float fcrosstime);

int32_t buffer1[1000] = { 0 };
int32_t buffer2[1000] = { 0 };
float fbuffer[1000] = { 0 };

void PrintPlot2(int V, int V2, int min, int max)
{
	float F = (V - min) / (float)(max - min);
	if (F < 0) F = 0;
	if (F > 1) F = 1;
	float F2 = (V2 - min) / (float)(max - min);
	if (F2 < 0) F2 = 0;
	if (F2 > 1) F2 = 1;
	printf("|");
	for (int i = 0; i < 75; i++)
	{
		float Fl = i / 74.0f;
		bool a = F > Fl;
		bool b = F2 > Fl;
		int R = (a ? 1 : 0) + (b ? 2 : 0);
		switch (R)
		{
		case 0: printf(" "); break;
		case 1: printf("*"); break;
		case 2: printf("o"); break;
		case 3: printf("#"); break;
		}
	}
	printf("|\n");
}

void PrintPlot3(int V, int V2,int V3, int min, int max)
{
	float F = (V - min) / (float)(max - min);
	if (F < 0) F = 0;
	if (F > 1) F = 1;
	float F2 = (V2 - min) / (float)(max - min);
	if (F2 < 0) F2 = 0;
	if (F2 > 1) F2 = 1;
	float F3 = (V3 - min) / (float)(max - min);
	if (F3 < 0) F2 = 0;
	if (F3 > 1) F2 = 1;
	printf("|");
	for (int i = 0; i < 75; i++)
	{
		
		float Fl = i / 74.0f;
		float Fl2 = (i+1) / 74.0f;
		int R = 0;
		if (F > Fl) R += 1;
		if (F > Fl2) R -= 1;
		if (F2 > Fl) R += 2;
		if (F2 > Fl2) R -= 2;
		if (F3 > Fl) R += 4;
		if (F3 > Fl2) R -= 4;

		switch (R)
		{
		case 0: printf(" "); break;
		case 1: printf("."); break;
		case 2: printf("O"); break;
		case 3: printf("0"); break;
		case 4: printf("x"); break;
		case 5: printf("X"); break;
		case 6: printf("*"); break;
		case 7: printf("#"); break;
		}
	}
	printf("|\n");
}


int32_t scaler = 1 << 13;


int main(int argc, char **argv)
{
	printf("Fish size: %d bytes\n\n", sizeof(BigFish_t));
	for (int i = 0; i < 32; i++)
	{
		int mini = 0;
		int maxi = 0;
		float max = -10000;
		float min = 100000;
		for (int j = 0; j < 48; j++)
		{
			if (blep[(i + j * 32) * 2] > max) { max = blep[(i + j * 32) * 2]; maxi = i; };
			if (blep[(i + j * 32) * 2] < min) { min = blep[(i + j * 32) * 2]; mini = i; };
		}
		printf("min: %f (%d), max %f (%d)\n", min, intblep[mini * 2], max, intblep[maxi * 2]);
	}
	printf("\n");

	for (uint64_t ii = 0; ii <= 0xffffffff; ii+=0x10)
	{
		for (int i = 0; i < 48; i++)
		{
			fbuffer[i] = 0;

			buffer1[i] = 0;
			buffer2[i] = 0;
		}

		uint32_t Phase = ii ;
		float fphase = Phase / (65536.0*65536.0f);
		AddBlep(fbuffer, 0, scaler, fphase);
		AddBlepFixed(   buffer1, 0, scaler, Phase);
		AddBlepFixedRef(buffer2, 0, scaler, Phase);
		int dif = 0;
		int rdif = 0;
		for (int i = 0; i < 48; i++)
		{
			float R = fbuffer[i];
			rdif += abs((int)R - buffer1[i]);
			dif += abs(buffer1[i] - buffer2[i]);
		}

		if (rdif > 1000)
		{
			printf("rdif buzz: %d (%d, %x)\n", rdif, ii, Phase);
		}
		if (dif > 100)
		{
	//		printf("dif buzz: %d (%d, %x)\n", dif, ii, Phase);
		}		
	}
	for (int i = 0; i < 48; i++)
	{
		PrintPlot3(fbuffer[i], buffer1[i], buffer2[i], -scaler, scaler);
	}

	{
		for (int i = 0; i < 48; i++)
		{
			fbuffer[i] = 0;

			buffer1[i] = 0;
			buffer2[i] = 0;
		}

		uint32_t Phase =0;
		float fphase = Phase / (65536.0*65535.0f);
		AddBlep(fbuffer, 0, scaler, fphase);
		AddBlepFixed(buffer1, 0, scaler, Phase);
		AddBlepFixedRef(buffer2, 0, scaler, Phase);
		printf("int32_t blepres[49] = {");
		for (int i = 0; i < 48; i++) printf("%d, ", (int)fbuffer[i]);
		printf("0};\n\n");
	}
	
	auto a = SMMLA(0, 0x0fffffff, 0x10000);
	printf("R%d\n", a);

	getch();
}