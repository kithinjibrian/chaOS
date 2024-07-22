#include "kernel.h"

void panic(const char* msg)
{
	print(msg);
}


void main(void)
{
	init_gdt();
	init_idt();
	init_timer(100);
}