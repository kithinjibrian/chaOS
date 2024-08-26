#include "kernel.h"

logo(chaos_logo);

multiboot_t *mboot_g;

void kernel_main(u32_t magic, multiboot_t *mbt)
{
	panic(magic == 0x2BADB002);
	panic(mbt->flags >> 6 & 0x1);
	print("%s\n\n", chaos_logo);

	mboot_g = mbt;

	init_gdt();
	init_idt();
	init_pmm();
	init_page();
	init_syscall();
	do_initcall();

	while (1)
		;
}