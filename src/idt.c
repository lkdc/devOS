#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "system.h"
#include "util.h"
#include "idt.h"

idt_gate_t idt[256] __attribute__((aligned (8)));
idt_ptr_t idtr __attribute__((aligned (8)));
extern void idt_load(idt_ptr_t*) __attribute__((cdecl));

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();

void idt_set_gate(uint8_t idx, uint32_t base, uint16_t sel, uint8_t flags)
{
  /* The interrupt routine's base address */
   idt[idx].base_lo = (base & 0xFFFF);
   idt[idx].base_hi = (base >> 16) & 0xFFFF;

   idt[idx].sel = sel;
   idt[idx].always0 = 0;
   idt[idx].flags = flags;
}

void idt_install(void)
{
    idtr.limit = (sizeof (idt_gate_t) * 256) - 1;
    idtr.base = (uint32_t)&idt;
    memset((char*)&idt, 0, sizeof(idt_gate_t) * 256);

    /* Add any new ISRs to the IDT here using idt_set_gate */
    idt_load(&idtr);
}

void isr_install()
{
    idt_set_gate(0, (uint32_t)&isr0, 0x10, 0x8E);
    idt_set_gate(1, (uint32_t)&isr1, 0x10, 0x8E);
    idt_set_gate(2, (uint32_t)&isr2, 0x10, 0x8E);
    idt_set_gate(3, (uint32_t)&isr3, 0x10, 0x8E);
    idt_set_gate(4, (uint32_t)&isr4, 0x10, 0x8E);
    idt_set_gate(5, (uint32_t)&isr5, 0x10, 0x8E);
    idt_set_gate(6, (uint32_t)&isr6, 0x10, 0x8E);
    idt_set_gate(7, (uint32_t)&isr7, 0x10, 0x8E);
    idt_set_gate(8, (uint32_t)&isr8, 0x10, 0x8E);
    idt_set_gate(9, (uint32_t)&isr9, 0x10, 0x8E);
    idt_set_gate(10, (uint32_t)&isr10, 0x10, 0x8E);
    idt_set_gate(11, (uint32_t)&isr11, 0x10, 0x8E);
    idt_set_gate(12, (uint32_t)&isr12, 0x10, 0x8E);
    idt_set_gate(13, (uint32_t)&isr13, 0x10, 0x8E);
    idt_set_gate(14, (uint32_t)&isr14, 0x10, 0x8E);
    idt_set_gate(16, (uint32_t)&isr16, 0x10, 0x8E);
    idt_set_gate(17, (uint32_t)&isr17, 0x10, 0x8E);
    idt_set_gate(18, (uint32_t)&isr18, 0x10, 0x8E);
    idt_set_gate(19, (uint32_t)&isr19, 0x10, 0x8E);
    idt_set_gate(20, (uint32_t)&isr20, 0x10, 0x8E);
}

const char *exception_messages[31] =
{
    "Divide Error",
    "Debug Exception",
    "NMI Interrupt",
    "Breakpoint",
    "Overflow",
    "BOUND Range Exceeded",
    "Invalid Opcode (Undefined Opcode)",
    "Device Not Available (No Math Coprocessor)",
    "Double Fault",
    "Comprocessor Segment Overrun (reserved)",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection",
    "Page Fault",
    "Intel Reserved",
    "x87 FPU Floating-Point Error (Math Fault)",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Intel Reserved",
    "Intel Reserved",
    "Intel Reserved",
    "Intel Reserved",
    "Intel Reserved",
    "Intel Reserved",
    "Intel Reserved",
    "Intel Reserved",
    "Intel Reserved",
    "Intel Reserved",
    "Intel Reserved"
};


void fault_handler(struct regs *r)
{
    if (r->int_no < 32)
    {
          puts("Exception!");
          puts(exception_messages[r->int_no]);
    }
    for (;;);
}
