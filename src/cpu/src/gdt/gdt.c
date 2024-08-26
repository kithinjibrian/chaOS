#include "gdt.h"

tss_entry_t tss_entry_g;

gdt_entry_t gdt_entries_g[6];
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

static void write_tss(u32_t num, u16_t ss0, u32_t esp0)
{
	u32_t base = (u32_t)&tss_entry_g;
	u32_t limit = sizeof(tss_entry_t) + base;

	gdt_set_gate(num, base, limit, 0xE9, 0x00);

	memset(&tss_entry_g, 0, sizeof(tss_entry_t));

	tss_entry_g.ss0 = ss0;
	tss_entry_g.esp0 = esp0;

	tss_entry_g.cs = 0x08 | 0x3;
	tss_entry_g.ss = tss_entry_g.ds = tss_entry_g.es = tss_entry_g.fs = tss_entry_g.gs = 0x10 | 0x3;
	tss_entry_g.iomap_base = sizeof(tss_entry_g);
}

void set_kernel_stack(u32_t stack)
{
	tss_entry_g.esp0 = stack;
}

/**
 * Initialize the Global Descriptor Table
 */
int init_gdt(void)
{
	gdt_ptr_g.limit = (sizeof(gdt_entry_t) * 6) - 1;
	gdt_ptr_g.base = (u32_t)&gdt_entries_g;

	gdt_set_gate(0, 0, 0, 0, 0);

	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
	write_tss(5, 0x10, 0x0);

	gdt_flush((u32_t)&gdt_ptr_g);
	tss_flush();

	return 0;
}