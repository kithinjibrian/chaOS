#include "kernel.h"

void panic(const char *msg)
{
	print((char *)msg);
}

void main(void)
{
	clear();
	print("into kernel");
	init_gdt();
	init_idt();
	irq_reg_handler(1, &keyboard_handler);

	int x = 10;
	int arr[x];
	memset(arr, 3, x * sizeof(arr[0]));
	for(int i = 0; i < x; ++i)
	{
		print(" %d,", arr[i]);
	}
	while (1)
		;
}