#include "keyboard.h"

bool_e caps_on_g = FALSE;
bool_e shift_on_g = FALSE;

const u32_t UNKNOWN = 0xFFFFFFFF;
const u32_t F1 = 0xFFFFFFFF - 1;
const u32_t F2 = 0xFFFFFFFF - 2;
const u32_t F3 = 0xFFFFFFFF - 3;
const u32_t F4 = 0xFFFFFFFF - 4;
const u32_t F5 = 0xFFFFFFFF - 5;
const u32_t F6 = 0xFFFFFFFF - 6;
const u32_t F7 = 0xFFFFFFFF - 7;
const u32_t F8 = 0xFFFFFFFF - 8;
const u32_t F9 = 0xFFFFFFFF - 9;
const u32_t UP = 0xFFFFFFFF - 10;
const u32_t F10 = 0xFFFFFFFF - 11;
const u32_t F11 = 0xFFFFFFFF - 12;
const u32_t F12 = 0xFFFFFFFF - 13;
const u32_t ESC = 0xFFFFFFFF - 14;
const u32_t ALT = 0xFFFFFFFF - 15;
const u32_t END = 0xFFFFFFFF - 16;
const u32_t CTRL = 0xFFFFFFFF - 17;
const u32_t HOME = 0xFFFFFFFF - 18;
const u32_t LEFT = 0xFFFFFFFF - 19;
const u32_t DOWN = 0xFFFFFFFF - 20;
const u32_t RIGHT = 0xFFFFFFFF - 21;
const u32_t RSHIFT = 0xFFFFFFFF - 22;
const u32_t LSHIFT = 0xFFFFFFFF - 23;
const u32_t PAGEUP = 0xFFFFFFFF - 24;
const u32_t INSERT = 0xFFFFFFFF - 25;
const u32_t DELETE = 0xFFFFFFFF - 26;
const u32_t NUMLOCK = 0xFFFFFFFF - 27;
const u32_t CAPSLOCK = 0xFFFFFFFF - 28;
const u32_t PAGEDOWN = 0xFFFFFFFF - 29;
const u32_t PRINTSCN = 0xFFFFFFFF - 30;
const u32_t SCROLLLOCK = 0xFFFFFFFF - 31;
const u32_t PAUSEBREAK = 0xFFFFFFFF - 32;

const u32_t shift_up[128] = {
	UNKNOWN, ESC,
	//
	'!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
	//
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
	//
	CTRL, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', LSHIFT,
	//
	'\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', RSHIFT,
	//
	'*', ALT, ' ', CAPSLOCK, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10,
	//
	NUMLOCK, SCROLLLOCK, HOME, UP, PAGEUP,
	//
	'-',
	//
	LEFT, UNKNOWN, RIGHT,
	//
	'+',
	//
	END, DOWN, PAGEDOWN,
	//
	INSERT, DELETE, UNKNOWN, UNKNOWN, UNKNOWN, F11, F12, UNKNOWN,
	//
	UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
	//
	UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
	//
	UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
	//
	UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
	//
	UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN};

const u32_t shift_down[128] = {
	UNKNOWN, ESC,
	//
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
	//
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
	//
	CTRL, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', LSHIFT,
	//
	'\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', RSHIFT,
	//
	'*', ALT, ' ', CAPSLOCK, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10,
	//
	NUMLOCK, SCROLLLOCK, HOME, UP, PAGEUP,
	//
	'-',
	//
	LEFT, UNKNOWN, RIGHT,
	//
	'+',
	//
	END, DOWN, PAGEDOWN,
	//
	INSERT, DELETE, UNKNOWN, UNKNOWN, UNKNOWN, F11, F12, UNKNOWN,
	//
	UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
	//
	UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
	//
	UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
	//
	UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
	//
	UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN};

void keyboard_handler(registers_t regs)
{
	u8_t scan_code = port_byte_in(0x60) & 0x7F;
	u8_t press = port_byte_in(0x60) & 0x80;

	switch (scan_code)
	{
	/*skip esc and function keys*/
	case 1:
	case 59:
	case 60:
	case 61:
	case 62:
	case 63:
	case 64:
	case 65:
	case 66:
	case 67:
	case 68:
	case 87:
	case 88:
		break;

	case 42:
		// shift key
		shift_on_g = !press;
		break;

	case 58:
		// caps lock
		if (!press)
		{
			caps_on_g = !caps_on_g;
		}
		break;

	default:
		if (!press)
		{
			char ch;
			if (shift_on_g)
			{
				ch = shift_up[scan_code];
			}
			else
			{
				ch = shift_down[scan_code];
			}

			if (caps_on_g && ch >= 'a' && ch <= 'z')
			{
				/* convert to upper case */
				ch -= 32;
			}
			else if (caps_on_g && ch >= 'A' && ch <= 'Z')
			{
				/* convert to lower case */
				ch += 32;
			}

			print("%c", ch);
		}
		break;
	}
}

int __INIT__ init_keyboard()
{
	irq_reg_handler(1, &keyboard_handler);
	return 0;
}

module_init(init_keyboard);
module_author("Kithinji Brian");
module_description("Simple keyboard driver that prints to VGA screen.");