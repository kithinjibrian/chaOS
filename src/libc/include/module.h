#ifndef MODULE_H
#define MODULE_H

#include "vga.h"
#include "type.h"
#include "macro.h"
#include "multiboot.h"

#define KERNEL_VIRT_BASE 0xC0000000

extern multiboot_t *mboot_g;

typedef int (*initcall_fun)(void);
typedef void (*exitcall_fun)(void);

/**
 * Places data in the .modinfo section
 */
#define __MOD__ __attribute__((__section__(".modinfo")))

/**
 * Places a function in the .init.text section
 * Should be used for initialization functions
 */
#define __INIT__ __attribute__((__section__(".init.text")))

/**
 * Places a function in the .exit.text section
 * Should be used for exit functions
 */
#define __EXIT__ __attribute__((__section__(".exit.text")))

#define define_initcall(fun, level)             \
	static initcall_fun initcall_##fun __USED__ \
		__attribute__((__section__(".initcall" #level ".init"))) = fun

#define pure_initcall(fn) define_initcall(fn, 0)
#define core_initcall(fn) define_initcall(fn, 1)
#define fs_initcall(fn) define_initcall(fn, 5)
#define device_driver(fun) define_initcall(fun, 6)

#define module_init(fun) device_driver(fun)

#define __module_info__(tag, name, info) \
	static const char unique_id(name)[] __USED__ __MOD__ align(1) = stringify(tag) "=" info

#define module_info(tag, info) __module_info__(tag, tag, info)

#define module_author(__author__) module_info(author, __author__)

#define module_description(__desc__) module_info(desc, __desc__)

void modules_load();

#endif