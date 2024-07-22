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

static int print_char(char c, int col, int row, char attr)
{
	unsigned char *screen = (unsigned char *)VIDEO_ADDRESS;

	if (!attr)
		attr = 0x0f;

	if (col >= MAX_COLS || row >= MAX_ROWS)
	{
		screen[2 * (MAX_COLS)*MAX_ROWS - 2] = 'E';
		screen[2 * (MAX_COLS)*MAX_ROWS - 1] = 0xf4;
		return get_offset(col, row);
	}

	int offset;

	if (col >= 0 && row >= 0)
		offset = get_offset(col, row);
	else
		offset = get_cursor_offset();

	if (c == '\n')
	{
		row = get_offset_row(offset);
		offset = get_offset(0, row + 1);
	}
	else
	{
		screen[offset] = c;
		screen[offset + 1] = attr;
		offset += 2;
	}

	set_cursor_offset(offset);
	return offset;
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

void print_at(char *message, int col, int row)
{
	int offset;

	if (col >= 0 && row >= 0)
	{
		offset = get_offset(col, row);
	}
	else
	{
		offset = get_cursor_offset();
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}

	int i = 0;
	while (message[i] != 0)
	{
		offset = print_char(message[i++], col++, row, 0x0f);
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}
}

void print(char *message)
{
	print_at(message, -1, -1);
}