#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>

typedef struct Random_s Random_t;

struct Random_s
{
	uint32_t lcg, xorshift;
};

extern void Random_Init(const uint32_t seedA, const uint32_t seedB, Random_t* pRandom);
extern void Random_Init64(const uint64_t seed, Random_t* pRandom);

extern uint32_t Random_NextInt(Random_t* pRandom);
extern float Random_NextFloat(Random_t* pRandom);
extern double Random_NextDouble(Random_t* pRandom);

#endif
