#include "timer.h"

u32_t tick_g = 0;

static void timer_callback(registers_t regs)
{
	tick_g++;
}

/**
 * Initialize the timer
 * @param freq The frequency of the timer
 */
int __INIT__ init_timer(void)
{
	tick_g = 0;

	irq_reg_handler(0, &timer_callback);
	u32_t divisor = 1193180 / 100;

	/* Send command to start timer */
	port_byte_out(0x43, 0x36);

	/* Send the divisor byte-wise by splitting it in upper and lower bytes */
	port_byte_out(0x40, (u8_t)(divisor & 0xFF));
	port_byte_out(0x40, (u8_t)((divisor >> 8) & 0xFF));

	return 0;
}

core_initcall(init_timer);
module_author("Kithinji Brian");
module_description("Simple timer.");