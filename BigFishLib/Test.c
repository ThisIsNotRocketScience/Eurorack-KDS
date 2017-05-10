#include "BigFish.h"

void Test()
{
	BigFish_t T;
	BigFish_Init(&T);

	int32_t  buffer[256];
	for (int i = 0; i < 100; i++)
	{
		BigFish_Update(&T);
		BigFish_GenerateBlock(&T, buffer, 256);
	}
}