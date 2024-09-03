#include "mouse.h"

u8_t mouse_buttons_g;
s8_t mouse_x_g, mouse_y_g;

static void mouse_wait(u8_t type)
{
	u32_t time = 100000;

	while (time--)
	{
		if (type == 0)
		{
			if ((port_byte_in(MOUSE_CMD_PORT) & 1) == 1)
				return;
		}
		else
		{
			if ((port_byte_in(MOUSE_CMD_PORT) & 2) == 0)
				return;
		}
	}
}

static void mouse_write(u8_t data)
{
	mouse_wait(1);
	port_byte_out(MOUSE_CMD_PORT, MOUSE_WRITE); // tell the mouse we want to write
	mouse_wait(1);
	port_byte_out(MOUSE_CMD_PORT, data); // write the data
}

static u8_t mouse_read()
{
	mouse_wait(0);
	return port_byte_in(MOUSE_DATA_PORT);
}

void mouse_handler(registers_t *regs __UNUSED__)
{
	// u8_t status = mouse_read();

	print("Mouse: %c, %c\n", 10, 20);
}

int __INIT__ init_mouse(void)
{
	mouse_wait(1);
	mouse_write(0xA8); // Enable auxiliary mouse port

	mouse_wait(1);
	mouse_write(0x20); // Get the current state

	mouse_wait(0);
	u8_t status = (mouse_read() | 2); // Set the 2nd bit to enable the mouse

	mouse_wait(1);
	mouse_write(0x60); // Set the state

	mouse_wait(1);
	mouse_write(status);

	mouse_wait(1);
	// mouse_write(0xF6); // Set default settings
	if (mouse_read() != 0xFA)
	{
		print("Failed to enable mouse\n");
	}

	// mouse_write(0xF4); // Enable the mouse
	// mouse_read();

	register_irq(12, &mouse_handler);

	return 0;
}

void __EXIT__ exit_mouse(void)
{
	free_irq(12);
}

module_init(init_mouse);
module_exit(exit_mouse);
module_author("Kithinji Brian");
module_description("Simple mouse driver.");