#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "idt.h"

extern void idt_load(idt_ptr_t*) __attribute__((cdecl));
idt_gate_t idt[256] __attribute__((aligned (8)));
idt_ptr_t idtr __attribute__((aligned (8)));

void idt_set_gate(uint8_t idx, uint32_t base, uint16_t sel, uint8_t flags)
{
  /* TODO */
}

void idt_install(void)
{
    idtr.limit = (sizeof (idt_gate_t) * 256) - 1;
    idtr.base = &idt;
    memset((char*)&idt, 0, sizeof(idt_gate_t) * 256);

    /* Add any new ISRs to the IDT here using idt_set_gate */

    idt_load(&idtr);
}
