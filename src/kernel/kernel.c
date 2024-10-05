#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <string.h>

#include <kernel/tty.h>
#include <kernel/serial.h>
#include <kernel/log.h>

// Prevent stuff from working if we're not using a cross compiler.
#if defined(__linux__)
#error "You are not using a cross-compiler."
#endif

void kernel_main(void)
{
    // Initialize the terminal interface
    term_init();

    //term_writestring("TTY Initialized!\n");

    kprintl("TTY Initialized!", LOG_SYSTEM);

    int hasSerial = 1;

    kwritel("Initializing COM1 (0x3F8) Serial Port for debugging...", LOG_SYSTEM);
    if (initSerial(0x3F8) == 1)
    {
        kprint("FAIL!", 0x04);
        hasSerial = 0;
    }
    else
    {
        kprint("OK!", 0x02);
        kwritel("Routing TTY output to Serial...", LOG_SYSTEM);
        term_setSerial(0x3F8);
        kprint("OK!", 0x02);
    }
    kprintl("Serial initialization finished!", LOG_INFO);
    if (hasSerial == 0)
        kprintl("NOTE: Debugging through COM1 port is unavailable!", LOG_WARN);

    kprintl("Loading GDT...", LOG_SYSTEM);
    kprintl("Disabling interrupts...\n", LOG_SYSTEM);
    __asm__ volatile ("cli");
}