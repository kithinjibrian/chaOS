#ifndef MEMORY_H
#define MEMORY_H

#include "../../config/config.h"


void kmemset(void *dest, int val, u32_t count);
void kmemcpy(char *source, char *dest, u32_t count);
int kmemcmp(void* s1, void* s2, int count);

#endif

