#ifndef STRING_H
#define STRING_H

#include "type.h"

void itoa(int val, char *buf);
void memset(void *dest, int val, u32_t count);
void memcpy(char *source, char *dest, u32_t count);

#endif