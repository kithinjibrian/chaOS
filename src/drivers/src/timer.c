#include "timer.h"

u32_t tick_g = 0;
DLIST_HEAD(wakeup_list_g);

static void timer_callback(registers_t *regs)
{
	tick_g++;

	timer_t *t;
	dlist_for_each_entry(t, &wakeup_list_g, list)
	{
		if (t->remaining_ticks > 0)
		{
			t->remaining_ticks--;
		}

		if (t->remaining_ticks == 0)
		{
			t->handler(regs);			   // Call the handler
			t->remaining_ticks = t->delay; // Reset the remaining ticks
		}
	}
}

/**
 * Initialize the timer
 * @param freq The frequency of the timer
 */
int __INIT__ init_timer(void)
{
	tick_g = 0;

	register_irq_handler(0, &timer_callback);
	u32_t divisor = 1193180 / 1;

	if (divisor > 65535)
	{
		divisor = 65535;
	}

	/* Send command to start timer */
	port_byte_out(0x43, 0x36);

	/* Send the divisor byte-wise by splitting it in upper and lower bytes */
	port_byte_out(0x40, (u8_t)(divisor & 0xFF));
	port_byte_out(0x40, (u8_t)((divisor >> 8) & 0xFF));

	return 0;
}

/**/
void set_interval(fun_timer_cb cb, int delay)
{
	timer_t *t = (timer_t *)kmalloc(sizeof(timer_t));
	t->handler = cb;
	t->delay = delay;
	t->remaining_ticks = delay;

	init_dlist_head(&(t->list));

	dlist_add_tail(&wakeup_list_g, &(t->list));
}

module_init(init_timer);
module_author("Kithinji Brian");
module_description("Simple timer.");