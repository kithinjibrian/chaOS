#include "port.h"

/**
 * Reads a byte from the specified port
 * @param port The port to read from
 */
unsigned char port_byte_in(unsigned short port)
{
	unsigned char ret;
	__asm__ __volatile__("inb %1, %0" : "=a"(ret) : "dN"(port));
	return ret;
}

/**
 * Writes a byte to the specified port
 * @param port The port to write to
 * @param data The data to write
 */
void port_byte_out(unsigned short port, unsigned char data)
{
	__asm__ __volatile__("outb %0, %1" : : "a"(data), "dN"(port));
}

/**
 * Reads a word from the specified port
 * @param port The port to read from
 */
unsigned short port_word_in(unsigned short port)
{
	unsigned short ret;
	__asm__ __volatile__("inw %1, %0" : "=a"(ret) : "dN"(port));
	return ret;
}

/**
 * Writes a word to the specified port
 * @param port The port to write to
 * @param data The data to write
 */
void port_word_out(unsigned short port, unsigned short data)
{
	__asm__ __volatile__("outw %0, %1" : : "a"(data), "dN"(port));
}
