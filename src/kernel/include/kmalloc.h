#ifndef KMALLOC_H
#define KMALLOC_H

#include "page.h"
#include "../../libc/include/type.h"
#include "../../drivers/include/vga.h"

/**
 * Defined in linker.ld file
 */
extern u32_t kernel_vend[];

u32_t kmalloc_p(u32_t size);

void set_placement_address_g(u32_t addr);

#endif