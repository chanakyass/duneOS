#ifndef SYSTEM_H
#define SYSTEM_H

#include "stdint.h"

struct regs
{
    uint32_t gs, fs, es, ds;                /* pushed segs last */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* pushed by 'pusha' */
    uint32_t int_no, err_code;              /* pushed in ISR */
    uint32_t epi, cs, eflags, useresp, ss;  /* pushed by proc automatically */
};

void gdt_install();

void idt_install();
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

void irq_install();

#endif /* SYSTEM_H */