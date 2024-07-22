#include "vga.h"

static int get_offset(int col, int row)
{
	return 2 * (row * MAX_COLS + col);
}

static int get_offset_row(int offset)
{
	return offset / (2 * MAX_COLS);
}

static int get_offset_col(int offset)
{
	return (offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2;
}

static void set_cursor_offset(int offset)
{
	offset /= 2;
	port_byte_out(REG_SCREEN_CTRL, 14);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
	port_byte_out(REG_SCREEN_CTRL, 15);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

static int get_cursor_offset()
{
	port_byte_out(REG_SCREEN_CTRL, 14);
	int offset = port_byte_in(REG_SCREEN_DATA) << 8;
	port_byte_out(REG_SCREEN_CTRL, 15);
	offset += port_byte_in(REG_SCREEN_DATA);
	return offset * 2;
}

void clear()
{
	int i;
	int screen_size = MAX_COLS * MAX_ROWS;
	char *screen = (char *)VIDEO_ADDRESS;

	for (i = 0; i < screen_size; i++)
	{
		screen[i * 2] = ' ';
		screen[i * 2 + 1] = 0x0f;
	}

	set_cursor_offset(get_offset(0, 0));
}

void print()
{
}