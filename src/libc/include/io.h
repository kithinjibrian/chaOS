#ifndef IO_H
#define IO_H

#include "arg.h"
#include "math.h"
#include "ctype.h"
#include "macro.h"
#include "string.h"

#define ZEROPAD 1  /* pad with zero */
#define SIGN 2	   /* unsigned/signed long */
#define PLUS 4	   /* show plus */
#define SPACE 8	   /* space if plus */
#define LEFT 16	   /* left justified */
#define SMALL 32   /* Must be 32 == 0x20 */
#define SPECIAL 64 /* 0x */

static __INLINE__ int __do_div__(size_t *n, int base)
{
	int res = *n % (unsigned)base;
	*n /= (unsigned)base;
	return res;
}

#define do_div(n, base) __do_div__((size_t *)&(n), (unsigned)(base))

/* console.c */
void puts(const char *str);

int printf(const char *fmt, ...);
int sprintf(char *buf, const char *fmt, ...);
int vsprintf(char *buf, const char *fmt, va_list_t args);

#endif