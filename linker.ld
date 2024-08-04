/* specifies format of final executable binary */

#include "linker.cha"

#define START 0x00100000

ENTRY(start)

SECTIONS
{
	. = START;

	kernel_pstart_g = .;

	.multiboot ALIGN(4K): {
		*(.multiboot)
	}

	.boot ALIGN(4K): {
		*(.boot)
	}

	. += LOAD_OFFSET;
	kernel_vstart_g = LOAD_OFFSET;

	TEXT
	RO_DATA
	INIT_TEXT_SECTION
	EXIT_TEXT_SECTION
	INIT_DATA_SECTION
	RW_DATA
	BSS_SECTION

	kernel_vend_g = .;
	kernel_pend_g = . - LOAD_OFFSET;

	DISCARDS
}