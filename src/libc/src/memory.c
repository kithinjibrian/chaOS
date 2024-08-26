#include "string.h"

/**
 * Copy a block of memory
 * @param dest pointer to destination
 * @param source pointer to source
 * @param count number of bytes
 */
void memcpy(void *dest, void *source, size_t count)
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
void memset(void *dest, int val, size_t count)
{
	u8_t *ptr = (u8_t *)dest;
	while (count--)
		*ptr++ = (u8_t)val;
}

/**
 * Compare two blocks of memory
 * @param s1 pointer to first block
 * @param s2 pointer to second block
 * @param count number of bytes
 * @return TRUE if the blocks are equal, FALSE otherwise
 */
bool_e memcmp(void *s1, void *s2, size_t count)
{
	u8_t *p1 = (u8_t *)s1;
	u8_t *p2 = (u8_t *)s2;
	while (count--)
		if (*p1++ != *p2++)
			return FALSE;
	return TRUE;
}