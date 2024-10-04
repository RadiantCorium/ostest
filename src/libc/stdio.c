#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

int puts(const char *string)
{
    return printf("%s\n", string);
}

#if defined(__is_libk)
#include <kernel/tty.h>
#endif

int putchar(int ic)
{
    #if defined(__is_libk)
    char c = (char)ic;
    terminal_write(&c, sizeof(c));
    #else
    // TODO: Implement stdio and the write system call.
    #endif
    return ic;
}

static bool print(const char *data, size_t length)
{
    const unsigned char *bytes = (const unsigned char *)data;

    for (size_t i = 0; i < length; i++)
        if (putchar(bytes[i]) == EOF)
            return false;
    
    return true;
}

int printf(const char *restrict format, ...)
{
    // TODO
}