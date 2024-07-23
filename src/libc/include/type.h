#ifndef TYPE_H
#define TYPE_H

typedef unsigned char u8_t;
typedef unsigned int u32_t;
typedef unsigned short u16_t;

typedef struct registers_t
{
	u32_t ds;
	u32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	u32_t int_no, err_code;
	u32_t eip, cs, eflags, user_esp, user_ss;
} registers_t;

#endif