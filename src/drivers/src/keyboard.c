#include "keyboard.h"

void keyboard_handler(registers_t regs)
{
	u8_t scancode = port_byte_in(0x60);
	u8_t press = port_byte_in(0x60);

	char ac[10];
	itoa(scancode, ac, 10);

	char ac2[10];
	itoa(press, ac2, 10);

	print(ac);
	print(" : ");
	print(ac2);
	print("\n");
}