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

void print_at(char c)
{
	/* Handle backspace, by decrementing cursor position */
	if (c == 0x08 && cursor_x)
		cursor_x--;

	/*
	Handle tab, by incrementing cursor position
	but only to a point where it is a multiple of 8
	*/
	else if (c == 0x09)
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

void print(const char *message)
{
	while (*message)
		print_at(*message++);
}
