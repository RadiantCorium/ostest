#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <string.h>

#include <kernel/tty.h>

// Prevent stuff from working if we're not using a cross compiler.
#if defined(__linux__)
#error "You are not using a cross-compiler."
#endif

void kernel_main(void)
{
    // Initialize the terminal interface
    term_init();

    int i = 0;

    while (1)
    {
        if (i == 50000000)
            term_writestring("\nOne");
        if (i == 100000000)
            term_writestring("\nTwo");
        if (i == 150000000)
        {
            term_writestring("\nThree");
            i = 0;
        }

        i++;
    }
}