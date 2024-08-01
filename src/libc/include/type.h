#ifndef TYPE_H
#define TYPE_H

#define NULL ((void *)0)

typedef unsigned char u8_t;
typedef unsigned int u32_t;
typedef unsigned long u64_t;
typedef unsigned short u16_t;
typedef const char *string_t;

typedef unsigned int size_t;

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
#define container_of(ptr, type, member) ({                      \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    	\
    (type *)( (char *)__mptr - offsetof(type,member) ); })

typedef enum
{
	FALSE = 0,
	TRUE
} bool_e;

typedef struct list
{
	struct list *next, *prev;
} list_t;

typedef struct registers_t
{
	u32_t ds;
	u32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	u32_t int_no, err_code;
	u32_t eip, cs, eflags, user_esp, user_ss;
} registers_t;

#endif