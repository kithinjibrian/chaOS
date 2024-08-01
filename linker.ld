/* specifies format of final executable binary */

#define init_calls_level(level)		\
	initcall##level##_start_g = .;	\
	*(.initcall##level.init) 

#define INIT_CALLS			\
    initcall_start_g = .;	\
    init_calls_level(0)		\
    init_calls_level(1)		\
    init_calls_level(2)		\
    init_calls_level(3)		\
    init_calls_level(4)		\
    init_calls_level(5)		\
    init_calls_level(6)		\
    init_calls_level(7)		\
    initcall_end_g = .;            


ENTRY(start)

SECTIONS
{
	. = 0x00100000;

	kernel_pstart_g = .;

	.multiboot ALIGN(4K): {
		*(.multiboot)
	}

	.boot ALIGN(4K): {
		*(.boot)
	}

	. += 0xC0000000;

	kernel_vstart_g = 0xC0000000;

	.text ALIGN(4K): AT(ADDR(.text) - 0xC0000000) {
		*(.text)
		*(.rodata)
	}

	.init.text ALIGN(4K): AT(ADDR(.init.text) - 0xC0000000) {
		*(.init.text)
	}

	.exit.text ALIGN(4K): AT(ADDR(.exit.text) - 0xC0000000) {
		*(.exit.text)
	}

	.modinfo ALIGN(4K): AT(ADDR(.modinfo) - 0xC0000000) {
		*(modinfo)
	}

	.initcall.init ALIGN(4K): AT(ADDR(.initcall.init) - 0xC0000000) {
		INIT_CALLS
	}

	.data ALIGN(4K): AT(ADDR(.data) - 0xC0000000) {
		*(.data)
	}


	.bss ALIGN(4K): AT(ADDR(.bss) - 0xC0000000) {
		*(.bss)
	}

	/**
	* Discard unused sections
	*/
	/DISCARD/ : {
		*(.comment)
		*(.eh_frame)
		*(.note.gnu.build-id)
	}

	kernel_vend_g = .;

	kernel_pend_g = . - 0xC0000000;
}