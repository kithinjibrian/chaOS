#include "idt.h"

idt_entry_t idt_g[256];
idt_ptr_t idt_ptr_g;

static void set_idt_gate(int num, u32_t base, u16_t sel, u8_t flags)
{
	idt_g[num].base_lo = base & 0xFFFF;
	idt_g[num].base_hi = (base >> 16) & 0xFFFF;
	idt_g[num].sel = sel;
	idt_g[num].always0 = 0;
	idt_g[num].flags = flags | 0x60;
}

/**
 * Initialize the idt_g
 */
void init_idt(void)
{
	idt_ptr_g.limit = (sizeof(idt_entry_t) * 256) - 1;
	idt_ptr_g.base = (u32_t)&idt_g;

	memset(&idt_g, 0, sizeof(idt_entry_t) * 256);

	/**
	 * Remap master and slave PICs
	 * Master: 0x20, data: 0x21
	 * Slave: 0xA0, data: 0xA1
	 */
	port_byte_out(0x20, 0x11);
	port_byte_out(0xA0, 0x11);

	port_byte_out(0x21, 0x20);
	port_byte_out(0xA1, 0x28);

	port_byte_out(0x21, 0x04);
	port_byte_out(0xA1, 0x02);

	port_byte_out(0x21, 0x01);
	port_byte_out(0xA1, 0x01);

	port_byte_out(0x21, 0x0);
	port_byte_out(0xA1, 0x0);

	set_idt_gate(0, (u32_t)isr0, 0x08, 0x8E);
	set_idt_gate(1, (u32_t)isr1, 0x08, 0x8E);
	set_idt_gate(2, (u32_t)isr2, 0x08, 0x8E);
	set_idt_gate(3, (u32_t)isr3, 0x08, 0x8E);
	set_idt_gate(4, (u32_t)isr4, 0x08, 0x8E);
	set_idt_gate(5, (u32_t)isr5, 0x08, 0x8E);
	set_idt_gate(6, (u32_t)isr6, 0x08, 0x8E);
	set_idt_gate(7, (u32_t)isr7, 0x08, 0x8E);
	set_idt_gate(8, (u32_t)isr8, 0x08, 0x8E);
	set_idt_gate(9, (u32_t)isr9, 0x08, 0x8E);
	set_idt_gate(10, (u32_t)isr10, 0x08, 0x8E);
	set_idt_gate(11, (u32_t)isr11, 0x08, 0x8E);
	set_idt_gate(12, (u32_t)isr12, 0x08, 0x8E);
	set_idt_gate(13, (u32_t)isr13, 0x08, 0x8E);
	set_idt_gate(14, (u32_t)isr14, 0x08, 0x8E);
	set_idt_gate(15, (u32_t)isr15, 0x08, 0x8E);
	set_idt_gate(16, (u32_t)isr16, 0x08, 0x8E);
	set_idt_gate(17, (u32_t)isr17, 0x08, 0x8E);
	set_idt_gate(18, (u32_t)isr18, 0x08, 0x8E);
	set_idt_gate(19, (u32_t)isr19, 0x08, 0x8E);
	set_idt_gate(20, (u32_t)isr20, 0x08, 0x8E);
	set_idt_gate(21, (u32_t)isr21, 0x08, 0x8E);
	set_idt_gate(22, (u32_t)isr22, 0x08, 0x8E);
	set_idt_gate(23, (u32_t)isr23, 0x08, 0x8E);
	set_idt_gate(24, (u32_t)isr24, 0x08, 0x8E);
	set_idt_gate(25, (u32_t)isr25, 0x08, 0x8E);
	set_idt_gate(26, (u32_t)isr26, 0x08, 0x8E);
	set_idt_gate(27, (u32_t)isr27, 0x08, 0x8E);
	set_idt_gate(28, (u32_t)isr28, 0x08, 0x8E);
	set_idt_gate(29, (u32_t)isr29, 0x08, 0x8E);
	set_idt_gate(30, (u32_t)isr30, 0x08, 0x8E);
	set_idt_gate(31, (u32_t)isr31, 0x08, 0x8E);

	set_idt_gate(32, (u32_t)irq0, 0x08, 0x8E);
	set_idt_gate(33, (u32_t)irq1, 0x08, 0x8E);
	set_idt_gate(34, (u32_t)irq2, 0x08, 0x8E);
	set_idt_gate(35, (u32_t)irq3, 0x08, 0x8E);
	set_idt_gate(36, (u32_t)irq4, 0x08, 0x8E);
	set_idt_gate(37, (u32_t)irq5, 0x08, 0x8E);
	set_idt_gate(38, (u32_t)irq6, 0x08, 0x8E);
	set_idt_gate(39, (u32_t)irq7, 0x08, 0x8E);
	set_idt_gate(40, (u32_t)irq8, 0x08, 0x8E);
	set_idt_gate(41, (u32_t)irq9, 0x08, 0x8E);
	set_idt_gate(42, (u32_t)irq10, 0x08, 0x8E);
	set_idt_gate(43, (u32_t)irq11, 0x08, 0x8E);
	set_idt_gate(44, (u32_t)irq12, 0x08, 0x8E);
	set_idt_gate(45, (u32_t)irq13, 0x08, 0x8E);
	set_idt_gate(46, (u32_t)irq14, 0x08, 0x8E);
	set_idt_gate(47, (u32_t)irq15, 0x08, 0x8E);

	idt_flush((u32_t)&idt_ptr_g);
}

unsigned char *exception_messages[] = {
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Overflow",
	"Bound Range Exceeded",
	"Invalid Opcode",
	"Device Not Available",

	"Double Fault",
	"Coprocessor Segment Overrun",
	"Invalid TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",

	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
};

/**
 * handle interrupt request
 * @param regs registers
 */
void isr_handler(registers_t regs)
{
	if (regs.int_no < 32)
	{
		print(exception_messages[regs.int_no]);
		while (1)
			;
	}
}

fun_handler_t ih[16] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0};

/**
 * register interrupt handler
 * @param irq interrupt number
 * @param handler interrupt handler
 */
void irq_reg_handler(int irq, fun_handler_t handler)
{
	ih[irq] = handler;
}

void irq_unreg_handler(int irq)
{
	ih[irq] = 0;
}

/**
 * handle interrupt request
 * @param regs registers
 */
void irq_handler(registers_t regs)
{
	/**
	 *	resize interrupt number so that it is in the range 0-15
	 *	i.e The timer interrupt is 32 but becomes 0
	 */
	fun_handler_t handler = ih[regs.int_no - 32];

	if (handler)
	{
		handler(regs);
	}

	/**
	 * send EOI (end of interrupt) to interrupt controller
	 */
	if (regs.int_no >= 40)
	{
		/**
		 * send reset signal to slave
		 */
		port_byte_out(0xA0, 0x20);
	}

	/**
	 * send reset signal to master
	 */
	port_byte_out(0x20, 0x20);
}
