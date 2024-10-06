#include <libk/stdlib.h>

void kitoa(int n, char *buffer, int base)
{
    int i = 0;

    // Handle zero
    if (n == 0)
    {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }

    int isNegative = 0;

    if (n < 0 && base == 10)
    {
        isNegative = 1;
        n = -n;
    }

    // Process individual digits
    while (n != 0)
    {
        int rem = n % base;
        buffer[i++] = (rem > 9) ? (rem - 10) + 'A' : rem + '0';
        n = n / base;
    }

    // Append negative sign for base 10
    if (isNegative)
    {
        buffer[i++] = '-';
    }

    // Terminate string
    buffer[i] = '\0';

    // Reverse the string
    for (int j = 0; j < i / 2; j++)
    {
        char temp = buffer[j];
        buffer[j] = buffer[i-j-1];
        buffer[i-j-1] = temp;
    }
}