#ifndef TYPE_H
#define TYPE_H

#define NULL ((void *)0)

typedef signed char s8_t;
typedef signed int s32_t;
typedef signed long s64_t;
typedef signed short s16_t;
typedef signed long ssize_t;

typedef unsigned char u8_t;
typedef unsigned int u32_t;
typedef unsigned long u64_t;
typedef unsigned short u16_t;
typedef unsigned long size_t;

typedef const char *string_t;

/**
 * void_fun - void function pointer
 * This pointer can point to any function.
 * Remember to cast it to the correct type
 */
typedef void (*void_fun)(void);

#define offsetof(type, member) ((size_t)(&((type *)0)->member))

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the member within the struct.
 */
static inline void *__container_of__(const void *ptr, size_t offset)
{
	return (void *)((char *)ptr - offset);
}

#define container_of(ptr, type, member) (         \
	(type *)__container_of__((const void *)(ptr), \
							 offsetof(type, member)))

typedef enum
{
	FALSE = 0,
	TRUE
} bool_e;

typedef struct dlist
{
	struct dlist *next, *prev;
} dlist_t;

typedef struct slist
{
	struct slist *next;
} slist_t;

typedef struct va_list
{
	u8_t *start;
	u8_t *current;
} va_list_t;

typedef struct array
{
	// array buffer
	u8_t *array;
	// reference counter
	size_t ref;
	// size of each element
	size_t size;
	// number of elements
	size_t count;
	// index for tracking the current element
	size_t index;
	// length of the array
	size_t length;
} array_t;

typedef void (*fun_free)(void *ptr);
typedef void *(*fun_malloc)(size_t size);
typedef void *(*fun_realloc)(void *ptr, size_t size);

typedef struct allocator
{
	fun_free free;
	fun_malloc malloc;
	fun_realloc realloc;
} allocator_t;

/**
 * registers - x86 registers
 */
typedef struct registers
{
	// Segment registers (16 bytes)
	u32_t gs, fs, es, ds;
	// General-Purpose Registers
	u32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	// Interrupt and Error Code:
	u32_t int_no, err_code;
	// Instruction Pointer and Flags:
	u32_t eip, cs, eflags, user_esp, ss;
} __attribute__((packed)) registers_t;

#endif