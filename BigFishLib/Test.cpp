#include "BigFish.h"

void Test()
{
	BigFish_t T;
	BigFish_Init(&T, 44100);

	int32_t  buffermain[256];
	int32_t  bufferosc[256];
	int32_t  input[256];
	for (int i = 0; i < 100; i++)
	{
		BigFish_Update(&T);
		BigFish_GenerateBlock(&T, input, bufferosc, buffermain, 1048);
	}
}