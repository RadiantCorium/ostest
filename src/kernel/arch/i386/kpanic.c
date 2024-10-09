#include <kernel/tty.h>
#include <kernel/kpanic.h>

#include <libk/stdio.h>

#include <stdint.h>

typedef struct registers_t
{
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t eip, cs, eflags;
    uint32_t useresp, ss;
};

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