#ifndef VGA_H
#define VGA_H

#include "port.h"
#include "args.h"
#include "string.h"

#define MAX_COLS 80
#define MAX_ROWS 25
#define RED_ON_WHITE 0xf4
#define WHITE_ON_BLACK 0x0f
#define VIDEO_ADDRESS 0xC00B8000

/* Screen i/o ports */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

void clear();
void print_char(char c);
void print(const char *fmt, ...);

#endif