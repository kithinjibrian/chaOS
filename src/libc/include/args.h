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

static inline void *__va_arg__(va_list_t list, size_t size)
{
	void *ptr = list.current;
	list.current += size;
	return ptr;
}

/**
 * va_arg - Get the next argument from a variable argument list
 * @param list: The variable argument list
 * @param type: The type of the argument
 */
#define va_arg(list, type) (*(type *)__va_arg__(list, sizeof(type)))

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

/**
 * count_args - Count the number of variable arguments
 * @param type: The type of the argument
 * @param ...: The variable arguments
 *
 * Warning: All arguments must be of the same type
 */
#define count_args(type, ...) (sizeof((type[]){__VA_ARGS__}) / sizeof(type))

#endif