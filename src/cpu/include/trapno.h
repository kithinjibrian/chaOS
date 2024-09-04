#ifndef TRAPNO_H
#define TRAPNO_H

#define TRAP_DE 0		 /* Divide-by-zero */
#define TRAP_DB 1		 /* Debug */
#define TRAP_NMI 2		 /* Non-maskable Interrupt */
#define TRAP_BP 3		 /* Breakpoint */
#define TRAP_OF 4		 /* Overflow */
#define TRAP_BR 5		 /* Bound Range Exceeded */
#define TRAP_UD 6		 /* Invalid Opcode */
#define TRAP_NM 7		 /* Device Not Available */
#define TRAP_DF 8		 /* Double Fault */
#define TRAP_OLD_MF 9	 /* Coprocessor Segment Overrun */
#define TRAP_TS 10		 /* Invalid TSS */
#define TRAP_NP 11		 /* Segment Not Present */
#define TRAP_SS 12		 /* Stack Segment Fault */
#define TRAP_GP 13		 /* General Protection Fault */
#define TRAP_PF 14		 /* Page Fault */
#define TRAP_SPURIOUS 15 /* Spurious Interrupt */
#define TRAP_MF 16		 /* x87 Floating-Point Exception */
#define TRAP_AC 17		 /* Alignment Check */
#define TRAP_MC 18		 /* Machine Check */
#define TRAP_XF 19		 /* SIMD Floating-Point Exception */
#define TRAP_VE 20		 /* Virtualization Exception */
#define TRAP_CP 21		 /* Control Protection Exception */
#define TRAP_VC 29		 /* VMM Communication Exception */
#define TRAP_IRET 32	 /* IRET Exception */

#endif