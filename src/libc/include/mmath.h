#ifndef MATH_H
#define MATH_H

#include "type.h"

#define CEIL(a, b) ((a + b - 1) / b)
#define IS_POWER_OF_2(x) ((x != 0) && ((x & (x - 1)) == 0))

#endif