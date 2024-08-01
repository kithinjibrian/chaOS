#include "kernel.h"

multiboot_t *mboot_g;

void main(u32_t magic, multiboot_t *mbt)
{
	panic(magic != 0x2BADB002);
	panic(!(mbt->flags >> 6 & 0x1));

	mboot_g = mbt;

	set_placement_address((u32_t)kernel_pend_g);

	init_gdt();
	init_idt();
	init_page();
	init_pmm();

	do_initcall();

	while (1)
		;
}