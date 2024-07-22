#include "kernel.h"

void main(void)
{
	init_gdt();
	init_idt();
	init_timer(100);
}