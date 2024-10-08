#ifndef KERNEL_H
#define KERNEL_H

#include "io.h"
#include "pmm.h"
#include "gdt.h"
#include "idt.h"
#include "page.h"
#include "arg.h"
#include "timer.h"
#include "array.h"
#include "module.h"
#include "kmalloc.h"
#include "keyboard.h"
#include "process.h"
#include "syscall.h"

#define logo(name)                                                  \
	char name[] = {                                                 \
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5f, 0x20, 0x20, \
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, \
		0x20, 0x20, 0x5f, 0x5f, 0x5f, 0x5f, 0x20, 0x20, 0x20, 0x20, \
		0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x20, 0xa, 0x20, 0x20, 0x20,  \
		0x20, 0x20, 0x20, 0x7c, 0x20, 0x7c, 0x20, 0x20, 0x20, 0x20, \
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2f, 0x20, \
		0x5f, 0x5f, 0x20, 0x5c, 0x20, 0x20, 0x2f, 0x20, 0x5f, 0x5f, \
		0x5f, 0x5f, 0x7c, 0xa, 0x20, 0x20, 0x5f, 0x5f, 0x5f, 0x20,  \
		0x7c, 0x20, 0x7c, 0x5f, 0x5f, 0x20, 0x20, 0x20, 0x20, 0x5f, \
		0x5f, 0x20, 0x5f, 0x20, 0x7c, 0x20, 0x7c, 0x20, 0x20, 0x7c, \
		0x20, 0x7c, 0x7c, 0x20, 0x28, 0x5f, 0x5f, 0x5f, 0x20, 0x20, \
		0xa, 0x20, 0x2f, 0x20, 0x5f, 0x5f, 0x7c, 0x7c, 0x20, 0x27,  \
		0x5f, 0x20, 0x5c, 0x20, 0x20, 0x2f, 0x20, 0x5f, 0x60, 0x20, \
		0x7c, 0x7c, 0x20, 0x7c, 0x20, 0x20, 0x7c, 0x20, 0x7c, 0x20, \
		0x5c, 0x5f, 0x5f, 0x5f, 0x20, 0x5c, 0x20, 0xa, 0x7c, 0x20,  \
		0x28, 0x5f, 0x5f, 0x20, 0x7c, 0x20, 0x7c, 0x20, 0x7c, 0x20, \
		0x7c, 0x7c, 0x20, 0x28, 0x5f, 0x7c, 0x20, 0x7c, 0x7c, 0x20, \
		0x7c, 0x5f, 0x5f, 0x7c, 0x20, 0x7c, 0x20, 0x5f, 0x5f, 0x5f, \
		0x5f, 0x29, 0x20, 0x7c, 0xa, 0x20, 0x5c, 0x5f, 0x5f, 0x5f,  \
		0x7c, 0x7c, 0x5f, 0x7c, 0x20, 0x7c, 0x5f, 0x7c, 0x20, 0x5c, \
		0x5f, 0x5f, 0x2c, 0x5f, 0x7c, 0x20, 0x5c, 0x5f, 0x5f, 0x5f, \
		0x5f, 0x2f, 0x20, 0x7c, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x2f}

extern u32_t initcall_start_g[];
extern u32_t initcall0_start_g[];
extern u32_t initcall1_start_g[];
extern u32_t initcall2_start_g[];
extern u32_t initcall3_start_g[];
extern u32_t initcall4_start_g[];
extern u32_t initcall5_start_g[];
extern u32_t initcall6_start_g[];
extern u32_t initcall7_start_g[];
extern u32_t initcall_end_g[];

/**
 * main kernel process
 */
void main(u32_t magic, multiboot_t *mbt);
void do_initcall();

#endif