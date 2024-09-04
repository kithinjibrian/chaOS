#ifndef IDT_H
#define IDT_H

#include "io.h"
#include "type.h"
#include "macro.h"
#include "module.h"
#include "string.h"
#include "idtentry.h"

#define IDT_ENTRIES 256
#define IDT_TABLE_SIZE (IDT_ENTRIES * sizeof(struct gate_desc))

#define GDT_ENTRY_KERNEL_CS 1 // 12
#define KERNEL_CS (GDT_ENTRY_KERNEL_CS * 8)

#define DPL0 0x0
#define DPL3 0x3

#define G(_vector, _addr, _type, _dpl, _segment) \
	{                                            \
		.bits.p = 1,                             \
		.bits.ist = 0,                           \
		.bits.zero = 0,                          \
		.bits.dpl = _dpl,                        \
		.bits.type = _type,                      \
		.addr = _addr,                           \
		.vector = _vector,                       \
		.segment = _segment,                     \
	}

/* Interrupt gate */
#define INTG(vector, addr) \
	G(vector, addr, GATE_INTERRUPT, DPL0, 0x08)

/* System interrupt gate */
#define SYSG(vector, addr) \
	G(vector, addr, GATE_INTERRUPT, DPL3, KERNEL_CS)

typedef void (*fun_handler)(registers_t *);

enum gate_types
{
	GATE_INTERRUPT = 0xE, /* 32-bit interrupt gate */
	GATE_TRAP = 0xF,	  /* 32-bit trap gate */
	GATE_CALL = 0xC,
	GATE_TASK = 0x5, /* Task gate */
};

typedef struct idt_bits
{
	u16_t ist : 3, /* unused in x86 32*/
		zero : 5,  /* unused in x86 32*/
		type : 5,  /* Defines the type of interrupt gate */
		dpl : 2,   /*defines the CPU Privilege Levels*/
		p : 1;	   /* Present bit */
} __PACKED__ idt_bits_t;

typedef struct idt_data
{
	void_fun addr;
	unsigned int vector;
	unsigned int segment;
	struct idt_bits bits;
} idt_data_t;

typedef struct gate_desc
{
	u16_t base_lo;
	u16_t segment;
	struct idt_bits bits;
	u16_t base_hi;
} __PACKED__ gate_desc_t;

typedef struct idt_entry
{
	u16_t base_lo;
	u16_t kernel_cs;
	u8_t reserved;
	u8_t attributes;
	u16_t base_hi;
} __PACKED__ idt_entry_t;

typedef struct idt_ptr
{
	u16_t limit;
	u32_t base;
} __PACKED__ idt_ptr_t;


static __INLINE__ void native_load_idt(const struct idt_ptr *iptr)
{
	__asm__ __volatile__("lidtl %0" ::"m"(*iptr));
}
#define load_idt(dtr) native_load_idt(dtr)

static __INLINE__ void
native_write_idt_entry(struct gate_desc *idt, int entry, struct gate_desc *gate)
{
	memcpy(&idt[entry], gate, sizeof(*gate));
}

#define write_idt_entry(dt, entry, g) native_write_idt_entry(dt, entry, g)

void idt_setup_early_traps(void);

#endif
