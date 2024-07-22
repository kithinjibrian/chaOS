#include "gdt.h"

gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;

static void gdt_set_gate(
	int num,
	u32_t base,
	u32_t limit,
	u8_t access,
	u8_t gran)
{
	gdt_entries[num].base_low = (base & 0xFFFF);
	gdt_entries[num].base_mid = (base >> 16) & 0xFF;
	gdt_entries[num].base_high = (base >> 24) & 0xFF;

	gdt_entries[num].limit_low = (limit & 0xFFFF);
	gdt_entries[num].granularity = (limit >> 16) & 0x0F;

	gdt_entries[num].granularity |= gran & 0xF0;
	gdt_entries[num].access = access;
}

void init_gdt(void)
{
	gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
	gdt_ptr.base = (u32_t)&gdt_entries;

	gdt_set_gate(0, 0, 0, 0, 0);

	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

	gdt_flush((u32_t)&gdt_ptr);
}