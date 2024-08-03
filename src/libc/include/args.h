#ifndef ARGS_H
#define ARGS_H

#include "type.h"

/**
 * va_start - Initialize a variable argument list
 * @param list: The variable argument list
 * @param last: The last argument before ...
 */
#define va_start(list, last)                       \
	do                                             \
	{                                              \
		list.start = (u8_t *)&last + sizeof(last); \
		list.current = list.start;                 \
	} while (0);

/**
 * va_arg - Get the next argument from a variable argument list
 * @param list: The variable argument list
 * @param type: The type of the argument
 */
#define va_arg(list, type)                \
	({                                    \
		type *tmp = (type *)list.current; \
		list.current += sizeof(type);     \
		*tmp;                             \
	});

/**
 * va_copy - Copy a variable argument list
 * @param dest: The destination variable argument list
 * @param src: The source variable argument list
 */
#define va_copy(dest, src)              \
	do                                  \
	{                                   \
		(dest).start = (src).start;     \
		(dest).current = (src).current; \
	} while (0)

/**
 * va_end - Terminate a variable argument list
 * @param list: The variable argument list
 */
#define va_end(list)         \
	do                       \
	{                        \
		list.start = NULL;   \
		list.current = NULL; \
	} while (0);

#endif