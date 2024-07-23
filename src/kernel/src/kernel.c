#include "kernel.h"

void panic(const char *msg)
{
	print((char *)msg);
}

void main(void)
{
	init_gdt();
	init_idt();
	irq_reg_handler(1, &keyboard_handler);
	while (1)
		;
}