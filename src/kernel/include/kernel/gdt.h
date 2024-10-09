#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct GDT
{
    int base;
    uint32_t limit;
    uint8_t accessByte;
    uint8_t flags;
};

void encodeGdtEntry(uint8_t *target, struct GDT source)
{
    if (source.limit > 0xFFFFF) {}

    // Encode the limit
    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6] = (source.limit >> 16) & 0xFF;

    // Encode the base
    target[2] = source.base & 0xFF;
    target[3] = (source.base >> 8) & 0xFF;
    target[4] = (source.base >> 16) & 0xFF;
    target[7] = (source.base >> 24) & 0xFF;

    // Encode the access byte
    target[5] = source.accessByte;

    // Endcode the flags
    target[6] |= (source.flags << 4);
}

#endif