#ifndef KHEAP_H
#define KHEAP_H

/**
 * Kernel Heap
 */

#include "../../libc/include/type.h"

#include <stdint.h>
#include <stddef.h>

void kheap_init();
void* kmalloc(size_t size);
void* kzalloc(size_t size);
void kfree(void* ptr);


#endif