#include "linkage.h"

.macro SAVE_ALL
    pushl %fs
    pushl %es
	pushl %ds
	pushl %eax
	pushl %ecx
	pushl %edx
	pushl %ebx
	pushl %esp
	pushl %ebp
	pushl %esi
	pushl %edi
.endm

.macro RESTORE_INT_REGS
	popl %edi
	popl %esi
	popl %ebp
	popl %esp
	popl %ebx
	popl %edx
	popl %ecx
	popl %eax
.endm

.macro RESTORE_REGS pop=0
	RESTORE_INT_REGS
	popl %ds
	popl %es
	popl %fs
	addl $(4 + \pop), %esp /* Pop the error code and GS slot */
.endm

.macro idtentry vector asmsym cfunc has_error_code:req
SYM_CODE_START(\asmsym)
	cld
	.if \has_error_code == 0
		pushl	$0		/* Clear the error code */
	.endif

	/* Push the C-function address into the GS slot */
	pushl	$\cfunc
	/* Invoke the common exception entry */
	jmp	handle_exception
.endm

SYM_CODE_START_LOCAL_NOALIGN(handle_exception)
	SAVE_ALL
	movl	11*4(%esp), %edi /* Get the C function address from the GS slot */
	push	%esp 		/* Push the current stack pointer (struct register) */
	call	*%edi 		/* Call the C function */
	addl	$4, %esp

handle_exception_return:
	RESTORE_REGS 4
	/*  pop cs, eip, eflags, user_ss, and user_esp */
	iret

#include "idtentry.h"



