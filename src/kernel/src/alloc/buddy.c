#include "kmalloc.h"

#define HEADER_SIZE 4

/**
 * Minimum allocation size is 8 bytes because we have a 4 byte header
 * and need to start at a 4 byte boundary.
 */
#define MIN_ALLOC_LOG2 3
#define MIN_ALLOC ((size_t)1 << MIN_ALLOC_LOG2)

/**
 * Maximum allocation size is 256 MiB because we reserved the address range
 * 0xD0000000 - 0xE0000000 for the kernel heap
 */
#define MAX_ALLOC_LOG2 28
#define MAX_ALLOC ((size_t)1 << MAX_ALLOC_LOG2)

/**
 *
 */
#define BUCKET_COUNT (MAX_ALLOC_LOG2 - MIN_ALLOC_LOG2 + 1)

/**
 * Each bucket corresponds to a certain allocation size and stores
 * a free list that tracks the nodes that are not in use.
 */
static list_t buckets_g[BUCKET_COUNT];

/**
 * Tracks the size of the binary tree
 */
static size_t bucket_limit_g;

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
u8_t node_is_split_g[(1 << (BUCKET_COUNT - 1)) / 8];

/*
 * This is the starting address of the address range for this allocator. Every
 * returned allocation will be an offset of this pointer from 0 to MAX_ALLOC.
 */
static u8_t *base_ptr_g;

/*
 * This is the maximum address that has ever been used by the allocator. It's
 * used to know when to call "brk" to request more memory from the kernel.
 */
static u8_t *max_ptr_g;

static int update_max_ptr(u8_t *new_value)
{
	if (new_value > max_ptr_g)
	{
		if (get_page((virtual_address)new_value))
		{
			print("max_ptr_g: %x\n", new_value);
		}
		else
		{
			print("not mapped max_ptr_g: %x\n", new_value);
		}
		max_ptr_g = new_value;
	}
	return 1;
}

static u8_t *ptr_for_node(size_t index, size_t bucket)
{
	size_t in_block_index = (index - (1 << bucket) + 1);
	return base_ptr_g + (in_block_index << (MAX_ALLOC_LOG2 - bucket));
}

static size_t node_for_ptr(u8_t *ptr, size_t bucket)
{
	size_t block_index = (ptr - base_ptr_g) >> (MAX_ALLOC_LOG2 - bucket);
	int index_offset = (1 << bucket) - 1;
	return block_index + index_offset;
}

/*
 * Given the index of a node, this returns the "is split" flag of the parent.
 */
static int parent_is_split(size_t index)
{
	index = (index - 1) / 2;
	return bit_get_v8(node_is_split_g, index);
}

/*
 * Given the index of a node, this flips the "is split" flag of the parent.
 */
static void flip_parent_is_split(size_t index)
{
	index = (index - 1) / 2;
	bit_flip_v8(node_is_split_g, index);
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

static int lower_bucket_limit(size_t bucket)
{

	while (bucket < bucket_limit_g)
	{
		u8_t *right_child;
		size_t root = node_for_ptr(base_ptr_g, bucket_limit_g);

		size_t parent = (root - 1) / 2;
		if (!bit_get_v8(node_is_split_g, parent))
		{
			list_del((list_t *)base_ptr_g);
			init_list_head(&buckets_g[--bucket_limit_g]);
			list_add_tail(&buckets_g[bucket_limit_g], (list_t *)base_ptr_g);
			continue;
		}

		right_child = ptr_for_node(root + 1, bucket_limit_g);
		if (!update_max_ptr(right_child + sizeof(list_t)))
		{
			return 0;
		}

		list_add_tail(&buckets_g[bucket_limit_g], (list_t *)right_child);
		init_list_head(&buckets_g[--bucket_limit_g]);

		root = (root - 1) / 2;
		if (root != 0)
		{
			flip_parent_is_split(root);
		}
	}

	return 1;
}

void *kmalloc(size_t size)
{
	size_t original_bucket, bucket;

	if (size + HEADER_SIZE > MAX_ALLOC)
	{
		return NULL;
	}

	if (base_ptr_g == NULL)
	{
		base_ptr_g = max_ptr_g = (u8_t *)KERNEL_HEAP;
		bucket_limit_g = BUCKET_COUNT - 1;
		update_max_ptr(base_ptr_g + sizeof(list_t));
		init_list_head(&buckets_g[BUCKET_COUNT - 1]);
		list_add_tail(&buckets_g[BUCKET_COUNT - 1], (list_t *)base_ptr_g);
	}

	bucket = bucket_for_request(size + HEADER_SIZE);
	original_bucket = bucket;

	while (bucket + 1 != 0)
	{
		size_t length, bytes_needed, i;
		u8_t *ptr;

		if (!lower_bucket_limit(bucket))
		{
			return NULL;
		}

		ptr = (u8_t *)list_pop(&buckets_g[bucket]);
		if (!ptr)
		{
			if (bucket != bucket_limit_g || bucket == 0)
			{
				bucket--;
				continue;
			}

			if (!lower_bucket_limit(bucket - 1))
			{
				return NULL;
			}

			ptr = (u8_t *)list_pop(&buckets_g[bucket]);
		}

		length = (size_t)1 << (MAX_ALLOC_LOG2 - bucket);
		bytes_needed = bucket < original_bucket ? length / 2 + sizeof(list_t) : length;

		if (!update_max_ptr(ptr + bytes_needed))
		{
			list_add_tail(&buckets_g[bucket], (list_t *)ptr);
			return NULL;
		}

		i = node_for_ptr(ptr, bucket);
		if (i != 0)
		{
			flip_parent_is_split(i);
		}

		while (bucket < original_bucket)
		{
			i = i * 2 + 1;
			bucket++;
			flip_parent_is_split(i);

			list_add_tail(&buckets_g[bucket], (list_t *)ptr_for_node(i + 1, bucket));
		}

		*(size_t *)ptr = size;
		return ptr + HEADER_SIZE;
	}

	return NULL;
}

void kfree(void *ptr)
{
	size_t bucket, i;

	if (!ptr)
	{
		return;
	}

	ptr = (u8_t *)ptr - HEADER_SIZE;
	bucket = bucket_for_request(*(size_t *)ptr + HEADER_SIZE);

	i = node_for_ptr((u8_t *)ptr, bucket);

	while (i != 0)
	{
		flip_parent_is_split(i);

		if (parent_is_split(i) || bucket == bucket_limit_g)
		{
			break;
		}

		list_del((list_t *)ptr_for_node(((i - 1) ^ 1) + 1, bucket));
		i = (i - 1) / 2;
		bucket--;
	}

	list_add(&buckets_g[bucket], (list_t *)ptr_for_node(i, bucket));
}