#include "module.h"

static void load_module(multiboot_module_t *entry)
{
	char *name = (char *)(entry->mod_start + KERNEL_VIRT_BASE);

	printf("Loading module: %s\n", name);
}

void modules_load()
{
	if (mboot_g->mods_count > 0)
	{
		for (u32_t i = 0; i < mboot_g->mods_count; i++)
		{
			multiboot_module_t *entry = (multiboot_module_t *)((mboot_g->mods_addr + KERNEL_VIRT_BASE) + i * sizeof(multiboot_module_t));
			load_module(entry);
		}
	}
}