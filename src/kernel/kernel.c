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

    term_writestring("TTY Initialized!\n");

    term_writestring("Initializing COM1 serial port for debugging...");
}