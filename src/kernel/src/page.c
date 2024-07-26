#include "page.h"

void invalidate(u32_t vaddr);

void init_page()
{
	/**
	 * Recursively map all pages in the initial page table
	 */
	initial_page_dir[1023] = ((u32_t)initial_page_dir - KERNEL_VBASE) |
							 PAGE_PRESENT |
							 PAGE_WRITE;

	/**
	 * Identity map the first 4mb of memory
	 * Includes BIOS, VGA frame buffer, ...memory regions
	 */
	physical_address page_table_addr = (physical_address)pmm_alloc_block();
	page_table_t *page_table = (page_table_t *)(KERNEL_VBASE + page_table_addr);

	memset(page_table, 0, sizeof(page_table_t));

	for (int i = 0; i < 1024; i++)
	{
		page_table->entries[i].present = 1;
		page_table->entries[i].frame = i;
	}

	initial_page_dir[PD_INDEX(0x00000000)] = page_table_addr |
											 PAGE_PRESENT |
											 PAGE_WRITE;

	/**
	 * Remap kernel memory region
	 */
	physical_address page_table_addr2 = (physical_address)pmm_alloc_block();
	page_table_t *page_table2 = (page_table_t *)(KERNEL_VBASE + page_table_addr2);

	memset(page_table2, 0, sizeof(page_table_t));

	for (int i = 0, frame = KERNEL_LOAD_ADDR; i < 1024; i++, frame += PAGE_SIZE)
	{
		page_table2->entries[i].present = 1;
		page_table2->entries[i].frame = PT_INDEX(frame);
	}

	initial_page_dir[PD_INDEX(KERNEL_VBASE)] = page_table_addr2 |
											   PAGE_PRESENT |
											   PAGE_WRITE;
}

bool alloc_page(page_t page)
{
	physical_address addr = (physical_address)pmm_alloc_block();

	page.present = 1;
	page.frame = PT_INDEX(addr);

	return true;
}

bool free_page(page_t page)
{
	// pmm_free_block((void *)page.frame);
	return true;
}

bool map_page(physical_address paddr, virtual_address vaddr)
{
	u32_t *page_dir = (u32_t *)PAGE_DIR;

	u32_t pde = page_dir[PD_INDEX(vaddr)];

	if (pde & PAGE_PRESENT)
	{
		u32_t *page_table = (u32_t *)GET_PAGE_ADDRESS(vaddr);

		if (!(page_table[PT_INDEX(vaddr)] & PAGE_PRESENT))
		{
			page_table[PT_INDEX(vaddr)] = paddr | PAGE_PRESENT | PAGE_WRITE;
		}
		else
		{
			return false;
		}
	}
	else
	{
		/**
		 * Allocate new page table
		 */
		physical_address page_table_addr = (physical_address)pmm_alloc_block();
		page_dir[PD_INDEX(vaddr)] = page_table_addr | PAGE_PRESENT | PAGE_WRITE;

		u32_t *page_table = (u32_t *)GET_PAGE_ADDRESS(vaddr);
		page_table[PT_INDEX(vaddr)] = paddr | PAGE_PRESENT | PAGE_WRITE;
	}

	return true;
}

bool unmap_page(virtual_address vaddr)
{
	u32_t *page_dir = (u32_t *)PAGE_DIR;
	u32_t pde = page_dir[PD_INDEX(vaddr)];

	if (pde & PAGE_PRESENT)
	{
		u32_t *page_table = (u32_t *)GET_PAGE_ADDRESS(vaddr);

		if (page_table[PT_INDEX(vaddr)] & PAGE_PRESENT)
		{
			page_table[PT_INDEX(vaddr)] = 2; // r/w, not present
		}

		int i = 0;
		for (; i < 1024; i++)
		{
			if (page_table[i] & PAGE_PRESENT)
				break;
		}

		if (i == 1024)
		{
			pmm_free_block((void *)(pde & 0xFFFFF000));
			page_dir[PD_INDEX(vaddr)] = 2;
		}
	}

	return true;
}

void invalidate(u32_t vaddr)
{
	asm volatile("invlpg (%0)" ::"r"(vaddr) : "memory");
}