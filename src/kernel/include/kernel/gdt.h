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

// Defined in kernel/gdt.c
void encodeGdtEntry(uint8_t *target, struct GDT source);

void loadGDT(uint8_t table[]);

extern void setGdt(uint16_t limit, uint32_t base);

#endif