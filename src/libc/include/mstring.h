#ifndef STRING_H
#define STRING_H

#include "type.h"

void htoa(u32_t hex, char *str);
void itoa(int num, char *str, int base);
void memset(void *dest, int val, u32_t count);
void memcpy(void *dest, void *source, u32_t count);

#endif