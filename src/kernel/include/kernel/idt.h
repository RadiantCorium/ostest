#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct InterruptDescriptor32
{
    uint16_t offset1;
    uint16_t selector;
    uint8_t zero;
    uint8_t typeAttributes;
    uint16_t offset1;
} __attribute__((packed));

#endif