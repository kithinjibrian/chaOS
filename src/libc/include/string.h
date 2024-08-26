#ifndef STRING_H
#define STRING_H

#include "type.h"

void htoa(u32_t hex, char *str);
void itoa(int num, char *str, int base);
void memset(void *dest, int val, size_t count);
void memcpy(void *dest, void *source, size_t count);

size_t strlen(const char *str);
char *strchr(const char *str, int c);
char *strncpy(char *dest, const char *src, size_t n);
bool_e strncmp(const char *str1, const char *str2, size_t len);

#endif