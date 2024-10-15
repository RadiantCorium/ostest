#include <kernel/gdt.h>
#include <kernel/log.h>

void encodeGdtEntry(uint32_t *targetLow, uint32_t *targetHigh, struct SegmentDescriptor32 source)
{
    // Check the limit to make sure that it can be encoded
    if (source.limit > 0xFFFFF) {kerror("SegmentDescriptor32 cannot encode limits larger than 0xFFFFF");}
    
    uint16_t baseLow = source.base & 0xFFFF;
    uint8_t baseMid = (source.base >> 16) & 0xFF;
    uint8_t baseHigh = (source.base >> 24) & 0xFF;

    uint16_t limitLow = source.limit & 0xFFFF;
    uint8_t limitHigh = (source.limit >> 16) & 0x0F;

    *targetLow = ((uint32_t)limitLow) |
                ((uint32_t)baseLow << 16);

    *targetHigh = ((uint32_t)baseMid) |
                ((uint32_t)source.accessByte << 8) |
                ((uint32_t)limitHigh << 16) |
                ((uint32_t)source.flags << 20) |
                ((uint32_t)baseHigh << 24);
}