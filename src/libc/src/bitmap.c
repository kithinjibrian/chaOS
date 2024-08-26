#include "bitmap.h"

/**
 * Search for bit at index in bitmap of size 8 bits
 * @param bitmap bitmap
 * @param index index in bitmap
 *
 * @return the index of the bit
 */
int bit_search_v8(u8_t bitmap[], int size, u8_t value)
{
	for (int i = 0; i < size * 8; ++i)
	{
		if (((bitmap[i / 8] >> (i % 8)) & 1) == value)
		{
			return i;
		}
	}
	return -1;
}

/**
 * Print bit at index in bitmap of size 8 bits
 * @param bitmap bitmap
 * @param size size of bitmap
 */
void bit_print_v8(u8_t bitmap[], int start, int size)
{
	for (int i = start; i < start + size; ++i)
	{
		print("(row: %d) = ", i);
		for (int j = 7; j >= 0; --j)
		{
			print("%d", (bitmap[i] >> j) & 1);
		}
		print(" (hex: %x) (ascii: %c)", bitmap[i], bitmap[i]);
		print("\n");
	}
	print("\n");
}

/**
 * Search for bit at index in bitmap of size 32 bits
 * @param bitmap bitmap
 * @param index index in bitmap
 *
 * @return the index of the bit
 */
int bit_search_v32(u32_t bitmap[], int size, u32_t value)
{
	for (int i = 0; i < size * 32; ++i)
	{
		if (((bitmap[i / 32] >> (i % 32)) & 1) == value)
		{
			return i;
		}
	}
	return -1;
}

/**
 * Print bit at index in bitmap of size 32 bits
 * @param bitmap bitmap
 * @param start start index in bitmap
 * @param index index in bitmap
 */
void bit_print_v32(u32_t bitmap[], u32_t start, u32_t size)
{
	for (u32_t i = start; i < start + size; ++i)
	{
		print("(row: %d) = ", i);
		for (int j = 31; j >= 0; --j)
		{
			print("%d", (bitmap[i] >> j) & 1);
		}
		print(" (hex: %x) (ascii: %c)", bitmap[i], bitmap[i]);
		print("\n");
	}
	print("\n");
}