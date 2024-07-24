#include "timer.h"

u32_t tick = 0;

static void timer_callback(registers_t regs)
{
	tick++;
}

/**
 * Initialize the timer
 * @param freq The frequency of the timer
 */
void init_timer(u32_t freq)
{
	tick = 0;

	irq_reg_handler(0, &timer_callback);
	u32_t divisor = 1193180 / freq;

	/* Send command to start timer */
	port_byte_out(0x43, 0x36);

	/* Send the divisor byte-wise by splitting it in upper and lower bytes */
	port_byte_out(0x40, (u8_t)(divisor & 0xFF));
	port_byte_out(0x40, (u8_t)((divisor >> 8) & 0xFF));
}