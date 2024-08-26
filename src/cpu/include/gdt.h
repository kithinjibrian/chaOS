#ifndef GDT_H
#define GDT_H

#include "type.h"
#include "string.h"
#include "macro.h"

typedef struct gdt_entry
{
	u16_t limit_low;
	u16_t base_low;
	u8_t base_mid;
	u8_t access;
	u8_t granularity;
	u8_t base_high;
} __PACKED__ gdt_entry_t;

typedef struct gdt_ptr
{
	u16_t limit;
	u32_t base;
} __PACKED__ gdt_ptr_t;

typedef struct tss_entry
{
	u32_t backlink;
	u32_t esp0;
	u32_t ss0;
	u32_t esp1;
	u32_t ss1;
	u32_t esp2;
	u32_t ss2;
	u32_t cr3;
	u32_t eip;
	u32_t eflags;
	u32_t eax;
	u32_t ecx;
	u32_t edx;
	u32_t ebx;
	u32_t esp;
	u32_t ebp;
	u32_t esi;
	u32_t edi;
	u32_t es;
	u32_t cs;
	u32_t ss;
	u32_t ds;
	u32_t fs;
	u32_t gs;
	u32_t ldtr;
	u16_t trap;
	u16_t iomap_base;
} __PACKED__ tss_entry_t;

extern u32_t stack_top_g[];

extern void tss_flush(void);
extern void gdt_flush(u32_t);

int init_gdt(void);
void set_kernel_stack(u32_t stack);

#endif