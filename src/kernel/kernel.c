#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <libk/string.h>
#include <libk/stdio.h>

#include <kernel/tty.h>
#include <kernel/serial.h>
#include <kernel/log.h>

#include <kernel/kpanic.h>

// Prevent stuff from working if we're not using a cross compiler.
#if defined(__linux__)
#error "You are not using a cross-compiler."
#endif

void kernel_main(void)
{
    // Initialize the terminal interface
    term_init();

    term_setcolor(0x0F);

    klog("TTY Initialized!");

    kpanic("Test");

    kprintf("\n\r===SERIAL INITIALIZATION===\n\r");

    int hasSerial = 1;

    klog("Initializing COM1 (0x3F8) Serial Port for debugging...");
    if (initSerial(0x3F8) == 1)
    {
        kerror("COM1 Serial Chip test FAILED!");
        hasSerial = 0;
    }
    else
    {
        klog("Routing TTY output to Serial...");
        term_setSerial(0x3F8);
    }
    klog("Serial initialization finished!");
    if (hasSerial == 0)
        kwarn("NOTE: Debugging through COM1 port is unavailable!");

    kprintf("\n\r===MMU INITIALIZATION===\n\r");

    
}