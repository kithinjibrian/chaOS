#ifndef KHEAP_H
#define KHEAP_H

/**
 * Kernel Heap
 */

#include "../../drivers/include/vga.h"  /* print() */
#include "../../libc/include/type.h"

void kheap_init();
void* kmalloc(u32_t size);
void* kzalloc(u32_t size);
void kfree(void* ptr);


#endif