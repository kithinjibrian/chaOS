#include "mstring.h"

/**
 * Convert integer to ASCII string
 *
 * @param num number to convert
 * @param str destination string
 * @param base base of the number
 */
void itoa(int num, char *str, int base)
{
	/**
	 * Handle special case for base 10 (negative numbers)
	 */
	if (base == 10 && num < 0)
	{
		/**
		 * Convert negative number to positive for processing
		 */
		int temp = -num;
		num = temp;
		// Add minus sign
		str[0] = '-';
		str++;
	}

	/**
	 * Convert number to string
	 * Buffer for the number representation
	 */
	char buffer[33];
	int i = 0;

	/**
	 * Handle 0 explicitly, otherwise empty string is produced
	 */
	if (num == 0)
	{
		str[i++] = '0';
	}
	else
	{
		while (num != 0)
		{
			int digit = num % base;
			if (digit < 10)
			{
				buffer[i++] = digit + '0';
			}
			else
			{
				buffer[i++] = digit - 10 + 'a'; // For bases > 10
			}
			num /= base;
		}
	}

	/**
	 * Append null terminator
	 */
	buffer[i] = '\0';

	/**
	 *  Reverse the string
	 */
	for (int j = 0; j < i; j++)
	{
		str[j] = buffer[i - j - 1];
	}
	str[i] = '\0';
}