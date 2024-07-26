#ifndef BITMAP_H
#define BITMAP_H

#include "type.h"
#include "../../drivers/include/vga.h"

int bit_get_v8(u8_t bitmap[], int index);
void bit_set_v8(u8_t bitmap[], int index);
void bit_print_v8(u8_t bitmap[], int size);
void bit_clear_v8(u8_t bitmap[], int index);
int bit_search_v8(u8_t bitmap[], int size, int value);

int bit_get_v32(u32_t bitmap[], int index);
void bit_set_v32(u32_t bitmap[], int index);
void bit_clear_v32(u32_t bitmap[], int index);
void bit_print_v32(u32_t bitmap[], u32_t size);
int bit_search_v32(u32_t bitmap[], int size, int value);

#endif
