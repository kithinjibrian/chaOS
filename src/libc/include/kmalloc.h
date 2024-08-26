#ifndef KMALLOC_H
#define KMALLOC_H

#include "vga.h"
#include "type.h"
#include "dlist.h"
#include "assert.h"

void kfree(void *ptr);
void *kmalloc(size_t size);
void *krealloc(void *ptr, size_t size);

void *kmalloc_pa(u32_t size);
void *kmalloc_pp(u32_t size, u32_t *phys);
void *kmalloc_p(u32_t size, u32_t align, u32_t *phys);

void set_placement_address(u32_t addr);

#endif