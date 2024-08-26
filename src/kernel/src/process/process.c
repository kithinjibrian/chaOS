#include "process.h"

size_t next_pid_g = 0;
process_t *processes_list_g, *current_process_g;
SPINLOCK_INIT(process_lock_g);

void scheduler(registers_t *regs __UNUSED__)
{
	thread_t *current = get_current_thread();
	thread_t *next = get_next_thread();
	if (current)
	{
		set_current_thread(next);
		context_switch(&current->context, next->context);
	}
}

int init_process()
{
	set_interval(scheduler, 10);
	return 0;
}

process_t *create_process(char *name)
{
	process_t *process = (process_t *)kmalloc(sizeof(process_t));

	strncpy(process->name, name, strlen(name));
	process->pid = next_pid_g++;

	return process;
}

void user()
{
	u32_t n = 0;
	while (n < 500)
	{
		n++;
	}
	print("kola: %d\n", n);
}

void user2()
{
	u32_t n = 0;
	while (n < 400)
	{
		n++;
	}
	print("kola kola kola: %d\n", n);
}