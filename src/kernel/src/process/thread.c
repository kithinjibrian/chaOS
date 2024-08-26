#include "process.h"

DLIST_HEAD(thread_list_g);

size_t next_tid_g = 0;
thread_t *current_thread_g = NULL;

thread_t *create_thread(thread_fun fn, void *arg)
{
	u8_t *sp;
	thread_t *thread = (thread_t *)kmalloc(sizeof(thread_t));

	strncpy(thread->name, arg, strlen(arg));

	thread->tid = next_tid_g++;
	thread->status = READY;
	thread->kernel_stack = (u8_t *)kmalloc(PAGE_SIZE);
	sp = thread->kernel_stack + PAGE_SIZE;

	sp -= sizeof(registers_t);
	thread->regs = (registers_t *)sp;

	sp -= sizeof(context_t);
	thread->context = (context_t *)sp;

	thread->context->edi = 0;
	thread->context->esi = 0;
	thread->context->ebx = 0;
	thread->context->ebp = (u32_t)thread->kernel_stack + PAGE_SIZE;
	thread->context->eip = (u32_t)fn;

	print("thread: %p\n", thread);

	init_dlist_head(&(thread->list));

	dlist_add_tail(&thread_list_g, &(thread->list));

	return thread;
}

thread_t *get_next_thread()
{
	if (current_thread_g)
	{

		return dlist_next_entry_circular(current_thread_g, &thread_list_g, list);
	}

	return NULL;
}

thread_t *get_current_thread()
{
	return current_thread_g;
}

void set_current_thread(thread_t *thread)
{
	current_thread_g = thread;
}