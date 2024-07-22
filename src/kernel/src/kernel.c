#include "kernel.h"

void panic(const char *msg)
{
	print((char *)msg);
}

void main(void)
{
	init_gdt();
	init_idt();
	init_timer(100);
}