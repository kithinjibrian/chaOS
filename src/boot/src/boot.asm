KERNEL_VBASE equ 0xC0000000	; Kernel is placed at 3gb
KERNEL_PAGE_NUM equ (KERNEL_VBASE >> 22) ; The page that contains the kernel. page 768

MBOOT_PAGE_ALIGN equ 1 << 0
MBOOT_MEM_INFO equ 1 << 1
MBOOT_USE_GFX equ 0 ; to turn on when building graphics mode

MBOOT_MAGIC equ 0x1badb002
MBOOT_FLAGS equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO | MBOOT_USE_GFX
MBOOT_CHECKSUM equ -(MBOOT_MAGIC + MBOOT_FLAGS)

section .multiboot
	align 4
	dd MBOOT_MAGIC
	dd MBOOT_FLAGS
	dd MBOOT_CHECKSUM
	dd 0, 0, 0, 0, 0

	dd 0 ; near graphic mode
	dd 800 ; screen width
	dd 600 ; screen height
	dd 32 ; screen depth

section .bss
align 16
stack_bottom:
	resb 16384 ; reserve 16 KB stack on a 16-byte boundary
stack_top:

section .boot
global start
start:
	; NOTE: Until paging is set up, the code must be position-independent and use physical
    ; addresses, not virtual ones!
	mov ecx, (initial_page_dir - KERNEL_VBASE)
	mov cr3, ecx	; Load Page Directory Base Register.

	mov ecx, cr4
	or ecx, 0x00000010	; Set PSE bit in CR4 to enable 4MB pages.
	mov cr4, ecx

	mov ecx, cr0
	or ecx, 0x80000000	 ; Set PG bit in CR0 to enable paging.
	mov cr0, ecx

    ; Start fetching instructions in kernel space.
    ; Since eip at this point holds the physical address of this command (approximately 0x00100000)
    ; we need to do a long jump to the correct virtual address of StartInHigherHalf which is
    ; approximately 0xC0100000.
	jmp higher_half

section .text
extern main
higher_half:
    ; Unmap the identity-mapped first 4MB of physical address space. It should not be needed
    ; anymore.
	mov dword [initial_page_dir], 0
	invlpg [0]

	; set up the stack
	mov esp, stack_top

	; pass Multiboot info structure -- WARNING: This is a physical address and may not be
    ; in the first 4MB!
	; To correct that, we can translate the address of the multiboot struct to virtual address
	; By adding the kernel base address
	; All addresses within the multiboot info will still have to be remapped
	add ebx, KERNEL_VBASE
	push ebx

	; pass Multiboot magic number
	push eax
	xor ebp, ebp
	call main

; Avoid the CPU from executuing rogue instructions
halt:
	hlt
	jmp halt


section .data
align 4096
global initial_page_dir
initial_page_dir:
    ; This page directory entry identity-maps the first 4MB of the 32-bit physical address space.
    ; All bits are clear except the following:
    ; bit 7: PS The kernel page is 4MB.
    ; bit 1: RW The kernel page is read/write.
    ; bit 0: P  The kernel page is present.
    ; This entry must be here -- otherwise the kernel will crash immediately after paging is
    ; enabled because it can't fetch the next instruction! It's ok to unmap this page later
	dd 0x00000083
	times (KERNEL_PAGE_NUM - 1) dd 0	; Pages before kernel space.
	; This page directory entry defines a 4MB page containing the kernel.
	dd 0x00000083
	dd 0x00000083

	times (1024 - KERNEL_PAGE_NUM - 2) dd 0 ; Pages after the kernel image.
