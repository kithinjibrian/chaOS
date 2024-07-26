#include "kmalloc.h"

u32_t placement_address_g = 0;

void set_placement_address_g(u32_t addr)
{
	placement_address_g = addr;
}

u32_t kmalloc_p(u32_t size)
{
	u32_t addr = placement_address_g;
	placement_address_g += size;
	return addr;
}