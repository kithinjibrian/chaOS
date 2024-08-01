#include "mstring.h"

/**
 * Copy a block of memory
 * @param dest pointer to destination
 * @param source pointer to source
 * @param count number of bytes
 */
void memcpy(void *dest, void *source, u32_t count)
{
	u8_t *d = (u8_t *)dest;
	u8_t *s = (u8_t *)source;
	while (count--)
		*d++ = *s++;
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