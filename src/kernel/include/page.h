#ifndef PAGE_H
#define PAGE_H

#include "pmm.h"

#include "../../cpu/include/idt.h"
#include "../../drivers/include/vga.h"
#include "../../libc/include/multiboot.h"

#define PAGE_SIZE 0x1000

#define PAGE_DIR 0xFFFFF000
#define TEMP_DIR 0xFFFFE000
#define KERNEL_DIR 0xFFF00000
#define KERNEL_HEAP 0xE0000000
#define KERNEL_VBASE 0xC0000000
#define PAGE_FIRST_DIR 0xFFC00000
#define KERNEL_LOAD_ADDR 0x00100000

#define ALIGN __attribute__((aligned(0x1000)))

#define PD_INDEX(x) (((x) >> 22) & 0x3FF)
#define PT_INDEX(x) (((x) >> 12) & 0x3FF)
#define GET_PHYSICAL_ADDRESS(addr) (*addr & ~0xFFF)

#define GET_PAGE_ADDRESS(vaddr) (PAGE_FIRST_DIR + (PD_INDEX(vaddr) * PAGE_SIZE))

#define PAGE_PRESENT (1 << 0)
#define PAGE_WRITE (1 << 1)
#define PAGE_PS (1 << 7)

/**
 * Trick compiler into thinking this is an array
 * so that we can access the address without using &(reference)
 *
 * Defined in linker.ld file
 */
extern u32_t kernel_vend_g[];
extern u32_t kernel_pend_g[];
extern u32_t kernel_vstart_g[];
extern u32_t kernel_pstart_g[];

/**
 * Defined in kmalloc.c file
 */

/**
 * Defined in boot.asm file
 */
extern u32_t initial_page_dir[1024];

typedef u32_t virtual_address;
typedef u32_t physical_address;
typedef u32_t pagetable_address;

typedef struct page
{
	u32_t present : 1;
	u32_t read_write : 1;
	u32_t user : 1;
	u32_t write_through : 1;
	u32_t cache : 1;
	u32_t accessed : 1;
	u32_t dirty : 1;
	u32_t page_size : 1;
	u32_t global : 1;
	u32_t available : 3;
	u32_t frame : 20;
} page_t;

typedef struct page_dir
{
	u32_t present : 1;
	u32_t read_write : 1;
	u32_t user : 1;
	u32_t write_through : 1;
	u32_t cache : 1;
	u32_t accessed : 1;
	u32_t dirty : 1;
	u32_t page_size : 1;
	u32_t global : 1;
	u32_t available : 3;
	u32_t frame : 20;
} page_dir_t;

typedef struct page_table
{
	page_t entries[1024];
} page_table_t;

typedef struct page_dir_table
{
	page_dir_t entries[1024];
} page_dir_table_t;

int init_page(void);
void print_page_dir();
void invalidate(u32_t vaddr);
bool_e alloc_page(page_t *page);
bool_e unmap_page(virtual_address vaddr);
page_t *get_page(virtual_address vaddr);
void print_page_table(page_table_t *page_table);
bool_e map_page(physical_address paddr, virtual_address vaddr, int flags);

#endif