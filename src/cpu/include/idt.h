#ifndef IDT_H
#define IDT_H

#include "vga.h"
#include "type.h"
#include "port.h"
#include "macro.h"
#include "string.h"
#include "kmalloc.h"

#define NUM_SYSCALLS 3

#define SYSCALL 128

#define PIC1 0x20
#define PIC2 0xA0

#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)

#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

#define PIC_EOI 0x20

#define IRQ0 32
#define IRQ_TIMER 0
#define IRQ_KEYBOARD 1

#define NO_IRQs 16

/**
 * Enables interrupts
 */
static inline void sti()
{
	__asm__ __volatile__("sti");
}

/**
 * Disables interrupts
 */
static inline void cli()
{
	__asm__ __volatile__("cli");
}

typedef void (*fun_handler)(registers_t *);

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

typedef struct irqaction
{
	u32_t irq;
	u32_t flags;
	struct irq *next;
	const char *name;
	fun_handler handler;
} irqaction_t;

typedef struct irq_desc
{
	const char *name;
	struct irqaction *action;
} irq_desc_t;

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

int init_idt(void);

void trap(registers_t *regs);

void isr_unreg_handler(int isr);
void register_isr_handler(int isr, fun_handler handler);

void irq_unreg_handler(int irq);
void register_irq_handler(int irq, fun_handler handler);

void register_irq(u32_t irq, fun_handler handler, u32_t flags, const char *name);

void syscall_handler(registers_t *regs);

#endif