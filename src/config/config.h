#ifndef CONFIG_H
#define CONFIG_H

/* global configurations defined here */

typedef unsigned char u8_t;
typedef unsigned int u32_t;
typedef unsigned short u16_t;

#define CHAOS_HEAP_ADDRESS        0x01000000 
#define CHAOS_HEAP_TABLE_ADDRESS  0x00007E00

// 100MB heap size
#define CHAOS_HEAP_SIZE_BYTES     104857600
#define CHAOS_HEAP_BLOCK_SIZE     4096
#define CHAOS_HEAP_ADDRESS        0x01000000 
#define CHAOS_HEAP_TABLE_ADDRESS  0x00007E00

#endif