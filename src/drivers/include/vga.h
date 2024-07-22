#ifndef VGA_H
#define VGA_H

#include "port.h"

#define MAX_COLS 80
#define MAX_ROWS 25
#define VIDEO_ADDRESS 0xB8000

/* Screen i/o ports */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

void clear();
void print(char *message);
void print_at(char *message, int col, int row);

#endif