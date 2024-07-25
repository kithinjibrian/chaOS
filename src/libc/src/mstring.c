#include "mstring.h"

void reverse(char str[], int length)
{
	int start = 0;
	int end = length - 1;
	while (start < end)
	{
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
}

/**
 * Convert integer to ASCII string
 *
 * @param num number to convert
 * @param str destination string
 * @param base base of the number
 */
void itoa(int num, char *str, int base)
{
	int i = 0;
	int isNegative = 0;

	// Handle 0 explicitly, otherwise empty string is printed for 0
	if (num == 0)
	{
		str[i++] = '0';
		str[i] = '\0';
		return;
	}

	// In standard itoa(), negative numbers are only handled if base is 10
	if (num < 0 && base == 10)
	{
		isNegative = 1;
		num = -num;
	}

	// Process individual digits
	while (num != 0)
	{
		int rem = num % base;
		str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num = num / base;
	}

	// If number is negative, append '-'
	if (isNegative)
	{
		str[i++] = '-';
	}

	str[i] = '\0'; // Append string terminator

	// Reverse the string
	reverse(str, i);
}

void htoa(u32_t hex, char *str)
{
	const char hexDigits[] = "0123456789ABCDEF";
	int i = 0;

	// Add '0x' prefix
	str[i++] = '0';
	str[i++] = 'x';

	// Convert hex value to string from highest nibble to lowest
	int shift = (sizeof(unsigned int) * 8) - 4; // Start with the highest nibble
	int leading = 1;							// Flag to skip leading zeros

	while (shift >= 0)
	{
		int digit = (hex >> shift) & 0xF; // Extract each nibble

		if (digit != 0 || !leading)
		{ // Skip leading zeros
			str[i++] = hexDigits[digit];
			leading = 0;
		}

		shift -= 4;
	}

	// If all digits are zeros (hex is zero), add a single zero
	if (leading)
	{
		str[i++] = '0';
	}

	str[i] = '\0'; // Null-terminate the string
}

