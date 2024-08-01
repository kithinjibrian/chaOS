#include "pmm.h"

static u32_t mem_size_g = 0;
static u32_t used_blocks_g = 0;
static u32_t max_blocks_g = 0;
u32_t *mem_map_g = (u32_t *)BITMAP;

static void map_memory();

int init_pmm()
{
	mem_size_g = mboot_g->mem_upper * 1024;
	max_blocks_g = mem_size_g / MBLOCK_SIZE;

	int n = CEIL(CEIL(max_blocks_g, 8), 0x1000);
	void *a = kmalloc_pa(n << 12);

	for (int i = 0; i < n; i++)
	{
		map_page((u32_t)a + (i << 12),
				 BITMAP + (i << 12),
				 PAGE_PRESENT | PAGE_WRITE);
	}

	used_blocks_g = max_blocks_g;

	/**
	 * set all memory blocks to used
	 * Divide by 8 because each bit is a byte and memset only sets 1 byte
	 */
	memset(mem_map_g, 0xff, n << 12);

	map_memory();

	return 0;
}

static void map_memory()
{
	for (int a = 0; a < mboot_g->mmap_length; a += sizeof(multiboot_mmap_entry_t))
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
					print("addr: %x, len: %d, type: %d\n",
						  entry->addr_low,
						  entry->len_low,
						  entry->type);

					pmm_init_region(placement_address_g, entry->len_low);
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
		bit_clear_v32(mem_map_g, start);
		used_blocks_g--;
	}
}

void pmm_deinit_region(u32_t base, u32_t size)
{
	int start = CEIL(base, MBLOCK_SIZE);
	int num_blocks = CEIL(size, MBLOCK_SIZE);

	for (; num_blocks > 0; num_blocks--, start++)
	{
		bit_set_v32(mem_map_g, start);
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
	int frame = bit_search_v32(mem_map_g, max_blocks_g, FALSE);

	if (frame == -1)
	{
		return NULL;
	}

	bit_set_v32(mem_map_g, frame);
	used_blocks_g++;

	return (void *)(frame * MBLOCK_SIZE);
}

void pmm_free_block(void *block)
{
	int frame = CEIL((u32_t)block, MBLOCK_SIZE);
	bit_clear_v32(mem_map_g, frame);
	used_blocks_g--;
}