#include <kernel/idt.h>

#include <libk/stdio.h>

struct InterruptDescriptor32
{
    uint16_t offset1;
    uint16_t selector;
    uint8_t zero;
    uint8_t typeAttributes;
    uint16_t offset2;
} __attribute__((packed));

enum gateType {
    GATE_INT = 0x8E,
    GATE_TRAP = 0x8F,
    GATE_TASK = 0x85
};

static struct InterruptDescriptor32 _fillIRQ(void (*wrapper)(void), enum gateType type)
{
    struct InterruptDescriptor32 irq;

    uint32_t handlerAddress = (uint32_t)wrapper;

    irq.offset1 = handlerAddress & 0xFFFF;
    irq.offset2 = (handlerAddress >> 16) & 0xFFFF;
    irq.selector = 0x08;
    irq.zero = 0;
    irq.typeAttributes = type;
    
    // 0000000000001000

    return irq;
}

void encodeIdtEntry(void *idtHigh, void *idtLow, struct InterruptDescriptor32 ides)
{
    uint32_t *targetLow = (uint32_t *)idtLow;
    uint32_t *targetHigh = (uint32_t *)idtHigh;
    
    *targetLow = ((uint32_t)ides.offset1) |
                ((uint32_t)ides.selector << 16);

    *targetHigh = ((uint32_t)ides.zero) |
                ((uint32_t)ides.typeAttributes << 8) |
                ((uint32_t)ides.offset2 << 16);
}

void fillIdt(void *idtPtr)
{
    struct InterruptDescriptor32 irq;

    irq = _fillIRQ(isrWrapper, GATE_TRAP);

    // Interrupts 0x0-0x1F are all GATE_TRAP type (exceptions).
    for (int i = 0x0; i <= 0x1F*2; i+=2)
    {
        encodeIdtEntry(idtPtr + (i * sizeof(struct InterruptDescriptor32)), idtPtr + (i+1 * sizeof(struct InterruptDescriptor32)), irq);
        // kprintf("\tidt[%d] = 0x%X 0x%X\n\r", i, *(uint64_t*)(idtPtr + (i * sizeof(struct InterruptDescriptor32))), *(uint64_t*)(idtPtr + (i+1 * sizeof(struct InterruptDescriptor32))));
    }

    return;

    // GATE_INT types
    irq = _fillIRQ(isrWrapper, GATE_INT);

    // Interrupts 0-15 are all GATE_INT type. The PICs are remapped to 0x20.
    for (int i = 0x20*2; i <= 0x2F*2; i+=2)
    {
        encodeIdtEntry(idtPtr + (i * sizeof(struct InterruptDescriptor32)), idtPtr + (i+1 * sizeof(struct InterruptDescriptor32)), irq);
        // kprintf("\tidt[%d] = 0x%X 0x%X\n\r", i, *(uint64_t*)(idtPtr + (i * sizeof(struct InterruptDescriptor32))), *(uint64_t*)(idtPtr + (i+1 * sizeof(struct InterruptDescriptor32))));
    }
}