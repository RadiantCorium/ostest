#include <kernel/gdt.h>

void loadGDT(uint32_t table[])
{
    struct SegmentDescriptor32 gdt = {0, 0, 0, 0};

    table[0] = 0;
    table[1] = 0;

    // Kernel Mode Code Segment
    gdt.base = 0;
    gdt.limit = 0xFFFFF;
    gdt.accessByte = 0x9A;
    gdt.flags = 0xC;
    encodeGdtEntry(&table[2], &table[3], gdt);

    // Kernel Mode Data Segment
    gdt.base = 0;
    gdt.limit = 0xFFFFF;
    gdt.accessByte = 0x92;
    gdt.flags = 0xC;
    encodeGdtEntry(&table[4], &table[5], gdt);

    // User Mode Code Segment
    gdt.base = 0;
    gdt.limit = 0xFFFFF;
    gdt.accessByte = 0xFA;
    gdt.flags = 0xC;
    encodeGdtEntry(&table[6], &table[7], gdt);

    // User Mode Data Segment
    gdt.base = 0;
    gdt.limit = 0xFFFFF;
    gdt.accessByte = 0xF2;
    gdt.flags = 0xC;
    encodeGdtEntry(&table[8], &table[9], gdt);

    // TODO: Implement TSS Segment
}