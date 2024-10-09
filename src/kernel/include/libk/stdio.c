#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>

#include <libk/string.h>
#include <libk/stdio.h>
#include <libk/stdlib.h>

#include <kernel/tty.h>

static bool kprint(const char *data, size_t length)
{
    const unsigned char *bytes = (const unsigned char *)data;

    for (size_t i = 0; i < length; i++)
    {
        if (kputchar(bytes[i]) == EOF)
            return false;
    }
    return true;
}

int kprintf(const char *__restrict format, ...)
{
    va_list parameters;
    va_start(parameters, format);

    int written = 0;

    while (*format != '\0')
    {
        size_t maxrem = INT_MAX - written;

        if (format[0] != '%' || format[1] == '%')
        {
            if (format[0] == '%')
                format++;

            size_t amount = 1;

            while (format[amount] && format[amount] != '%')
                amount++;
            
            if (maxrem < amount)
            {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            
            if (!kprint(format, amount))
                return -1;
            format += amount;
            written += amount;
            
            continue;
        }

        format++;

        // Because C moment i need to put this here
        size_t len;
        char bfr[255];

        switch (*format)
        {
            case 'c':
                format++;

                char c = (char)va_arg(parameters, int);

                len = sizeof(c);

                bfr[0] = c;
                bfr[1] = '\0';

                break;
            case 's':
                format++;

                const char *str = va_arg(parameters, const char*);

                len = kstrlen(str);

                kmemcpy(bfr, str, len);

                break;
            case 'd':
            case 'i':
            {
                format++;

                int n = va_arg(parameters, int);

                kitoa(n, bfr, 10);

                len = kstrlen(bfr);

                break;
            }
            case 'x':
            case 'X':
            {
                static int isCapital;

                if (*format == 'X')
                    isCapital = 1;
                else
                    isCapital = 0;

                format++; 

                int n = va_arg(parameters, int);

                len = 0;

                do
                {
                    int digit = n & 0xF;
                    if (digit < 10)
                        bfr[len++] = '0' + digit;
                    else
                        bfr[len++] = (isCapital ? 'A' : 'a') + (digit - 10);
                    n >>= 4; 
                } while (n);

                if (len == 0)
                    bfr[len++] = '0';

                bfr[len] = '\0';

                for (int i = 0; i < len / 2; i++)
                {
                    char tmp = bfr[i];
                    bfr[i] = bfr[len - i - 1];
                    bfr[len - i - 1] = tmp;
                }
            }
        }

        if (maxrem < len)
        {
            // TODO: Set errno to EOVERFLOW.

            return -1;
        }

        if (!kprint(bfr, len))
            return -1;

        written += len;
    }

    va_end(parameters);
    return written;
}

int kputchar(int c)
{
    // Because this is the LibK version we don't need to worry about how this is handled in userspace.
    // In kernelspace we will always only have the TTY anyway.
    char cc = (char)c;
    term_putchar(cc);
    return c;
}

int kputs(const char *str)
{
    return kprintf("%s\n\r", str);
}