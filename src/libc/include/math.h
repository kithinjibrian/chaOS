#ifndef MATH_H
#define MATH_H

#include "type.h"

#define CEIL(a, b) ((a + b - 1) / b)
#define IS_POWER_OF_2(x) ((x != 0) && ((x & (x - 1)) == 0))

static inline int round_up(size_t num, size_t target)
{
	if (num <= 0)
	{
		return -1; // Return an error code for invalid multiple
	}

	int remainder = num % target;

	if (remainder == 0)
	{
		return num; // No rounding needed, it's already a multiple of `multiple`
	}

	return num + (target - remainder);
}

#endif