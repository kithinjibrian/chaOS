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
}

void print()
{
}