#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "system.h"
#include "util.h"
#include "idt.h"
#include "keyboard.h"
#include "timer.h"

idt_gate_t idt[256] __attribute__((aligned (8)));
idt_ptr_t idtr __attribute__((aligned (8)));
extern void idt_load(idt_ptr_t*);

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

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

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

static inline void iowait(void)
{
	asm volatile("jmp 1f\n\t"
			"1:jmp 2f\n\t"
			"2:");
}

void irq_remap(void)
{
    outb(0x20, 0x11);
    iowait();
    outb(0xA0, 0x11);
    iowait();
    outb(0x21, 0x20);
    iowait();
    outb(0xA1, 0x28);
    iowait();
    outb(0x21, 0x04);
    iowait();
    outb(0xA1, 0x02);
    iowait();
    outb(0x21, 0x01);
    iowait();
    outb(0xA1, 0x01);
    iowait();
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

void irq_install()
{
    irq_remap();

    idt_set_gate(32, (unsigned)irq0, 0x10, 0x8E);
    idt_set_gate(33, (unsigned)irq1, 0x10, 0x8E);
    idt_set_gate(34, (unsigned)irq2, 0x10, 0x8E);
    idt_set_gate(35, (unsigned)irq3, 0x10, 0x8E);
    idt_set_gate(36, (unsigned)irq4, 0x10, 0x8E);
    idt_set_gate(37, (unsigned)irq5, 0x10, 0x8E);
    idt_set_gate(38, (unsigned)irq6, 0x10, 0x8E);
    idt_set_gate(39, (unsigned)irq7, 0x10, 0x8E);
    idt_set_gate(40, (unsigned)irq8, 0x10, 0x8E);
    idt_set_gate(41, (unsigned)irq9, 0x10, 0x8E);
    idt_set_gate(42, (unsigned)irq10, 0x10, 0x8E);
    idt_set_gate(43, (unsigned)irq11, 0x10, 0x8E);
    idt_set_gate(44, (unsigned)irq12, 0x10, 0x8E);
    idt_set_gate(45, (unsigned)irq13, 0x10, 0x8E);
    idt_set_gate(46, (unsigned)irq14, 0x10, 0x8E);
    idt_set_gate(47, (unsigned)irq15, 0x10, 0x8E);
}

const char *int_messages[48] =
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
    "Intel Reserved",
    "IRQ 0 - System Timer",
    "IRQ 1 - Keyboard Controller",
    "IRQ 2 - Cascade Interrupt for PIC2"
    "IRQ 3 - Serial Port COM2",
    "IRQ 4 - Serial Port COM1",
    "IRQ 5 - Parallel Port 2 / Sound Card",
    "IRQ 6 - Floppy Disk",
    "IRQ 7 - Parallel Port 1",
    "IRQ 8 - RTC",
    "IRQ 9 - ACPI",
    "IRQ 10 - Open Interrupt",
    "IRQ 11 - Open Interrupt",
    "IRQ 12 - PS/2 Mouse",
    "IRQ 13 - FPU / Interprocessor Interrupt",
    "IRQ 14 - Primary ATA",
    "IRQ 15 - Secondary ATA"
};

void isr_handler(struct regs *r)
{
    if (r->int_no < 32)
    {
        printf("Exception Nr: %u -> ", r->int_no);
        printf("%s\n", int_messages[r->int_no]);
    }
    for (;;){}
}

static __inline__ void eoi(uint32_t int_no)
{
  if (int_no >= 40){
      outb(0xA0, 0x20);
  }
  outb(0x20, 0x20);
}

extern struct keyboard_buffer k_buff;

void irq_handler(struct regs *r)
{
    switch(r->int_no){
        case 32:
            timer_handler();
            break;
        case 33:
            keyboard_handler(&k_buff);
            break;
        default:
            break;
    }
    eoi(r->int_no);
}

void print_c(struct keyboard_buffer *k_buff)
{
  struct keyboard_buffer *buf;
  buf = k_buff;
  if (buf->head != buf->tail) {
    printf("%c", buf->key[buf->tail]);
    buf->tail = (buf->tail + 1) % 128;
  }
}
