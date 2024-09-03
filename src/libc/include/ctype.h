#ifndef CTYPE_H
#define CTYPE_H

#define _U 0x01	 /* upper */
#define _L 0x02	 /* lower */
#define _D 0x04	 /* digit */
#define _C 0x08	 /* cntrl */
#define _P 0x10	 /* punct */
#define _S 0x20	 /* white space (space/lf/tab) */
#define _X 0x40	 /* hex digit */
#define _SP 0x80 /* hard space (0x20) */

extern const unsigned char ctype[];

#define ismask(x) (ctype[(int)(unsigned char)(x)])

#define isalnum(c) ((ismask(c) & (_U | _L | _D)) != 0)
#define isalpha(c) ((ismask(c) & (_U | _L)) != 0)
#define iscntrl(c) ((ismask(c) & (_C)) != 0)
#define isgraph(c) ((ismask(c) & (_P | _U | _L | _D)) != 0)
#define islower(c) ((ismask(c) & (_L)) != 0)
#define isprint(c) ((ismask(c) & (_P | _U | _L | _D | _SP)) != 0)
#define ispunct(c) ((ismask(c) & (_P)) != 0)
/* Note: isspace() must return false for %NUL-terminator */
#define isspace(c) ((ismask(c) & (_S)) != 0)
#define isupper(c) ((ismask(c) & (_U)) != 0)
#define isxdigit(c) ((ismask(c) & (_D | _X)) != 0)

#define isascii(c) (((unsigned char)(c)) <= 0x7f)
#define toascii(c) (((unsigned char)(c)) & 0x7f)

#if __has_builtin(__builtin_isdigit)
#define isdigit(c) __builtin_isdigit(c)
#else
static inline int isdigit(int c)
{
	return c >= '0' && c <= '9';
}
#endif

static inline unsigned char __tolower__(unsigned char c)
{
	if (isupper(c))
		c -= 'A' - 'a';
	return c;
}

static inline unsigned char __toupper__(unsigned char c)
{
	if (islower(c))
		c -= 'a' - 'A';
	return c;
}

#define tolower(c) __tolower__(c)
#define toupper(c) __toupper__(c)

#endif