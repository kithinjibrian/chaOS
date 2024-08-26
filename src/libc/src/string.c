#include "string.h"

static void reverse(char str[], int length)
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

/**
 * Return length of the string
 * @param str string
 *
 * @return length of the string
 */
size_t strlen(const char *str)
{
	size_t len = 0;
	while (*str++)
	{
		len++;
	}
	return len;
}

/**
 * Compare two strings
 * @param str1 first string
 * @param str2 second string
 *
 * @return TRUE if the strings are equal, FALSE otherwise
 */
bool_e strncmp(const char *str1, const char *str2, size_t len)
{
	for (size_t i = 0; i < len; i++)
	{
		if (str1[i] != str2[i])
		{
			return FALSE;
		}
	}
	return TRUE;
}

/**
 * Find the first occurrence of the character in the string
 * @param str string
 * @param c asci character
 *
 * @return pointer to the first occurrence of the character
 * in the string or NULL if not found
 */
char *strchr(const char *str, int c)
{
	char ch = (char)c;

	while (*str != '\0')
	{
		if (*str == ch)
		{
			return (char *)str;
		}
		str++;
	}

	if (ch == '\0')
	{
		return (char *)str;
	}

	return NULL;
}

/**
 * Copy a string
 * @param dest destination string
 * @param src source string
 *
 * @return pointer to the destination string
 */
char *strncpy(char *dest, const char *src, size_t n)
{
	size_t i;

	for (i = 0; i < n && src[i] != '\0'; i++)
	{
		dest[i] = src[i];
	}

	// Pad the rest of the destination string with null characters
	for (; i < n; i++)
	{
		dest[i] = '\0';
	}

	return dest;
}
