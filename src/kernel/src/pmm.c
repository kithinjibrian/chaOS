#include "pmm.h"

static u32_t mem_size_g = 0;
static u32_t used_blocks_g = 0;
static u32_t max_blocks_g = 0;

/**
 * This is very wasteful because we are assuming the ram is always 4GB
 * but the other option is to try and allocate memory for the bitmap after the kernel space which I
 * found to be very erronous.
 */
u32_t physical_bitmap[BITMAP_SIZE];

static void map_memory();

int __INIT__ init_pmm()
{
	mem_size_g = mboot_g->mem_upper * 1024;
	max_blocks_g = mem_size_g / MBLOCK_SIZE;
	used_blocks_g = max_blocks_g;

	memset(physical_bitmap, 0xff, BITMAP_SIZE * sizeof(u32_t));

	map_memory();

	return 0;
}

static void map_memory()
{
	for (u32_t a = 0; a < mboot_g->mmap_length; a += sizeof(multiboot_mmap_entry_t))
	{
		multiboot_mmap_entry_t *entry = (multiboot_mmap_entry_t *)((mboot_g->mmap_addr + KERNEL_VBASE) + a);

		/**
		 * Skip 1mb low end memory that starts at 0x0
		 */
		if (entry->addr_low)
		{
			if (entry->type == 1)
			{
				/**
				 * In memory where our kernel is
				 */
				if (entry->addr_low == (u32_t)kernel_pstart_g)
				{
					/**
					 * Skip the first 4mb where the kernel lives
					 */
					pmm_init_region(0x400000, entry->len_low);
				}
				else
				{
					pmm_init_region(entry->addr_low, entry->len_low);
				}
			}
		}
	}
}

void pmm_init_region(u32_t base, u32_t size)
{
	int start = CEIL(base, MBLOCK_SIZE);
	int num_blocks = CEIL(size, MBLOCK_SIZE);

	for (; num_blocks > 0; num_blocks--, start++)
	{
		bit_clear_v32(physical_bitmap, start);
		used_blocks_g--;
	}
}

void pmm_deinit_region(u32_t base, u32_t size)
{
	int start = CEIL(base, MBLOCK_SIZE);
	int num_blocks = CEIL(size, MBLOCK_SIZE);

	for (; num_blocks > 0; num_blocks--, start++)
	{
		bit_set_v32(physical_bitmap, start);
		used_blocks_g++;
	}
}

/**
 * Allocate a block of memory.
 * Warning: Map the physical address to a virtual address space
 * @returns physical address to allocated memory
 */
void *pmm_alloc_block()
{
	int frame = bit_search_v32(physical_bitmap, max_blocks_g, FALSE);

	if (frame == -1)
	{
		return NULL;
	}

	bit_set_v32(physical_bitmap, frame);
	used_blocks_g++;

	return (void *)(frame * MBLOCK_SIZE);
}

void pmm_free_block(void *block)
{
	int frame = CEIL((u32_t)block, MBLOCK_SIZE);
	bit_clear_v32(physical_bitmap, frame);
	used_blocks_g--;
}