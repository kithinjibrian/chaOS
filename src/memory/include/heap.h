#ifndef HEAP_H
#define HEAP_H

#include "../../config/config.h"
#include "../../config/status.h"

#define HEAP_BLOCK_TABLE_ENTRY_TAKEN    0x01
#define HEAP_BLOCK_TABLE_ENTRY_FREE     0x00

#define HEAP_BLOCK_HAS_NEXT             0b10000000
#define HEAP_BLOCK_IS_FIRST             0b01000000

struct heap_table
{
    u8_t* entries;
    u32_t total;
};


struct heap
{
    struct heap_table* table;
    void* start_address;
};

int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table);
void* heap_malloc(struct heap* heap, u8_t size);
void heap_free(struct heap* heap, void* ptr);

#endif