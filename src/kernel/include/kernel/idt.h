#ifndef IDT_H
#define IDT_H

#include <stdint.h>

extern void setIdt(uint16_t limit, uint32_t base);

extern void isrWrapper(void);

void fillIdt(void *idtPtr);

#endif