#ifndef STRING_H
#define STRING_H

#include "type.h"
#include <stdbool.h>

/**
 * find length of string @param ptr
 */
int strlen(const char* ptr);
/**
 * find length of string 
 * @param ptr string whose length is to be determined
 * @param max maximum count
 */
int strnlen(const char* ptr, int max);
/**
 * check if character parsed is a digit
 * @param c character to be tested
 */
bool isdigit(char c);
/**
 * conversion to digit
 * @param c character to be converted
 */
int tonumericdigit(char c);
/**
 * copy from one string to the other
 * @param dest destination
 * @param src source
 */
char* strcpy(char* dest, const char* src);
/**
 * copy from string to string with count limit
 * @param count restriction
 * @param dest destination
 * @param src source
 */
char* strncpy(char* dest, const char* src, int count);
/**
 * compare to certain number of bits
 */
int strncmp(const char* str1, const char* str2, int n);
/**
 * 
 */
int istrncmp(const char* s1, const char* s2, int n);
/**
 * find position of null terminator
 * @param str string
 * @param max maximup possible position of null terminator
 * @param terminator terminator
 */
int strnlen_terminator(const char* str, int max, char terminator);
/**
 * 
 */
char tolower(char s1);

#endif