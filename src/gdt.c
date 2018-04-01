#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "system.h"
#include "gdt.h"

gdt_descriptor_t gdt[7] __attribute__((aligned (8)));
gdt_ptr_t gdtr __attribute__((aligned (8)));
tss_entry_t tss_entry;
extern void gdt_load(gdt_ptr_t*);
extern void tss_flush();
extern void tss_stack;

// Setup a segment descriptor in the gdt
static void gdt_setDescriptor(uint32_t idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
    gdt[idx].base_low    =  base & 0xFFFF;
    gdt[idx].base_middle = (base >> 16) & 0xFF;
    gdt[idx].base_high   = (base >> 24) & 0xFF;
    gdt[idx].limit_low   = limit & 0xFFFF;
    gdt[idx].granularity = (limit >> 16) & 0x0F;
    gdt[idx].granularity |= (granularity & 0xF0);
    gdt[idx].access      = access;
}

void gdt_install(void)
{
    gdtr.limit = (sizeof(gdt_descriptor_t) * 7) - 1;
    gdtr.base  = (uint32_t)&gdt;
    gdt_setDescriptor(0, 0, 0, 0, 0);   //null descriptor
    gdt_setDescriptor(1, 0, 0, 0, 0);   //not used
    gdt_setDescriptor(2, 0, 0xFFFFF, PRESENT | RING_0 | CODE_DATA_STACK | CODE_EXEC_READ,  _4KB_ | BIT_32 | MODE_IA32);
    gdt_setDescriptor(3, 0, 0xFFFFF, PRESENT | RING_0 | CODE_DATA_STACK | DATA_READ_WRITE, _4KB_ | BIT_32 | MODE_IA32);
    gdt_setDescriptor(4, 0, 0xFFFFF, PRESENT | RING_3 | CODE_DATA_STACK | CODE_EXEC_READ,  _4KB_ | BIT_32 | MODE_IA32);
    gdt_setDescriptor(5, 0, 0xFFFFF, PRESENT | RING_3 | CODE_DATA_STACK | DATA_READ_WRITE, _4KB_ | BIT_32 | MODE_IA32);
    gdt_setDescriptor(6, 0, 0, 0, 0);   //tss initialy set 0
    gdt_load(&gdtr);
}

void tss_install(void)
{
    uint32_t tss_base = (uint32_t)&tss_entry;
    uint32_t tss_limit = sizeof(tss_entry);
    memset((char*)&tss_entry, 0, sizeof(tss_entry_t));
    gdt_setDescriptor(6, tss_base, tss_limit, PRESENT | RING_3 | SYSTEM | 0x09, _4KB_ | BIT_32 | MODE_IA32);
    tss_entry.ss0  = 0x18;  // set the kernel stack segment
    tss_entry.esp0 = (uint32_t)&tss_stack; // set the kernel stack pointer.
    tss_flush();
}
/* Copyright */
