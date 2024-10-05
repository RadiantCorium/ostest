#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <string.h>

#include <kernel/tty.h>
#include <kernel/serial.h>

// Prevent stuff from working if we're not using a cross compiler.
#if defined(__linux__)
#error "You are not using a cross-compiler."
#endif

void kernel_main(void)
{
    // Initialize the terminal interface
    term_init();

    term_writestring("TTY Initialized!\n");

    int hasSerial = 1;

    term_writestring("Initializing COM1 (0x3F8) Serial Port for debugging...");
    if (initSerial(0x3F8) == 1)
    {
        term_writestring("FAIL!\n");
        hasSerial = 0;
    }
    else
    {
        term_writestring("OK!\n");
        term_writestring("Routing TTY output to Serial...");
        term_setSerial(0x3F8);
        term_writestring("OK!\n");
    }
    term_writestring("Serial initialization finished!\n");
    if (hasSerial == 0)
        term_writestring("NOTE: Debugging through COM1 port is unavailable!\n");
}