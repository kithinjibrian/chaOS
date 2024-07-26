#ifndef KERNEL_H
#define KERNEL_H

#include "page.h"
#include "pmm.h"

#include "../../cpu/include/gdt.h"
#include "../../cpu/include/idt.h"
#include "../../cpu/include/timer.h"
#include "../../drivers/include/vga.h"
#include "../../drivers/include/keyboard.h"

/**
 * main kernel process
 */
void main(u32_t magic, multiboot_t *mbt);

/**
 * alert for kernel failure
 */
void panic(const char *);

/**
 * error handling mechanism
 * cast error to void poiter and return
 */
#define ERROR(value) (void *)(value)
/**
 * integer error reporting mechanism
 */
#define ERROR_I(value) (int)(value)
/**
 * boolean error reporting
 */
#define ISERR(value) ((int)value < 0)

#endif