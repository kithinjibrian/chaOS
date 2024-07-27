#include "page.h"

/**
 * All address above 0xc0000000 are kernel address
 */
void init_page()
{
	/**
	 * Recursively map all pages in the initial page table
	 * The page dir is at 0xFFFFF000
	 */
	initial_page_dir[1023] = ((u32_t)initial_page_dir - KERNEL_VBASE) |
							 PAGE_PRESENT |
							 PAGE_WRITE;

	/**
	 * Remap kernel memory region
	 */
	physical_address pt_kernel_addr = (physical_address)pmm_alloc_block();
	page_table_t *pt_kernel = (page_table_t *)(KERNEL_VBASE + pt_kernel_addr);

	memset(pt_kernel, 0, sizeof(page_table_t));

	/**
	 * This are the pages the kernel has used so far
	 * Assuming only the kernel has allocated these
	 */
	int kernel_pages = PT_INDEX(placement_address_g) - PT_INDEX(KERNEL_LOAD_ADDR);

	for (int i = 0, frame = KERNEL_LOAD_ADDR; i < 1024; i++, frame += PAGE_SIZE)
	{
		if (i < kernel_pages)
		{
			pt_kernel->entries[i].present = 1;
			pt_kernel->entries[i].frame = frame >> 12;
		}
		else
		{
			pt_kernel->entries[i].present = 0;
		}
	}

	initial_page_dir[PD_INDEX(KERNEL_VBASE)] = pt_kernel_addr |
											   PAGE_PRESENT |
											   PAGE_WRITE;

	print("pt kernel addr: %x\n", initial_page_dir[PD_INDEX(KERNEL_VBASE)]);
}

void print_page_table(page_table_t *page_table)
{
	for (int i = 0; i < 5; i++)
	{
		print("page number: %d, "
			  "page present: %d, "
			  "page write: %d, "
			  "page frame: %d\n",
			  i, page_table->entries[i].present,
			  page_table->entries[i].read_write,
			  page_table->entries[i].frame);
	}
}

void print_page_dir()
{
	u32_t *page_dir = (u32_t *)PAGE_DIR;

	for (int i = 830; i < 840; i++)
	{
		print("pd number: %d, "
			  "pd present: %d, "
			  "pd write: %d, "
			  "pd frame: %d\n",
			  i, page_dir[i] & PAGE_PRESENT,
			  page_dir[i] & PAGE_WRITE,
			  0);
	}
}

page_t *get_page(virtual_address vaddr)
{
	u32_t *page_dir = (u32_t *)PAGE_DIR;
	u32_t pde = page_dir[PD_INDEX(vaddr)];

	if (pde & PAGE_PRESENT)
	{
		u32_t *page_table = (u32_t *)GET_PAGE_ADDRESS(vaddr);

		if (page_table[PT_INDEX(vaddr)] & PAGE_PRESENT)
		{
			return (page_t *)page_table[PT_INDEX(vaddr)];
		}
	}

	return NULL;
}

bool alloc_page(page_t *page)
{
	if (page->present)
	{
		return FALSE;
	}

	physical_address addr = (physical_address)pmm_alloc_block();

	if (!addr)
	{
		halt("Out of memory");
	}

	page->present = 1;
	page->read_write = 1;
	page->frame = addr >> 12;

	return TRUE;
}

bool free_page(page_t page)
{
	// pmm_free_block((void *)page.frame);
	return TRUE;
}

bool map_page(physical_address paddr, virtual_address vaddr)
{
	/**
	 * Virtual address of the page directory
	 * This address is mapped to 0xFFFFF000
	 */
	pagetable_address *page_dir = (pagetable_address *)PAGE_DIR;

	/**
	 * Physical address of the page table with bits turned on
	 */
	physical_address pde = page_dir[PD_INDEX(vaddr)];

	if (pde & PAGE_PRESENT)
	{
		/**
		 * Find the page table
		 */
		page_table_t *page_table = (page_table_t *)GET_PAGE_ADDRESS(vaddr);

		if (!page_table->entries[PT_INDEX(vaddr)].present)
		{
			page_table->entries[PT_INDEX(vaddr)].present = 1;
			page_table->entries[PT_INDEX(vaddr)].read_write = 1;
			page_table->entries[PT_INDEX(vaddr)].frame = paddr >> 12;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		/**
		 * Allocate new page table
		 */
		physical_address page_table_addr = (physical_address)pmm_alloc_block();
		page_dir[PD_INDEX(vaddr)] = page_table_addr | PAGE_PRESENT | PAGE_WRITE;

		page_table_t *page_table = (page_table_t *)GET_PAGE_ADDRESS(vaddr);

		for (int i = 0; i < 1024; i++)
		{
			if (i == PT_INDEX(vaddr))
			{
				page_table->entries[i].present = 1;
				page_table->entries[i].read_write = 1;
				page_table->entries[i].frame = paddr >> 12;
			}
			else
			{
				page_table->entries[i].present = 0;
			}
		}
	}

	return TRUE;
}

bool unmap_page(virtual_address vaddr)
{
	u32_t *page_dir = (u32_t *)PAGE_DIR;
	u32_t pde = page_dir[PD_INDEX(vaddr)];

	if (pde & PAGE_PRESENT)
	{
		page_table_t *page_table = (page_table_t *)GET_PAGE_ADDRESS(vaddr);

		if (page_table->entries[PT_INDEX(vaddr)].present)
		{
			page_table->entries[PT_INDEX(vaddr)].present = 0;
			page_table->entries[PT_INDEX(vaddr)].read_write = 0;

			/**
			 * Not sure if this should be here
			 * But to be clear, we need to invalidate
			 * a page in the TLB
			 * So that further access will not be
			 * mapped to this page and will cause a page fault
			 */
			invalidate(vaddr);
		}

		int i = 0;
		for (; i < 1024; i++)
		{
			if (page_table->entries[i].present)
				break;
		}

		if (i == 1024)
		{
			pmm_free_block((void *)(pde & 0xFFFFF000));

			page_dir[PD_INDEX(vaddr)] = 0;
		}
	}

	return TRUE;
}

void invalidate(u32_t vaddr)
{
	asm volatile("invlpg (%0)" ::"r"(vaddr) : "memory");
}