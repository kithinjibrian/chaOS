#include "page.h"

void page_fault(registers_t *regs);

/**
 * All address above 0xc0000000 are kernel address
 */
int init_page(void)
{
	/**
	 * Recursively map all pages in the initial page table
	 * The page dir is at 0xFFFFF000
	 */
	initial_page_dir[1023] = ((u32_t)initial_page_dir - KERNEL_VBASE) |
							 PAGE_PRESENT |
							 PAGE_USER |
							 PAGE_WRITE;

	create_page_table(initial_page_dir, KERNEL_VBASE, PAGE_PRESENT | PAGE_WRITE | PAGE_USER);

	/**
	 * Map all pages in the kernel from (0x00000000 to 0x00400000) 4mbs
	 */
	u32_t *page_kernel = (u32_t *)GET_PAGE_ADDRESS(KERNEL_VBASE);
	for (int i = 0; i < 1024; i++)
	{
		page_kernel[i] = i * PAGE_SIZE | PAGE_PRESENT | PAGE_WRITE | PAGE_USER;
	}

	create_page_table(initial_page_dir, KERNEL_HEAP, PAGE_PRESENT | PAGE_WRITE);

	u32_t *page_heap = (u32_t *)GET_PAGE_ADDRESS(KERNEL_HEAP);
	for (int i = 0; i < 1024; i++)
	{
		u32_t paddr = (u32_t)pmm_alloc_block();
		page_heap[i] = paddr | PAGE_PRESENT | PAGE_WRITE;
	}

	register_isr_handler(14, page_fault);

	return 0;
}

void page_fault(registers_t *regs __UNUSED__)
{
	u32_t faulting_address __UNUSED__;
	__asm__ __volatile__("mov %%cr2, %0" : "=r"(faulting_address));

	// The error code gives us details of what happened.
	// page not present?
	// int present = regs->err_code & 0x1;
	// write operation?
	// int rw = regs->err_code & 0x2;
	// processor was in user-mode?
	// int user_mode = regs->err_code & 0x4;
	// overwritten CPU-reserved bits of page entry?
	// int reserved = regs->err_code & 0x8;
	// caused by an instruction fetch?
	// int id = regs->err_code & 0x10;

	// print("page fault handler: %x %d\n", faulting_address, user_mode);
}

void print_page_table(page_table_t *page_table)
{
	for (int i = 0; i < 5; i++)
	{
		print("page number: %d, "
			  "page present: %d, "
			  "page addr: %x, "
			  "page frame: %d\n",
			  i, page_table->entries[i].present,
			  page_table->entries[i].frame * PAGE_SIZE,
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

bool_e create_page_table(u32_t *page_dir, virtual_address vaddr, int flags)
{
	if (page_dir[PD_INDEX(vaddr)] & PAGE_PRESENT)
		return FALSE;

	physical_address pt_table_addr = (physical_address)pmm_alloc_block();

	page_dir[PD_INDEX(vaddr)] = pt_table_addr | flags;

	return TRUE;
}

physical_address create_page_dir()
{
	physical_address pdir = (physical_address)pmm_alloc_block();
	if (!pdir)
		return 0;

	return pdir;
}

physical_address clone_page_table(page_table_t *src __UNUSED__)
{
	return 0x0;
}

physical_address clone_page_dir()
{
	/**
	 * The current page loaded at the cr3 register
	 */
	u32_t *src = (u32_t *)PAGE_DIR;

	physical_address dest = create_page_dir();
	map_page(dest, TEMP_DIR, PAGE_PRESENT | PAGE_WRITE);

	u32_t *dest_vaddr = (u32_t *)TEMP_DIR;

	// recursively map the new page
	dest_vaddr[1023] = dest | PAGE_PRESENT | PAGE_WRITE;

	for (int i = 0; i < 1023; i++)
	{
		/**
		 * Skip blank entries and the temp dir
		 */
		if (src[i] == 0 || i == 1022)
			continue;

		if (i == PD_INDEX(KERNEL_VBASE) || i == PD_INDEX(KERNEL_HEAP))
		{
			/**
			 * Maintain kernel code and heap mappings (i.e. link)
			 */
			dest_vaddr[i] = src[i];
		}
		else
		{
			/**
			 * Map this pages as readonly, so they can cause a page fault
			 * And we can do copy-on-write there
			 */
			dest_vaddr[i] = src[i] & ~PAGE_WRITE;
		}
	}

	invalidate(TEMP_DIR);
	src[PT_INDEX(TEMP_DIR)] = 0;

	return dest;
}

/**
 * Load the page directory into CR3
 * @param pdir page directory
 */
void pd_load(physical_address pdir)
{
	__asm__ __volatile__(
		"mov %0, %%cr3"
		:
		: "r"(pdir)
		: "memory");
}

/**
 * Read the current page directory
 *
 * @return page directory
 */
physical_address pd_read()
{
	physical_address paddr;
	__asm__ __volatile__(
		"mov %%cr3, %0"
		: "=r"(paddr)
		:
		: "memory");
	return paddr;
}

/**
 * Virtual address to physical address
 *
 * @param vaddr virtual address
 *
 * @return physical address
 */
physical_address vtop(virtual_address vaddr)
{
	u32_t *page_dir = (u32_t *)PAGE_DIR;
	u32_t pde = page_dir[PD_INDEX(vaddr)];

	if (pde & PAGE_PRESENT)
	{
		u32_t *page_table = (u32_t *)GET_PAGE_ADDRESS(vaddr);
		u32_t pte = page_table[PT_INDEX(vaddr)];

		if (pte & PAGE_PRESENT)
		{
			/**
			 * Wipe out the upper 12 bits that contain page flags
			 */
			return (u32_t)((pte & 0xFFFFF000) | (vaddr & 0xFFF));
		}
	}

	return 0x0;
}

virtual_address ptov(physical_address paddr)
{
	u32_t *page_dir = (u32_t *)PAGE_DIR;
	u32_t pde = page_dir[PD_INDEX(paddr)];

	if (pde & PAGE_PRESENT)
	{
		u32_t *page_table = (u32_t *)GET_PAGE_ADDRESS(paddr);
		u32_t pte = page_table[PT_INDEX(paddr)];

		if (pte & PAGE_PRESENT)
		{
			return (PD_INDEX(paddr) << 22) | (PT_INDEX(paddr) << 12);
		}
	}

	return 0x0;
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

bool_e alloc_page(page_t *page)
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

bool_e free_page(page_t page __UNUSED__)
{
	// pmm_free_block((void *)page.frame);
	return TRUE;
}

bool_e map_page(physical_address paddr, virtual_address vaddr, int flags)
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
			page_table->entries[PT_INDEX(vaddr)].present = (flags & PAGE_PRESENT) ? 1 : 0;
			page_table->entries[PT_INDEX(vaddr)].read_write = (flags & PAGE_WRITE) ? 1 : 0;
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
		create_page_table(page_dir, vaddr, PAGE_PRESENT | PAGE_WRITE);

		page_table_t *page_table = (page_table_t *)GET_PAGE_ADDRESS(vaddr);

		for (u32_t i = 0; i < 1024; i++)
		{
			if (i == PT_INDEX(vaddr))
			{
				page_table->entries[i].present = (flags & PAGE_PRESENT) ? 1 : 0;
				page_table->entries[i].read_write = (flags & PAGE_WRITE) ? 1 : 0;
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

bool_e unmap_page(virtual_address vaddr)
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
	__asm__ __volatile__("invlpg (%0)" ::"r"(vaddr) : "memory");
}