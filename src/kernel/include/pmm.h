#ifndef PMM_H
#define PMM_H

#include "kmalloc.h"

#include "../../libc/include/type.h"
#include "../../libc/include/mmath.h"
#include "../../libc/include/bitmap.h"
#include "../../drivers/include/vga.h"

#define MBLOCK_SIZE 4096

extern u32_t placement_address_g;

void init_pmm(u32_t mem_size);

void pmm_init_region(u32_t base, u32_t size);
void pmm_deinit_region(u32_t base, u32_t size);

void *pmm_alloc_block();
void pmm_free_block(void *block);

#endif