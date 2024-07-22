#include "kernel.h"

void panic(const char* msg)
{
	print(msg);
}


void main(void)
{
	clear();
	print("Brian Kithinji");
	while (1)
		;
}