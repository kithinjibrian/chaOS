#include "port.h"

unsigned char port_byte_in(unsigned short port)
{
	unsigned char ret;
	__asm__ __volatile__("inb %1, %0" : "=a"(ret) : "dN"(port));
	return ret;
}

void port_byte_out(unsigned short port, unsigned char data)
{
	__asm__ __volatile__("outb %0, %1" : : "a"(data), "dN"(port));
}

unsigned short port_word_in(unsigned short port)
{
	unsigned short ret;
	__asm__ __volatile__("inw %1, %0" : "=a"(ret) : "dN"(port));
	return ret;
}

void port_word_out(unsigned short port, unsigned short data)
{
	__asm__ __volatile__("outw %0, %1" : : "a"(data), "dN"(port));
}
