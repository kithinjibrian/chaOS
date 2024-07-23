#include "vga.h"

static u16_t *video_memory = (u16_t *)VIDEO_ADDRESS;
static int cursor_x = 0;
static int cursor_y = 0;

static void move_cursor()
{
	u16_t pos = cursor_y * MAX_COLS + cursor_x;
	port_byte_out(REG_SCREEN_CTRL, 14);
	port_byte_out(REG_SCREEN_DATA, (pos >> 8) & 0xFF); // Ensure 8-bit values
	port_byte_out(REG_SCREEN_CTRL, 15);
	port_byte_out(REG_SCREEN_DATA, pos & 0xFF);
}

static void scroll()
{
	if (cursor_y >= MAX_ROWS)
	{
		int i;
		for (i = 0; i < MAX_COLS * (MAX_ROWS - 1); i++)
			video_memory[i] = video_memory[i + MAX_COLS];

		for (i = MAX_COLS * (MAX_ROWS - 1); i < MAX_COLS * MAX_ROWS; i++)
			video_memory[i] = ' ' | (WHITE_ON_BLACK << 8);

		cursor_y = MAX_ROWS - 1;
	}
}

void clear()
{
	int i;
	for (i = 0; i < MAX_COLS * MAX_ROWS; i++)
		video_memory[i] = ' ' | (WHITE_ON_BLACK << 8);

	cursor_x = 0;
	cursor_y = 0;
	move_cursor();
}

void print_char(char c)
{
	/* Handle backspace, by decrementing cursor position */
	if (c == '\b')
	{
		/* If cursor is at the start of the line and not on the first line */
		if (cursor_x == 0 && cursor_y != 0)
		{
			cursor_y--;				 // Move up to the previous line
			cursor_x = MAX_COLS - 1; // Move to the end of the new line
		}
		else if (cursor_x > 0)
		{
			cursor_x--; // Move cursor left if not at the start of the line
		}

		/* Clear the character at the current cursor position */
		video_memory[cursor_y * MAX_COLS + cursor_x] = ' ' | (WHITE_ON_BLACK << 8);
	}

	/*
	Handle tab, by incrementing cursor position
	but only to a point where it is a multiple of 8
	*/
	else if (c == '\t')
		cursor_x = (cursor_x + 8) & ~(8 - 1);

	/* Handle carriage return */
	else if (c == '\r')
		cursor_x = 0;

	/* Handle line feed */
	else if (c == '\n')
	{
		cursor_x = 0;
		cursor_y++;
	}

	/* Printable characters only */
	else if (c >= ' ' && c <= '~')
	{
		video_memory[cursor_y * MAX_COLS + cursor_x] = c | (WHITE_ON_BLACK << 8);
		cursor_x++;
	}

	if (cursor_x >= MAX_COLS)
	{
		cursor_x = 0;
		cursor_y++;
	}

	scroll();
	move_cursor();
}

/**
 * Print a formatted string to the VGA.
 * Expected format specifier is %c, %s, %d.
 * %c - print a character.
 * %s - print a string.
 * %d - print an integer.
 * @param fmt format string
 * @param ... arguments
 */
void print(const char *fmt, ...)
{
	/**
	 * Get the pointer to the first argument
	 */
	int *p = (int *)(&fmt + 1);

	while (*fmt)
	{
		if (*fmt == '%')
		{
			fmt++;
			switch (*fmt)
			{
			case 'c':
				char c = (char)*p;
				print_char(c);
				break;

			case 's':
				char *s = (char *)*p;
				while (*s)
				{
					print_char(*s);
					s++;
				}
				break;

			case 'd':
			{
				int num = *p;

				/**
				 *  a 32 bit integer is at most 11 digits plus the sign
				 */
				char ac[11];

				itoa(num, ac, 10);

				for (int i = 0; i < 11; i++)
					print_char(ac[i]);

				break;
			}

			default:
				print_char('%');
				print_char(*fmt);
				break;
			}

			p++;
		}
		else
		{
			print_char(*fmt);
		}
		fmt++;
	}
}
