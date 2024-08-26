#ifndef PROCESS_H
#define PROCESS_H

#include "gdt.h"
#include "type.h"
#include "page.h"
#include "macro.h"
#include "dlist.h"
#include "timer.h"
#include "kmalloc.h"
#include "spinlock.h"

#define THREAD_NAME_LEN 32
#define PROCESS_NAME_LEN 32

typedef void (*thread_fun)(void *);

typedef enum
{
	READY,
	RUNNING,
	DEAD
} status_e;

typedef struct context
{
	u32_t edi, esi, ebx, ebp, eip;
} context_t;

typedef struct thread
{
	u32_t kernel_esp;
	status_e status;
	size_t tid;
	dlist_t list;
	size_t ticks;
	u8_t *kernel_stack;
	struct registers *regs;
	struct context *context;
	char name[THREAD_NAME_LEN];
} thread_t;

typedef struct process
{
	size_t pid;
	physical_address pdir;
	char name[PROCESS_NAME_LEN];
} process_t;

extern int context_switch(void *td, void *td2);

void scheduler(registers_t *regs);

int init_process();

thread_t *get_next_thread();
thread_t *get_current_thread();
process_t *create_process(char *name);
void set_current_thread(thread_t *thread);
thread_t *create_thread(thread_fun fn, void *arg);

void user();
void user2();

#endif