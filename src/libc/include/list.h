#ifndef LIST_H
#define LIST_H

#include "type.h"

#define LIST_HEAD_INIT(name) {&(name), &(name)}

#define LIST_HEAD(name) \
	struct list name = LIST_HEAD_INIT(name)

/**
 * list_for_each	-	iterate over a list
 * @pos:    the &struct list to use as a loop counter.
 * @head:   the head for your list.
 */
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * list_entry - get the struct for this entry
 * @ptr:    the &struct list pointer.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list within the struct type.
 */
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

/**
 * list_for_each_entry	-	iterate over list of given type
 * @pos:    the type * to use as a loop counter.
 * @head:   the head for your list.
 * @member: the name of the list within the struct type.
 */
#define list_for_each_entry(pos, head, member)                 \
	for (pos = list_entry((head)->next, typeof(*pos), member); \
		 &pos->member != (head);                               \
		 pos = list_entry(pos->member.next, typeof(*pos), member))

static inline void INIT_LIST_HEAD(list_t *list)
{
	list->next = list;
	list->prev = list;
}

/**
 * __list_add__ - add a new entry between two known consecutive entries
 * 
 * @new: new entry to be added
 * @prev: previous entry
 * @next: next entry
 */
static inline void __list_add__(list_t *new, list_t *prev, list_t *next)
{
	new->next = next;
	new->prev = prev;
	prev->next = new;
	next->prev = new;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 */
static inline void list_add(list_t *new, list_t *head)
{
	__list_add__(new, head, head->next);
}

/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 */
static inline void list_add_tail(list_t *new, list_t *head)
{
	__list_add__(new, head->prev, head);
}

/**
 * list_del - deletes entry from list
 */
static inline void __list_del__(list_t *prev, list_t *next)
{
	next->prev = prev;
	prev->next = next;
}

#endif