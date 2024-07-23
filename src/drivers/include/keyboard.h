#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "vga.h"
#include "port.h"
#include "../../libc/include/type.h"
#include "../../libc/include/mstring.h"

void keyboard_handler(registers_t regs);

#endif