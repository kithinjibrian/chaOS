#ifndef MEMORY_H
#define MEMORY_H

#include "config/config.h"


void memset(void *dest, int val, u32_t count);
void memcpy(char *source, char *dest, u32_t count);
int memcmp(void* s1, void* s2, int count);

#endif

