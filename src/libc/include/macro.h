#ifndef MACRO_H
#define MACRO_H

#define stringify(x) #x

#define concatenate(a, b) a##b
#define expand_concatenate(a, b) concatenate(a, b)
#define unique_id(prefix) expand_concatenate(prefix, __COUNTER__)

/**
 * Packed struct
 * Removes padding from structs
 */
#define __PACKED__ __attribute__((packed))

/**
 * Unused variable
 */
#define __UNUSED__ __attribute__((unused))

#define __MUST_CHECK__ __attribute__((warn_unused_result));

#define align(x) __attribute__((__aligned__(x)))

/**
 * Cold attribute - the function is not called often
 */
#define __COLD__ __attribute__((cold))

/**
 * Weak attribute - the function/variable can be overridden
 */
#define __WEAK__ __attribute__((weak))
#define __USED__ __attribute__((used))
/**
 * Naked attribute - removes function prologue and epilogue.
 * Should be used for functions that have inline assembly
 */
#define __NAKED__ __attribute__((naked))
#define __NORETURN__ __attribute__((noreturn))
#define __INLINE__ inline __attribute__((always_inline))
#define __HIDDEN__ __attribute__((__visibility__("hidden")))

#define cleanup(func) __attribute__((__cleanup__(func)))
#define section(section) __attribute__((__section__(section)))

#if __has_attribute(fallthrough)
#define fallthrough __attribute__((fallthrough))
#else
#define fallthrough \
	do              \
	{               \
	} while (0) /* fallthrough */
#endif

#endif