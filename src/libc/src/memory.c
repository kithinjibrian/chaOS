#include "mstring.h"

/**
 * Copy a block of memory
 * @param source pointer to source
 * @param dest pointer to destination
 * @param count number of bytes
 */
void memcpy(char *source, char *dest, u32_t count)
{
	int i;
	for (i = 0; i < count; i++)
		*(dest + 1) = *(source + 1);
}

/**
 * Fill a block of memory with a value
 * @param dest pointer to memory
 * @param val value to fill
 * @param count number of bytes
 */
void memset(void *dest, int val, u32_t count)
{
	u8_t *ptr = (u8_t *)dest;
	while (count--)
		*ptr++ = (u8_t)val;
}