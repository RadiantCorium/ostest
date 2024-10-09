#ifndef KPANIC_I386_H
#define KPANIC_I386_H

#include <kernel/kpanic.h>

#include <stdint.h>

typedef struct registers_t
{
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t eip, cs, eflags;
    uint32_t useresp, ss;
};

#endif