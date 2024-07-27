#include "../include/memory.h"

/**
 * Copy a block of memory
 * @param source pointer to source
 * @param dest pointer to destination
 * @param count number of bytes
 */
void kmemcpy(char *source, char *dest, u32_t count)
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
void kmemset(void *dest, int val, u32_t count)
{
	u8_t *ptr = (u8_t *)dest;
	while (count--)
		*ptr++ = (u8_t)val;
}

/**
 * compare values of two memory blocks
 * @param count compare upto this number of bytes
 */
int kmemcmp(void* s1, void* s2, int count)
{
    char* c1 = s1;
    char* c2 = s2;
    while(count-- > 0)
    {
        if (*c1++ != *c2++)
        {
            return c1[-1] < c2[-1] ? -1 : 1;
        }
    }

    return 0;
}