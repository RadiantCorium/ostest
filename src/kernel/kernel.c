#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <libk/string.h>
#include <libk/stdio.h>

#include <kernel/tty.h>
#include <kernel/serial.h>
#include <kernel/log.h>

#include <kernel/kpanic.h>

// Architecture specific includes
#if defined ARCH_i386
#include "arch/i386/include/kpanic.h"
#else
#error "Current architecture includes not implemented."
#endif

// Prevent stuff from working if we're not using a cross compiler.
#if defined(__linux__)
#error "You are not using a cross-compiler."
#endif

void kpanic(const char *msg);

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

void kpanic(const char *msg)
{
    registers_t regs;
    capture_registers(&regs);

    term_setcolor(0x4F);
    term_clear();

    term_setcursor(0, -1);

    kprintf("UNRECOVERABLE EXCEPTION (KERNEL PANIC)\n\n\r");

    kprintf("P_ERRMSG: %s\n\n\r", msg);

    // TODO: Add padding to prinf %x
    kprintf("REGISTER DUMP:\n\r");
    kprintf("EAX: 0x%X, EBX: 0x%X, ECX: 0x%X, EDX: 0x%X\n\r", regs.eax, regs.ebx, regs.ecx, regs.edx);
    kprintf("ESI: 0x%X, EDI: 0x%X, EBP: 0x%X, ESP: 0x%X\n\r", regs.esi, regs.edi, regs.ebp, regs.esp);
    kprintf("EIP: 0x%X, CS: 0x%X, EFLAGS 0x%X\n\r", regs.eip, regs.cs, regs.eflags);


    for (;;)
        asm ("hlt");
}