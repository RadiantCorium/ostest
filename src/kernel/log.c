#include <kernel/log.h>
#include <kernel/tty.h>

#include <libk/stdio.h>

// TODO: Add a way to process ANSI sequences so we don't have to call kprintf 3 times.

void klog(const char *msg)
{
    kprintf("[");
    term_setcolor(0x03);
    kprintf("LOG");
    term_setcolor(0x0F);
    kprintf("]: %s\n\r", msg);
}

void kwarn(const char *msg)
{
    kprintf("[");
    term_setcolor(0x0E);
    kprintf("WARN");
    term_setcolor(0x0F);
    kprintf("]: %s\n\r", msg);
}

void kerror(const char *msg)
{
    kprintf("[");
    term_setcolor(0x04);
    kprintf("ERROR");
    term_setcolor(0x0F);
    kprintf("]: %s\n\r", msg);
}