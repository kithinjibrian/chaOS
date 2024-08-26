.global context_switch
context_switch:
    # body
	movl 4(%esp), %eax	# Put the first(old) parameter into eax (context_t)
	movl 8(%esp), %edx	# Put the second(new) parameter in eax (context_t)

	cmp $0, %eax		# Check if old is null
	jne read_regs		# Jump if not equal
	jmp load_regs 		# if null jump to loading registers

read_regs:
	pushl %ebp
	pushl %ebx
	pushl %esi
	pushl %edi

	movl (%eax), %esp

load_regs:
	movl %edx, %esp
	popl %edi
	popl %esi
	popl %ebx
	popl %ebp
	ret
