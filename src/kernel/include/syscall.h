#ifndef SYSCALL_H
#define SYSCALL_H

#include "idt.h"
#include "type.h"

extern void_fun syscalls_g[NUM_SYSCALLS];

void init_syscall();
void syscall_reg(int syscall_id, void_fun handler);

#endif