#include "mstring.h"

void itoa(int val, char *buf)
{
	int i, sign;

	if ((sign = val) < 0)
		val = -val;
	i = 0;
	do
	{
		buf[i++] = val % 10 + '0';
	} while ((val /= 10) > 0);

	if (sign < 0)
		buf[i++] = '-';

	buf[i] = '\0';
}