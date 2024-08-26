#include "idt.h"

idt_entry_t idt_g[256];
idt_ptr_t idt_ptr_g;

const char *exception_messages[] = {
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

fun_handler isr_callbacks[32] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0};

fun_handler irq_callbacks[16] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0};

irq_desc_t irq_descs[NO_IRQs] = {0};

static void set_idt_gate(int num, u32_t base, u16_t kernel_cs, u8_t flags)
{
	idt_g[num].base_lo = base & 0xFFFF;
	idt_g[num].base_hi = (base >> 16) & 0xFFFF;
	idt_g[num].kernel_cs = kernel_cs;
	idt_g[num].reserved = 0;
	idt_g[num].attributes = flags | 0x60;
}

void irq_remap()
{
	/**
	 * Remap master and slave PICs
	 * Master: 0x20, data: 0x21
	 * Slave: 0xA0, data: 0xA1
	 */
	port_byte_out(PIC1_COMMAND, 0x11);
	port_byte_out(PIC2_COMMAND, 0x11);

	port_byte_out(PIC1_DATA, 0x20);
	port_byte_out(PIC2_DATA, 0x28);

	port_byte_out(PIC1_DATA, 0x04);
	port_byte_out(PIC2_DATA, 0x02);

	port_byte_out(PIC1_DATA, 0x01);
	port_byte_out(PIC2_DATA, 0x01);

	port_byte_out(PIC1_DATA, 0x0);
	port_byte_out(PIC2_DATA, 0x0);
}

/**
 * Initialize the idt_g
 */
int init_idt(void)
{
	idt_ptr_g.limit = (sizeof(idt_entry_t) * 256) - 1;
	idt_ptr_g.base = (u32_t)&idt_g;

	memset(&idt_g, 0, sizeof(idt_entry_t) * 256);

	irq_remap();

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

	set_idt_gate(SYSCALL, (u32_t)syscall_handler, 0x08, 0x8E);

	idt_flush((u32_t)&idt_ptr_g);

	sti();

	return 0;
}

void register_isr_handler(int isr, fun_handler handler)
{
	isr_callbacks[isr] = handler;
}

void isr_unreg_handler(int isr)
{
	isr_callbacks[isr] = 0;
}

/**
 * register interrupt handler
 * @param irq interrupt number
 * @param handler interrupt handler
 */
void register_irq_handler(int irq, fun_handler handler)
{
	irq_callbacks[irq] = handler;
}

void register_irq(u32_t irq, fun_handler handler, u32_t flags, const char *name)
{
	irqaction_t *irqaction = (irqaction_t *)kmalloc(sizeof(irqaction_t));
	irqaction->irq = irq;
	irqaction->flags = flags;
	irqaction->handler = handler;
	irqaction->name = name;
}

void irq_unreg_handler(int irq)
{
	irq_callbacks[irq] = 0;
}

void pic_send_eoi(int int_no)
{
	if (int_no >= 32 && int_no < 48)
	{
		if (int_no >= 40)
		{
			port_byte_out(PIC2_COMMAND, PIC_EOI);
		}

		port_byte_out(PIC1_COMMAND, PIC_EOI);
	}
}

/**
 * handle interrupt request
 * @param regs registers
 */
void trap(registers_t *regs)
{

	if (regs->int_no < 32)
	{
		if (isr_callbacks[regs->int_no])
		{
			isr_callbacks[regs->int_no](regs);
		}
		else
		{
			print("Interrupt: %s\n", exception_messages[regs->int_no]);
		}
	}
	else if (regs->int_no >= IRQ0 && regs->int_no < 48)
	{
		pic_send_eoi(regs->int_no);

		if (irq_callbacks[regs->int_no - IRQ0])
		{
			irq_callbacks[regs->int_no - IRQ0](regs);
		}
		else
		{
			goto default_trap;
		}
	}
	else if (regs->int_no == SYSCALL)
	{
		syscall_handler(regs);
	}

default_trap:
	return;
}
