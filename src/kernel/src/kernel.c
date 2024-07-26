#include "kernel.h"

void panic(const char *msg)
{
	print((char *)msg);
}

void check_paging()
{
	unsigned int cr0;
	asm volatile("mov %%cr0, %0" : "=r"(cr0));
	if (!(cr0 & 0x80000000))
	{
		panic("Paging not enabled\n");
	}
}

void main(u32_t magic, multiboot_t *mbt)
{

	if (magic != 0x2BADB002)
	{
		panic("Invalid magic number\n");
	}

	if (!(mbt->flags >> 6 & 0x1))
	{
		panic("Memory map not present\n");
	}

	check_paging();

	set_placement_address_g((u32_t)kernel_vend);

	init_gdt();
	init_idt();
	init_timer(100);
	irq_reg_handler(1, &keyboard_handler);

	u32_t mem_high = mbt->mem_upper * 1024;
	init_pmm(mem_high);

	for (int a = 0; a < mbt->mmap_length; a += sizeof(multiboot_mmap_entry_t))
	{
		multiboot_mmap_entry_t *entry = (multiboot_mmap_entry_t *)((mbt->mmap_addr + KERNEL_VBASE) + a);

		/**
		 * Skip BIOS memory that starts at 0x0
		 */
		if (entry->addr_low)
		{
			if (entry->type == 1)
			{
				/**
				 * In memory where our kernel is
				 */
				if (entry->addr_low == (u32_t)kernel_pstart)
				{
					print("addr: %x, len: %d, type: %d\n",
						  entry->addr_low,
						  entry->len_low,
						  entry->type);
					/**
					 * Skip the kernel, page directory and the bitmap that is placed adjacent to it
					 * -0xc0000000 is the offset of the kernel in physical memory
					 */
					pmm_init_region(placement_address_g - KERNEL_VBASE, entry->len_low);
				}
			}
		}
	}

	init_page();

	while (1)
		;
}