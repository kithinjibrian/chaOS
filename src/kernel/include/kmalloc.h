#ifndef KMALLOC_H
#define KMALLOC_H

#include "vga.h"
#include "page.h"
#include "type.h"
#include "list.h"
#include "massert.h"

typedef struct pool
{
	u8_t *buffer;
	u32_t size;
	u32_t csize;
	list_t list;
} pool_t, *pool_ptr_t;

void kfree(void *ptr);
void *kmalloc(u32_t size);

void *kmalloc_pa(u32_t size);
void *kmalloc_pp(u32_t size, u32_t *phys);
void *kmalloc_p(u32_t size, u32_t align, u32_t *phys);

void set_placement_address(u32_t addr);

#endif