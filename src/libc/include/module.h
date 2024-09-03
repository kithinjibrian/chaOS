#ifndef MODULE_H
#define MODULE_H

#include "io.h"
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
#define __MOD__ section(".modinfo")

/**
 * Places a function in the .init.text section
 * Should be used for initialization functions
 */
#define __INIT__ section(".init.text") __COLD__

/**
 * Places a function in the .exit.text section
 * Should be used for exit functions
 */
#define __EXIT__ section(".exit.text")

#define define_initcall(fun, level)             \
	static initcall_fun initcall_##fun __USED__ \
		section(".initcall" #level ".init") = fun

#define early_initcall(fn) define_initcall(fn, early)

#define pure_initcall(fn) define_initcall(fn, 0)
#define core_initcall(fn) define_initcall(fn, 1)
#define arch_initcall(fn) define_initcall(fn, 3)
#define fs_initcall(fn) define_initcall(fn, 5)
#define device_driver(fun) define_initcall(fun, 6)

#define module_init(fun) device_driver(fun)

#define module_exit(fun)               \
	static exitcall_fun exitcall_##fun \
		__UNUSED__ section(".exitcall.exit") = fun

#define __module_info__(tag, name, info) \
	static const char unique_id(name)[] __USED__ __MOD__ align(1) = stringify(tag) "=" info

#define module_info(tag, info) __module_info__(tag, tag, info)

#define module_author(__author__) module_info(author, __author__)

#define module_description(__desc__) module_info(desc, __desc__)

void modules_load();

#endif