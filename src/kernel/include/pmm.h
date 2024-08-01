#ifndef PMM_H
#define PMM_H

#include "kmalloc.h"

#include "../../libc/include/type.h"
#include "../../libc/include/mmath.h"
#include "../../libc/include/bitmap.h"
#include "../../libc/include/multiboot.h"

#include "../../drivers/include/vga.h"

#define MBLOCK_SIZE 4096
#define BITMAP 0xD0000000

extern multiboot_t *mboot_g;
extern u32_t placement_address_g;

int init_pmm(void);

u32_t pmm_alloc_frame();
void pmm_free_page(u32_t b);

void pmm_init_region(u32_t base, u32_t size);
void pmm_deinit_region(u32_t base, u32_t size);

void *pmm_alloc_block();
void pmm_free_block(void *block);

#endif