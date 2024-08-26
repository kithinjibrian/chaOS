#ifndef ASSERT_H
#define ASSERT_H

#include "type.h"
#include "vga.h"

void __assert_fail__(string_t, string_t, u32_t, string_t, bool_e);

#define assert(cond) ((cond) ? (void)0 : __assert_fail__(#cond, __FILE__, __LINE__, __func__, FALSE))

#define panic(cond) ((cond) ? (void)0 : __assert_fail__(#cond, __FILE__, __LINE__, __func__, TRUE))

#define halt(message)                       \
	do                                      \
	{                                       \
		print("%s\n", message);             \
		__asm__ __volatile__("cli \n hlt"); \
	} while (0)

#endif