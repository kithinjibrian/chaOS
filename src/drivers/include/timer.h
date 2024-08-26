#ifndef TIMER_H
#define TIMER_H

#include "idt.h"
#include "dlist.h"
#include "module.h"
#include "kmalloc.h"

typedef void (*fun_timer_cb)(registers_t *);

typedef struct timer
{
	fun_timer_cb handler; // Function pointer to the handler function
	dlist_t list;		  // List node for linking `timer_t` instances
	int delay;			  // Number of ticks to wait before calling the handler
	int remaining_ticks;  // Tracks the remaining ticks before the handler is called
} timer_t;

void set_interval(fun_timer_cb cb, int delay);

#endif