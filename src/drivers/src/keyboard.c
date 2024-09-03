#include "keyboard.h"

bool_e caps_on_g = FALSE;
bool_e shift_on_g = FALSE;

const u32_t UNKNOWN_g = 0xFFFFFFFF;
const u32_t F1_g = 0xFFFFFFFF - 1;
const u32_t F2_g = 0xFFFFFFFF - 2;
const u32_t F3_g = 0xFFFFFFFF - 3;
const u32_t F4_g = 0xFFFFFFFF - 4;
const u32_t F5_g = 0xFFFFFFFF - 5;
const u32_t F6_g = 0xFFFFFFFF - 6;
const u32_t F7_g = 0xFFFFFFFF - 7;
const u32_t F8_g = 0xFFFFFFFF - 8;
const u32_t F9_g = 0xFFFFFFFF - 9;
const u32_t UP_g = 0xFFFFFFFF - 10;
const u32_t F10_g = 0xFFFFFFFF - 11;
const u32_t F11_g = 0xFFFFFFFF - 12;
const u32_t F12_g = 0xFFFFFFFF - 13;
const u32_t ESC_g = 0xFFFFFFFF - 14;
const u32_t ALT_g = 0xFFFFFFFF - 15;
const u32_t END_g = 0xFFFFFFFF - 16;
const u32_t CTRL_g = 0xFFFFFFFF - 17;
const u32_t HOME_g = 0xFFFFFFFF - 18;
const u32_t LEFT_g = 0xFFFFFFFF - 19;
const u32_t DOWN_g = 0xFFFFFFFF - 20;
const u32_t RIGHT_g = 0xFFFFFFFF - 21;
const u32_t RSHIFT_g = 0xFFFFFFFF - 22;
const u32_t LSHIFT_g = 0xFFFFFFFF - 23;
const u32_t PAGEUP_g = 0xFFFFFFFF - 24;
const u32_t INSERT_g = 0xFFFFFFFF - 25;
const u32_t DELETE_g = 0xFFFFFFFF - 26;
const u32_t NUMLOCK_g = 0xFFFFFFFF - 27;
const u32_t CAPSLOCK_g = 0xFFFFFFFF - 28;
const u32_t PAGEDOWN_g = 0xFFFFFFFF - 29;
const u32_t PRINTSCN_g = 0xFFFFFFFF - 30;
const u32_t SCROLLLOCK_g = 0xFFFFFFFF - 31;
const u32_t PAUSEBREAK_g = 0xFFFFFFFF - 32;

const u32_t keymap[128] = {
	UNKNOWN_g, ESC_g,
	//
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
	//
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
	//
	CTRL_g, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', LSHIFT_g,
	//
	'\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', RSHIFT_g,
	//
	'*', ALT_g, ' ', CAPSLOCK_g, F1_g, F2_g, F3_g, F4_g, F5_g, F6_g, F7_g, F8_g, F9_g, F10_g,
	//
	NUMLOCK_g, SCROLLLOCK_g, HOME_g, UP_g, PAGEUP_g,
	//
	'-',
	//
	LEFT_g, UNKNOWN_g, RIGHT_g,
	//
	'+',
	//
	END_g, DOWN_g, PAGEDOWN_g,
	//
	INSERT_g, DELETE_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, F11_g, F12_g, UNKNOWN_g,
	//
	UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g,
	//
	UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g,
	//
	UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g,
	//
	UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g,
	//
	UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g, UNKNOWN_g};

char shift(char ch)
{
	if (ch >= 'a' && ch <= 'z')
	{
		return ch - 32;
	}

	switch (ch)
	{
	case '`':
		return '~';
	case '1':
		return '!';
	case '2':
		return '@';
	case '3':
		return '#';
	case '4':
		return '$';
	case '5':
		return '%';
	case '6':
		return '^';
	case '7':
		return '&';
	case '8':
		return '*';
	case '9':
		return '(';
	case '0':
		return ')';
	case '-':
		return '_';
	case '=':
		return '+';
	case '[':
		return '{';
	case ']':
		return '}';
	case ';':
		return ':';
	case '\'':
		return '"';
	case ',':
		return '<';
	case '.':
		return '>';
	case '/':
		return '?';
	case '\\':
		return '|';
	}

	return ch;
}

void keyboard_handler(registers_t *regs __UNUSED__)
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
				ch = shift(keymap[scan_code]);
			}
			else
			{
				ch = keymap[scan_code];
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
			printf("%c", ch);
		}
		break;
	}
}

int __INIT__ init_keyboard()
{
	register_irq(1, &keyboard_handler);
	return 0;
}

void __EXIT__ exit_keyboard()
{
	free_irq(1);
}

module_init(init_keyboard);
module_exit(exit_keyboard);
module_author("Kithinji Brian");
module_description("Simple keyboard driver that prints to VGA screen.");