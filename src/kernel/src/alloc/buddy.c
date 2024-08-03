#include "kmalloc.h"

#define HEADER_SIZE 4

/**
 * Minimum allocation size is 8 bytes because we have a 4 byte header
 * and need to start at a 4 byte boundary.
 */
#define MIN_ALLOC_LOG2 3
#define MIN_ALLOC ((size_t)1 << MIN_ALLOC_LOG2)

#define MAX_ALLOC_LOG2 8
#define MAX_ALLOC ((size_t)1 << MAX_ALLOC_LOG2)

/**
 *
 */
#define BUCKET_COUNT (MAX_ALLOC_LOG2 - MIN_ALLOC_LOG2 + 1)

/**
 * Each bucket corresponds to a certain allocation size and stores
 * a free list that tracks the nodes that are not in use.
 */
static list_t buckets[BUCKET_COUNT];

/**
 * Tracks the size of the binary tree
 */
static size_t bucket_limit;

/**
 * A lineraized binary tree that tracks the nodes that are split.
 * Only a single bit is used to track if the node is split or not
 *
 * Example:
 *
 * [256]
 * |----- [128]
 * |        |---- [64]
 * |		|       |---- [32]
 * |		|       |       |---- [16]
 * |		|		|		|---- [16]
 * |		|		|---- [32]
 * |		|       |       |---- [16]
 * |		|		|		|---- [16]
 * |        |---- [64]
 * |		|       |---- [32]
 * |		|       |       |---- [16]
 * |		|		|		|---- [16]
 * |		|		|---- [32]
 * |		|       |       |---- [16]
 * |		|		|		|---- [16]
 * |----- [128]
 * |        |---- [64]
 * |		|       |---- [32]
 * |		|       |       |---- [16]
 * |		|		|		|---- [16]
 * |		|		|---- [32]
 * |		|       |       |---- [16]
 * |		|		|		|---- [16]
 * |        |---- [64]
 * |		|       |---- [32]
 * |		|       |       |---- [16]
 * |		|		|		|---- [16]
 * |		|		|---- [32]
 * |		|       |       |---- [16]
 * |		|		|		|---- [16]
 *
 * - Move to parent:         index = (index - 1) / 2;
 * - Move to left child:     index = index * 2 + 1;
 * - Move to right child:    index = index * 2 + 2;
 * - Move to sibling:        index = ((index - 1) ^ 1) + 1;
 *
 * Note: we only track parent nodes thus the nodes for 8 bytes aren't
 * included in the tree
 */
static u8_t node_is_split[(1 << (BUCKET_COUNT - 1)) / 8];

/*
 * This is the starting address of the address range for this allocator. Every
 * returned allocation will be an offset of this pointer from 0 to MAX_ALLOC.
 */
static u8_t *base_ptr;

/*
 * This is the maximum address that has ever been used by the allocator. It's
 * used to know when to call "brk" to request more memory from the kernel.
 */
static u8_t *max_ptr;

static int update_max_ptr(u8_t *new_value)
{
	if (new_value > max_ptr)
	{
		max_ptr = new_value;
	}
	return 1;
}

static u8_t *ptr_for_node(size_t index, size_t bucket)
{
	size_t in_block_index = (index - (1 << bucket) + 1);
	return base_ptr + (in_block_index << (MAX_ALLOC_LOG2 - bucket));
}

static size_t node_for_ptr(u8_t *ptr, size_t bucket)
{
	size_t block_index = (ptr - base_ptr) >> (MAX_ALLOC_LOG2 - bucket);
	int index_offset = (1 << bucket) - 1;
	return block_index + index_offset;
}

/*
 * Given the index of a node, this returns the "is split" flag of the parent.
 */
static int parent_is_split(size_t index)
{
	index = (index - 1) / 2;
	return bit_get_v8(node_is_split, index);
}

/*
 * Given the index of a node, this flips the "is split" flag of the parent.
 */
static void flip_parent_is_split(size_t index)
{
	index = (index - 1) / 2;
	bit_flip_v8(node_is_split, index);
}

static size_t bucket_for_request(size_t request)
{
	size_t bucket = BUCKET_COUNT - 1;
	size_t size = MIN_ALLOC;

	while (size < request)
	{
		bucket--;
		size *= 2;
	}

	return bucket;
}

void *kmalloc(size_t size)
{
	size_t original_bucket, bucket;

	if (size + HEADER_SIZE > MAX_ALLOC)
	{
		return NULL;
	}

	if (base_ptr == NULL)
	{
		base_ptr = max_ptr = (u8_t *)KERNEL_HEAP;
		bucket_limit = BUCKET_COUNT - 1;
		update_max_ptr(base_ptr + sizeof(list_t));
		init_list_head(&buckets[BUCKET_COUNT - 1]);

		print("%x\n", base_ptr);

		// list_add_tail(&buckets[BUCKET_COUNT - 1], (list_t *)base_ptr);
	}
	return NULL;
}