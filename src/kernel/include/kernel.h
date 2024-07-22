#ifndef KERNEL_H
#define KERNEL_H

#include "../../drivers/include/vga.h"

/**
 * main kernel process
 */
void main(void);
/**
 * alert for kernel failure
 */
void panic(const char*);

/**
 * error handling mechanism
 * cast error to void poiter and return
 */
#define ERROR(value) (void*)(value)
/**
 * integer error reporting mechanism
 */
#define ERROR_I(value) (int)(value)
/**
 * boolean error reporting
 */
#define ISERR(value)((int)value < 0)

#endif