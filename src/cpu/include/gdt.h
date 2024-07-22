#ifndef GDT_H
#define GDT_H

#include "../../libc/include/type.h"

// remove padding from structs between the pragma directives
#pragma pack(push, 1)

typedef struct gdt_entry_t
{
	u16_t limit_low;
	u16_t base_low;
	u8_t base_mid;
	u8_t access;
	u8_t granularity;
	u8_t base_high;
} gdt_entry_t;

typedef struct gdt_ptr_t
{
	u16_t limit;
	u32_t base;
} gdt_ptr_t;

#pragma pack(pop)

extern void gdt_flush(u32_t);

void init_gdt(void);

#endif