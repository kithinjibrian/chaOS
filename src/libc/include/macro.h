#ifndef MACRO_H
#define MACRO_H

#define stringify(x) #x

#define concatenate(a, b) a##b
#define expand_concatenate(a, b) concatenate(a, b)
#define unique_id(prefix) expand_concatenate(prefix, __COUNTER__)

#define align(x) __attribute__((aligned(x)))

#define __USED__ __attribute__((__used__))
#define __HIDDEN__ __attribute__((__visibility__("hidden")))

#endif