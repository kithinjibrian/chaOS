#ifndef KERNEL_H
#define KERNEL_H

#include "../../cpu/include/gdt.h"
#include "../../cpu/include/idt.h"
#include "../../cpu/include/timer.h"
#include "../../drivers/include/vga.h"

void main(void);

#endif