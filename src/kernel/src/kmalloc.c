#include "kmalloc.h"

u32_t placement_address_g;

void set_placement_address(u32_t addr)
{
	placement_address_g = addr;
}

/**
 * Simple kernel's dumb kmalloc placement allocator
 * @param size size of memory
 */
void *kmalloc_p(u32_t size, u32_t align, u32_t *phys)
{
	if (align && (placement_address_g & 0xffff000))
	{

		placement_address_g &= 0xffff000;
		placement_address_g += 0x1000;
	}

	if (phys)
	{
		*phys = placement_address_g;
	}

	u32_t addr = placement_address_g;

	placement_address_g += size;

	return (void *)addr;
}

/**
 * kmalloc aligned
 * The memory allocated is page aligned
 * @param size size of memory
 */
void *kmalloc_pa(u32_t size)
{
	return kmalloc_p(size, 1, NULL);
}

/**
 *
 */
void *kmalloc_pp(u32_t size, u32_t *phys)
{
	return kmalloc_p(size, 0, phys);
}

void init_heap(void)
{
	void *a = kmalloc_pa(0x1000);
	map_page((u32_t)a, KERNEL_HEAP, PAGE_PRESENT | PAGE_WRITE);

	u8_t *kernel_heap = (u8_t *)KERNEL_HEAP;
}

void *kmalloc(u32_t size)
{
	return NULL;
}
