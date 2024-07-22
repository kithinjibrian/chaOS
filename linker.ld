/* specifies format of final executable binary */

OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
{
	. = 0x100000; /* 1 Mb */

	.text : /*ALIGN(4096)*/
	{
		*(.text)
	}

	.asm :
	{
		*(.asm)
	}

	.data : /*ALIGN(4096)*/
	{
		*(.data)
	}

	.bss : /*ALIGN(4096)*/
	{
		*(.bss)
	}
}