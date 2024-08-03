#ifndef KERNEL_H
#define KERNEL_H

#include "pmm.h"
#include "gdt.h"
#include "idt.h"
#include "vga.h"
#include "page.h"
#include "timer.h"
#include "module.h"
#include "kmalloc.h"
#include "keyboard.h"

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