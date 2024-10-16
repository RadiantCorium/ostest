#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct SegmentDescriptor32
{
    int base;
    uint32_t limit;
    uint8_t accessByte;
    uint8_t flags;
} __attribute__((packed));

// Defined in kernel/gdt.c
void encodeGdtEntry(uint32_t *targetLow, uint32_t *targetHigh, struct SegmentDescriptor32 source);

void loadGDT(uint32_t table[]);

extern void setGdt(uint16_t limit, uint32_t base);

extern void reloadSegments(void);

#endif