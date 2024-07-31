# The boot process
ChaOS uses Grub as the bootloader of choice which simplifies the boot process by a huge magnitude.

Grub searches for the kernel by looking for a magic number (`0x1badb002`) defined in `boot.asm` file.

This file, apart from being the entry point, does a couple of other things.

1. Enable paging.
2. Setup the stack.
3. Boot the kernel(the part written in C lang).


