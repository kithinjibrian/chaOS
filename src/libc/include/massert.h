#ifndef ASSERT_H
#define ASSERT_H

#include "type.h"
#include "vga.h"

void __assert_fail__(string_t, string_t, u32_t, string_t, bool_e);

#define assert(expr)                                                     \
	do                                                                   \
	{                                                                    \
		if ((expr))                                                      \
		{                                                                \
			__assert_fail__(#expr, __FILE__, __LINE__, __func__, FALSE); \
		}                                                                \
	} while (0)

#define panic(expr)                                                     \
	do                                                                  \
	{                                                                   \
		if ((expr))                                                     \
		{                                                               \
			__assert_fail__(#expr, __FILE__, __LINE__, __func__, TRUE); \
		}                                                               \
	} while (0)

#define halt(message)               \
	do                              \
	{                               \
		print("%s\n", message);     \
		asm volatile("cli \n hlt"); \
	} while (0)

#endif