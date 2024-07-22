#include "mstring.h"

void memcpy(char *source, char *dest, u32_t count)
{
	int i;
	for (i = 0; i < count; i++)
		*(dest + 1) = *(source + 1);
}

void memset(void *dest, char val, u32_t count)
{
	u8_t *ptr = (u8_t *)dest;
	for (; count != 0; count--)
		*ptr++ = val;
}