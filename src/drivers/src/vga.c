#include "vga.h"

static u16_t *video_memory = (u16_t *)VIDEO_ADDRESS;
static int cursor_x = 0;
static int cursor_y = 0;

static void move_cursor()
{
	u16_t pos = cursor_y * MAX_COLS + cursor_x;
	port_byte_out(REG_SCREEN_CTRL, 14);
	port_byte_out(REG_SCREEN_DATA, pos >> 8);
	port_byte_out(REG_SCREEN_CTRL, 15);
	port_byte_out(REG_SCREEN_DATA, pos);
}

static void scroll()
{
	u8_t attr = (0 << 4) | (15 & 0x0F);
	u8_t blank = 0x20 | (attr << 8);

	if (cursor_y >= MAX_ROWS)
	{
		int i;
		for (i = 0; i < MAX_COLS * (MAX_ROWS - 1); i++)
			video_memory[i] = video_memory[i + MAX_COLS];

		for (i = MAX_COLS * (MAX_ROWS - 1); i < MAX_COLS * MAX_ROWS; i++)
			video_memory[i] = blank;

		cursor_y = MAX_ROWS - 1;
	}
}

void clear()
{
	u8_t attr = (0 << 4) | (15 & 0x0F);
	u8_t blank = 0x20 | (attr << 8);
	int i;
	for (i = 0; i < MAX_COLS * MAX_ROWS; i++)
		video_memory[i] = blank;

	cursor_x = 0;
	cursor_y = 0;
	move_cursor();
}

void print_at(char c)
{
	u16_t attr = ((0 << 4) | (15 & 0x0F)) << 8;
	u16_t *location;

	if (c == 0x08 && cursor_x)
		cursor_x--;
	else if (c == 0x09)
		cursor_x = (cursor_x + 8) & ~(8 - 1);
	else if (c == '\r')
		cursor_x = 0;
	else if (c == '\n')
	{
		cursor_x = 0;
		cursor_y++;
	}
	else if (c >= ' ')
	{
		location = video_memory + (cursor_y * MAX_COLS + cursor_x);
		*location = c | attr;
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

void print(char *message)
{
	int i = 0;
	while (message[i] != '\0')
		print_at(message[i++]);
}
