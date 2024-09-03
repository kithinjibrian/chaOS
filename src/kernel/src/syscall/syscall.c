#include "syscall.h"

void init_syscall()
{
	syscall_reg(0, (void_fun)printf);
}

void syscall_reg(int syscall_id, void_fun handler)
{
	if (syscall_id < NUM_SYSCALLS)
	{
		syscalls_g[syscall_id] = handler;
	}
}
