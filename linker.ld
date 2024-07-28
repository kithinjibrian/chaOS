/* specifies format of final executable binary */
/* 
ENTRY(start)
OUTPUT_FORMAT(elf32-i386)

SECTIONS
{
	. = 0x00100000;

	.multiboot ALIGN(4K): {
		*(.multiboot)
	}

	.note.gnu.build-id ALIGN(4K): {
		*(.note.gnu.build-id)
	}

	.boot ALIGN(4K): {
		*(.boot)
	}

	. += 0xC0000000;

	kernel_vstart = .;
	kernel_pstart = . - 0xC0000000;

	.text ALIGN(4K): AT(ADDR(.text) - 0xC0000000) {
		*(.text)
	}

	.rodata ALIGN(4K): AT(ADDR(.rodata) - 0xC0000000) {
		*(.rodata)
	}

	.data ALIGN(4K): AT(ADDR(.data) - 0xC0000000) {
		*(.data)
	}

	.bss ALIGN(4K): AT(ADDR(.bss) - 0xC0000000) {
		*(COMMON)
		*(.bss)
	}

	.asm ALIGN(4K): AT(ADDR(.asm) - 0xC0000000){
		*(.asm)
	}

	kernel_vend = .;
	kernel_pend = . - 0xC0000000;
} */

ENTRY(start)
OUTPUT_FORMAT(elf32-i386)
SECTIONS
{
    . = 1M;
    .text : ALIGN(4096)
    {
        *(.text)
    }

    .asm : ALIGN(4096)
    {
        *(.asm)
    }

    .rodata : ALIGN(4096)
    {
        *(.rodata)
    }

    .data : ALIGN(4096)
    {
        *(.data)
    }

    .bss : ALIGN(4096)
    {
        *(COMMON)
        *(.bss)
    }
}