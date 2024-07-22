#include "timer.h"

u32_t tick;

void timer_callback(registers_t regs)
{
	tick++;
	print("Tick: ");
	print("\n");
}

void init_timer(u32_t freq)
{
	tick = 0;

	irq_reg_handler(0, &timer_callback);
	u32_t divisor = 1193180 / freq;

	port_byte_out(0x43, 0x36);
	port_byte_out(0x40, (u8_t)(divisor & 0xFF));
	port_byte_out(0x40, (u8_t)((divisor >> 8) & 0xFF));
}