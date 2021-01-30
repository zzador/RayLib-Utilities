#include <stdlib.h>

#include "random.h"

void Random_Init(const uint32_t seedA, const uint32_t seedB, Random_t* pRandom)
{
	pRandom->lcg = seedA;
	pRandom->xorshift = seedB;
}

void Random_Init64(const uint64_t seed, Random_t* pRandom)
{
	pRandom->lcg = seed & 0xFFFFFFFF;
	pRandom->xorshift = (seed >> 32) & 0xFFFFFFFF;
}

uint32_t Random_NextInt(Random_t* pRandom)
{
	pRandom->lcg = 1664525 * pRandom->lcg + 1013904223;

	if (pRandom->lcg == 0)
		pRandom->lcg = pRandom->xorshift ^ 1234567891;

	pRandom->xorshift ^= pRandom->xorshift << 13;
	pRandom->xorshift ^= pRandom->xorshift >> 17;
	pRandom->xorshift ^= pRandom->xorshift << 5;

	if (pRandom->xorshift == 0)
		pRandom->xorshift = pRandom->lcg ^ 1987654321;

	return (pRandom->lcg ^ pRandom->xorshift);
}

float Random_NextFloat(Random_t* pRandom)
{
	return (float) Random_NextDouble(pRandom);
}

double Random_NextDouble(Random_t* pRandom)
{
	return (double) (Random_NextInt(pRandom) / 4294967295.0);
}
