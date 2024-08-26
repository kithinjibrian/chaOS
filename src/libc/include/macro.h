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

#define __WARN_UNUSED__ __attribute__((warn_unused_result));

#define align(x) __attribute__((aligned(x)))

/**
 * Naked function - removes function prologue and epilogue.
 * Should be used for functions that have inline assembly
 */
#define __NAKED__ __attribute__((naked))
#define __USED__ __attribute__((__used__))
#define __HIDDEN__ __attribute__((__visibility__("hidden")))

#endif