.global gdt_flush
gdt_flush:
	movl 4(%esp), %eax
	lgdtl (%eax)

	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %fs
	movw %ax, %ss

	ljmpl $0x08, $(flush)

flush:
   ret

.global tss_flush
tss_flush:
	movw $((5 * 8) | 0), %ax
	ltrw %ax
	ret