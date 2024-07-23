#ifndef IDT_H
#define IDT_H

#include "../../libc/include/type.h"
#include "../../drivers/include/vga.h"
#include "../../libc/include/mstring.h"
#include "../../drivers/include/port.h"

// remove padding from structs between the pragma directives
#pragma pack(push, 1)

typedef struct idt_entry_t
{
	u16_t base_lo;
	u16_t sel;
	u8_t always0;
	u8_t flags;
	u16_t base_hi;
} idt_entry_t;

typedef struct idt_ptr_t
{
	u16_t limit;
	u32_t base;
} idt_ptr_t;

#pragma pack(pop)

typedef void (*fun_handler_t)(registers_t);

extern void idt_flush(u32_t);

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void init_idt(void);
void isr_handler(registers_t regs);
void irq_handler(registers_t regs);

void irq_unreg_handler(int irq);
void irq_reg_handler(int irq, fun_handler_t handler);

#endif