#include "gdt.h"

gdt_entry_t gdt_entries_g[5];
gdt_ptr_t gdt_ptr_g;

static void gdt_set_gate(
	int num,
	u32_t base,
	u32_t limit,
	u8_t access,
	u8_t gran)
{
	gdt_entries_g[num].base_low = (base & 0xFFFF);
	gdt_entries_g[num].base_mid = (base >> 16) & 0xFF;
	gdt_entries_g[num].base_high = (base >> 24) & 0xFF;

	gdt_entries_g[num].limit_low = (limit & 0xFFFF);
	gdt_entries_g[num].granularity = (limit >> 16) & 0x0F;

	gdt_entries_g[num].granularity |= gran & 0xF0;
	gdt_entries_g[num].access = access;
}

/**
 * Initialize the Global Descriptor Table
 */
int init_gdt(void)
{
	gdt_ptr_g.limit = (sizeof(gdt_entry_t) * 5) - 1;
	gdt_ptr_g.base = (u32_t)&gdt_entries_g;

	gdt_set_gate(0, 0, 0, 0, 0);

	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

	gdt_flush((u32_t)&gdt_ptr_g);

	return 0;
}