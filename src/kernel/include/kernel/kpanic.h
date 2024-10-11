#ifndef KPANIC_H
#define KPANIC_H

typedef struct registers_t registers_t;

enum ERRCODE {
    KPANIC_TEST,
};

void kpanic(enum ERRCODE code);
extern void capture_registers(registers_t *regs);

typedef struct StackFrame
{
    struct StackFrame *ebp;
    void *returnAddress;
} StackFrame;

#endif