#ifndef RAND_H
#define RAND_H

#include "type.h"

static u32_t rand_seed = 1;

static inline u32_t rand(void)
{
	rand_seed = rand_seed * 1103515245 + 12345;
	return (rand_seed >> 16) & 0x7FFF;
}

static inline void srand(u32_t seed)
{
	rand_seed = seed;
}

#endif