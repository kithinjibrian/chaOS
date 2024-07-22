#include "kernel.h"

void main(void)
{
	init_gdt();
	init_idt();
	while (1)
		;
}