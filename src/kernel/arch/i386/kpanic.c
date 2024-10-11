#include <kernel/tty.h>
#include <kernel/kpanic.h>

#include <libk/stdio.h>
#include <libk/string.h>

#include <stdint.h>

typedef struct registers_t
{
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t eip, cs, eflags;
    uint32_t useresp, ss;
};

static void printStackTrace(unsigned int maxFrames);

void kpanic(enum ERRCODE code)
{
    registers_t regs;
    capture_registers(&regs);

    term_setcolor(0x4F);
    term_clear();

    term_setpos(0, 0);

    term_setcolor(0xCF);

    char kpBfr[80];
    kmemset(kpBfr, ' ', 80);
    if (code == 0)
        kstrncpy(kpBfr + 34, "TEST KRNLPNC", sizeof("KERNEL PANIC") - 1);
    else
        kstrncpy(kpBfr + 34, "KERNEL PANIC", sizeof("KERNEL PANIC") - 1);
    kprintf(kpBfr);

    term_setcolor(0x4F);

    kprintf("KP_CODE: 0x%X\n\n\r", code);

    // TODO: Add padding to prinf %x
    kprintf("REGISTER DUMP:\n\r");
    kprintf("EAX: 0x%X, EBX: 0x%X, ECX: 0x%X, EDX: 0x%X\n\r", regs.eax, regs.ebx, regs.ecx, regs.edx);
    kprintf("ESI: 0x%X, EDI: 0x%X, EBP: 0x%X, ESP: 0x%X\n\r", regs.esi, regs.edi, regs.ebp, regs.esp);
    kprintf("EIP: 0x%X, CS: 0x%X, EFLAGS 0x%X\n\r", regs.eip, regs.cs, regs.eflags);

    kprintf("\nSTACK TRACE:\n\r");
    printStackTrace(10);

    #if defined ARCHS
    kprintf("\n\rARCHITECTURE: %s\n\n\r", ARCHS);
    #else
    kprintf("\n\rARCHITECTURE: N/A\n\n\r");
    #endif

    kprintf("PLEASE SUBMIT AN ISSUE TO HTTPS://GITHUB.COM/RADIANTCORIUM/OSTEST\n\rWITH A SCREENSHOT OF THIS SCREEN.");

    for (;;)
        asm ("hlt");
}

static void printStackTrace(unsigned int maxFrames)
{
    StackFrame *frame;
    asm volatile ("movl %%ebp, %0" : "=r"(frame));

    for (unsigned int i = 0; i < maxFrames; i++)
    {
        if (!frame)
            kprintf("0x0\n\r");
        else
            kprintf("0x%p\n\r", (void *)frame->returnAddress);
        frame = frame->ebp;
    }
}