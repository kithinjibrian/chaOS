#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include "type.h"
#include "macro.h"

typedef struct multiboot_aout_symtable
{
	u32_t tabsize;
	u32_t strsize;
	u32_t addr;
	u32_t reserved;
} multiboot_aout_symtable_t;

typedef struct multiboot_elf_section_header_table
{
	u32_t num;
	u32_t size;
	u32_t addr;
	u32_t shndx;
} multiboot_elf_section_header_table_t;

typedef struct multiboot
{
	/*multiboot info version number*/
	u32_t flags;

	/*available memory from BIOS*/
	u32_t mem_lower;
	u32_t mem_upper;

	/* "root" partition */
	u32_t boot_device;

	/*kernel command line*/
	u32_t cmdline;

	/*boot modules list*/
	u32_t mods_count;
	u32_t mods_addr;

	union
	{
		multiboot_aout_symtable_t aout_sym;
		multiboot_elf_section_header_table_t elf_sec;
	} u;

	/*mmap buffer*/
	u32_t mmap_length;
	u32_t mmap_addr;

	/*drive info buffer*/
	u32_t drives_length;
	u32_t drives_addr;

	/*ROM configuration table*/
	u32_t config_table;

	/*boot loader name*/
	u32_t boot_loader_name;

	/*APM table*/
	u32_t apm_table;

	/*video */
	u32_t vbe_control_info;
	u32_t vbe_mode_info;
	u16_t vbe_mode;
	u16_t vbe_interface_seg;
	u16_t vbe_interface_off;
	u16_t vbe_interface_len;

	u32_t framebuffer_addr_low;
	u32_t framebuffer_addr_high;
	u32_t framebuffer_pitch;
	u32_t framebuffer_width;
	u32_t framebuffer_height;
	u8_t framebuffer_bpp;

#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED 0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB 1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT 2
	u8_t framebuffer_type;
	union
	{
		struct
		{
			u32_t framebuffer_palette_addr;
			u16_t framebuffer_palette_num;
		};
		struct
		{
			u8_t framebuffer_red_field_pos;
			u8_t framebuffer_red_mask_size;
			u8_t framebuffer_green_field_pos;
			u8_t framebuffer_green_mask_size;
			u8_t framebuffer_blue_field_pos;
			u8_t framebuffer_blue_mask_size;
		};
	};
} multiboot_t;

typedef struct multiboot_mmap_entry
{
	u32_t size;
	u32_t addr_low;
	u32_t addr_high;
	u32_t len_low;
	u32_t len_high;

#define MULTIBOOT_MEMORY_AVAILABLE 1
#define MULTIBOOT_MEMORY_RESERVED 2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE 3
#define MULTIBOOT_MEMORY_NVS 4
#define MULTIBOOT_MEMORY_BADRAM 5
	u32_t type;
} __PACKED__ multiboot_mmap_entry_t;

typedef struct multiboot_module
{
	u32_t mod_start;
	u32_t mod_end;
	u32_t string;
	u32_t reserved;
} __PACKED__ multiboot_module_t;

#endif