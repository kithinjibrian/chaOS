#ifndef LIST_H
#define LIST_H

#include "type.h"

#define SLIST_HEAD_INIT(name) {&(name)}

#define SLIST_HEAD(name) \
	struct slist name = SLIST_HEAD_INIT(name)

/**
 * slist_for_each	-	iterate over a list
 * @param pos:    the &struct list to use as a loop counter.
 * @param head:   the head for your list.
 */
#define slist_for_each(pos, node) \
	for ((pos) = (node); pos; (pos) = (pos)->next)

/**
 * slist_entry - get the struct for this entry
 * @param ptr:    the &struct list pointer.
 * @param type:   the type of the struct this is embedded in.
 * @param member: the name of the list within the struct type.
 */
#define slist_entry(ptr, type, member) \
	container_of(ptr, type, member)

/**
 * dlist_for_each_entry	-	iterate over list of given type
 * @param pos:    the type * to use as a loop counter.
 * @param head:   the head for your list.
 * @param member: the name of the list within the struct type.
 */
#define slist_for_each_entry(pos, node, member)                 \
	for (pos = slist_entry((node), __typeof__(*(pos)), member); \
		 &pos->member != NULL;                                  \
		 pos = slist_entry((pos)->member.next, __typeof__(*(pos)), member))

static inline void init_slist_head(slist_t *list)
{
	list->next = list;
}

/**
 * __slist_add__ - add a nnew entry between two known consecutive entries
 *
 * @param nnew: nnew entry to be added
 * @param prev: previous entry
 * @param next: next entry
 */
static inline void __slist_add__(slist_t *nnew, slist_t *prev, slist_t *next)
{
	nnew->next = next;
	prev->next = nnew;
}

/**
 * slist_add - add a nnew entry
 * @param head: list head to add it before
 * @param nnew: nnew entry to be added
 */
static inline void slist_add(slist_t *head, slist_t *nnew)
{
	__slist_add__(nnew, head, head->next);
}

/**
 * dlist_add_tail - add a nnew entry
 * @param head: list head to add it before
 * @param nnew: nnew entry to be added
 */
static inline void slist_add_tail(slist_t *head, slist_t *nnew)
{
	//__slist_add__(nnew, head->prev, head);
}

/**
 * dlist_del - deletes entry from list by making prev/next entries
 *   point to each other
 *
 */
static inline void __slist_del__(slist_t *prev, slist_t *next)
{
	prev->next = next;
}

/**
 * slist_del - deletes entry from list
 * @param entry: the element to delete from the list
 */
static inline void slist_del(slist_t *entry)
{
	//__slist_del__(entry->prev, entry->next);
}

/**
 * slist_pop - delete previous entry from list and returns it
 *
 */
static inline slist_t *slist_pop(slist_t *list)
{
	// slist_t *prev = list->prev;
	// if (prev == list)
	// 	return NULL;
	// slist_del(prev);
	// return prev;
}

#endif