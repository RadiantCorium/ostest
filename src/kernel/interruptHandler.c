#include <kernel/kpanic.h>

#include <kernel/pic.h>

void interruptHandler(void)
{
    kpanic(KPANIC_TEST);

    PIC_sendEOI(0x07);
}