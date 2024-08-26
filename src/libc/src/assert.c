#include "assert.h"

void __assert_fail__(string_t expr, string_t file, u32_t line, string_t func, bool_e panic)
{
	if (panic)
	{
		print("\nThe kernel panicked!\n");
	}
	else
	{
		print("\nAssertion failed!\n");
	}

	print("Expression: %s\n"
		  "File: %s\n"
		  "Line: %d\n"
		  "Function: %s\n",
		  expr, file, line, func);

	if (panic)
	{
		__asm__ __volatile__("cli \n hlt");
	}
}