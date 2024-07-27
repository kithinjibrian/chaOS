#include "../include/kheap.h"
#include "../include/heap.h"
#include "../../config/config.h"
#include "../../config/status.h"

struct heap kernel_heap;
struct heap_table kernel_heap_table;

void kheap_init()
{
    int total_table_entries = CHAOS_HEAP_SIZE_BYTES / CHAOS_HEAP_BLOCK_SIZE;
    kernel_heap_table.entries = (u8_t*)(CHAOS_HEAP_TABLE_ADDRESS);
    kernel_heap_table.total = total_table_entries;

    void* end = (void*)(CHAOS_HEAP_ADDRESS + CHAOS_HEAP_SIZE_BYTES);
    int res = create(&kernel_heap, (void*)(CHAOS_HEAP_ADDRESS), end, &kernel_heap_table);
    if (res < 0)
    {
        print("Failed to create heap\n");
    }

}

void* kmalloc(u32_t size)
{
    return malloc(&kernel_heap, size);
}

void* kzalloc(u32_t size)
{
    void* ptr = kmalloc(size);
    if (!ptr)
        return 0;

    memset(ptr, 0x00, size);
    return ptr;
}

void kfree(void* ptr)
{
    free(&kernel_heap, ptr);
}