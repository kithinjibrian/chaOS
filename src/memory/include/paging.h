#ifndef PAGING_H
#define PAGING_H

#include "../../config/config.h"
#include <stddef.h>
#include <stdbool.h>


#define PAGING_CACHE_DISABLED   0b00010000
#define PAGING_WRITE_THROUGH    0b00001000
#define PAGING_ACCESS_FROM_ALL  0b00000100
#define PAGING_IS_WRITEABLE     0b00000010
#define PAGING_IS_PRESENT       0b00000001

/**
 * paging memory model
 * memory is managed as a bundle of page directories(page table)
 * each page directory has count of pages
 * each page subsequently has its size in bytes
 */
#define PAGING_TOTAL_ENTRIES_PER_TABLE  1024
#define PAGING_PAGE_SIZE                4096

/**
 * chunk of memory to be addressed
 */
struct paging_4gb_chunk
{
    u32_t* directory_entry;
};

struct paging_4gb_chunk* paging_new_4gb(u8_t flags);
void paging_switch(struct paging_4gb_chunk* directory);
void enable_paging();

int paging_set(u32_t* directory, void* virt, u32_t val);
bool paging_is_aligned(void* addr);

u32_t* paging_4gb_chunk_get_directory(struct paging_4gb_chunk* chunk);
void paging_free_4gb(struct paging_4gb_chunk* chunk);

int paging_map_to(struct paging_4gb_chunk *directory, void *virt, void *phys, void *phys_end, int flags);
int paging_map_range(struct paging_4gb_chunk* directory, void* virt, void* phys, int count, int flags);
int paging_map(struct paging_4gb_chunk* directory, void* virt, void* phys, int flags);
void* paging_align_address(void* ptr);
u32_t paging_get(u32_t* directory, void* virt);
void* paging_align_to_lower_page(void* addr);

#endif