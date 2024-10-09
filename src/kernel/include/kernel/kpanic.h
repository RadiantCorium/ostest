#ifndef KPANIC_H
#define KPANIC_H

typedef struct registers_t registers_t;

void kpanic(const char *msg);
extern void capture_registers(registers_t *regs);

#endif