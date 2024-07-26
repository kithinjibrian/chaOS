#include "pmm.h"

static u32_t mem_size_g = 0;
static u32_t used_blocks_g = 0;
static u32_t max_blocks_g = 0;
u32_t *mem_map_g = 0;

void init_pmm(u32_t mem_size)
{
	mem_size_g = mem_size;
	max_blocks_g = mem_size / MBLOCK_SIZE;

	mem_map_g = (u32_t *)kmalloc_p(CEIL(max_blocks_g, 8));

	used_blocks_g = max_blocks_g;

	/**
	 * set all memory blocks to used
	 * Divide by 8 because each bit is a byte and memset only sets 1 byte
	 */
	memset(mem_map_g, 0xff, CEIL(max_blocks_g, 8));
}

void pmm_init_region(u32_t base, u32_t size)
{
	int start = base / MBLOCK_SIZE;
	int num_blocks = size / MBLOCK_SIZE;

	for (; num_blocks > 0; num_blocks--, start++)
	{
		bit_clear_v32(mem_map_g, start);
		used_blocks_g--;
	}
}

void pmm_deinit_region(u32_t base, u32_t size)
{
	int start = base / MBLOCK_SIZE;
	int num_blocks = size / MBLOCK_SIZE;

	for (; num_blocks > 0; num_blocks--, start++)
	{
		bit_set_v32(mem_map_g, start);
		used_blocks_g++;
	}
}

void *pmm_alloc_block()
{
	int frame = bit_search_v32(mem_map_g, max_blocks_g, false);

	if (frame == -1)
	{
		print("Error!: Out of memory.\n");
		while (1)
			;
	}

	bit_set_v32(mem_map_g, frame);
	used_blocks_g++;

	return (void *)(frame * MBLOCK_SIZE);
}

void pmm_free_block(void *block)
{
	int frame = (u32_t)block / MBLOCK_SIZE;
	bit_clear_v32(mem_map_g, frame);
	used_blocks_g--;
}