#ifndef MODULE_H
#define MODULE_H

#include "type.h"
#include "macro.h"

typedef int (*initcall_fun)(void);
typedef void (*exitcall_fun)(void);

#define __MOD__ __attribute__((__section__(".modinfo")))
#define __INIT__ __attribute__((__section__(".init.text")))
#define __EXIT__ __attribute__((__section__(".exit.text")))

#define define_initcall(fun, level)             \
	static initcall_fun initcall_##fun __USED__ \
		__attribute__((__section__(".initcall" #level ".init"))) = fun;

#define pure_initcall(fn) define_initcall(fn, 0)
#define core_initcall(fn) define_initcall(fn, 1)
#define fs_initcall(fn) define_initcall(fn, 5)
#define device_driver(fun) define_initcall(fun, 6)

#define module_init(fun) device_driver(fun)

#define __module_info__(tag, name, info) \
	static const char unique_id(name)[] __USED__ __MOD__ align(1) = stringify(tag) "=" info;

#define module_info(tag, info) __module_info__(tag, tag, info)

#define module_author(__author__) module_info(author, __author__)

#define module_description(__desc__) module_info(desc, __desc__)

#endif