#include "kernel.h"

static u32_t *initcall_levels[] = {
	initcall0_start_g,
	initcall1_start_g,
	initcall2_start_g,
	initcall3_start_g,
	initcall4_start_g,
	initcall5_start_g,
	initcall6_start_g,
	initcall7_start_g,
	initcall_end_g,
};

static void do_one_initcall(u32_t *fn)
{
	initcall_fun *fun = (initcall_fun *)fn;
	(*fun)();
}

static void do_level_initcall(int level)
{
	u32_t *fn;
	for (fn = initcall_levels[level]; fn < initcall_levels[level + 1]; fn++)
		do_one_initcall(fn);
}

void do_initcall()
{
	u32_t *fn;

	int array_size = sizeof(initcall_levels) / sizeof(initcall_levels[0]);

	for (int i = 0; i < array_size - 1; i++)
	{
		do_level_initcall(i);
	}
}