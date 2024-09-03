#include "idt.h"

void *syscalls_g[NUM_SYSCALLS];

int call_syscall(registers_t regs, void *loc)
{
	int ret;

	__asm__ __volatile__(
		"push %1;"
		"push %2;"
		"push %3;"
		"push %4;"
		"push %5;"
		"call *%6;"
		"add $20, %%esp;"
		: "=a"(ret)
		: "r"(regs.edi), "r"(regs.esi), "r"(regs.edx), "r"(regs.ecx), "r"(regs.ebx), "r"(loc)
		: "memory");

	return ret;
}

void syscall_handler(registers_t *regs)
{
	// printf("syscall %d\n", regs.eax);
	if (regs->eax >= NUM_SYSCALLS)
	{
		return;
	}

	// regs.eax = call_syscall(regs, syscalls_g[regs.eax]);
}