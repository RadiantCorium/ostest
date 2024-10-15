#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <libk/string.h>
#include <libk/stdio.h>

#include <kernel/tty.h>
#include <kernel/serial.h>
#include <kernel/log.h>

#include <kernel/gdt.h>

#include <kernel/idt.h>
#include <kernel/pic.h>

#include <kernel/kpanic.h>

// Prevent stuff from working if we're not using a cross compiler.
#if defined(__linux__)
#error "You are not using a cross-compiler."
#endif

extern uint32_t gdt[10];

void kernel_init(void)
{
    // Initialize the terminal interface
    term_init();

    term_setcolor(0x0F);

    klog("TTY Initialized!");

    kprintf("\n\r===SERIAL INITIALIZATION===\n\r");

    int hasSerial = 1;

    klog("Initializing COM1 (0x3F8) Serial Port for debugging...");
    if (initSerial(0x3F8) == 1)
    {
        kerror("COM1 Serial Chip test FAILED!");
        kprintf("\tIf you don't have a Serial Port this is expected.\n\r");
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

    asm volatile ("cli");

    // I hate this
    kprintf("\n\r===GDT INITIALIZATION===\n\r");
    loadGDT(gdt);
    klog("GDT LOADED!");
    kprintf("\tGDT Base = 0x%p\n\r", &gdt);
    kprintf("\tGDT Limit = 0x%X\n\r", sizeof(gdt) - 1);
    
    kprintf("\tgdt[0] = 0x%X 0x%X\n\r", gdt[0], gdt[1]);
    
    for (size_t i = 2; i < sizeof(gdt) / sizeof(gdt[0]); i+=2)
    {
        kprintf("\tgdt[%d] = 0x%X 0x%X\n\r", i, gdt[i], gdt[i+1]);
    }

    if (gdt[0] != 0)
    {
        kerror("Null Descriptor is not zero.");
        return;
    }

    setGdt(sizeof(gdt) - 1, (uint32_t) &gdt);

    klog("Reloading Segment Registers...");
    reloadSegments();

    klog("GDT Initialization finished!");

    // asm volatile ("1: jmp 1b");

    kprintf("\n\r===INTERRUPT INITIALIZATION===\n\r");
    void *idtPtr;
    #ifdef ARCH_i386
    uint32_t idt[512];
    setIdt(sizeof(idt) - 1, (uint32_t) &idt);
	klog("IDT CREATED!");
    kprintf("\tIDT Base = 0x%p\n\r", &idt);
    kprintf("\tIDT Limit = 0x%X\n\r", sizeof(idt) - 1);
    idtPtr = (void*)idt;
    #else
    
    #endif

    klog("Remapping PIC...");
    PIC_remap(0x20, 0x28);

    klog("Filling IDT...");
    fillIdt(idtPtr);

    klog("Setting PIC Mask...");
    IRQ_clearMask(0x21);

    klog("Sending Interrupt 0x21...");
    asm volatile ("INT $0x21");
}
