#ifndef PMM_H
#define PMM_H

#include "vga.h"
#include "type.h"
#include "mmath.h"
#include "bitmap.h"
#include "kmalloc.h"
#include "multiboot.h"

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