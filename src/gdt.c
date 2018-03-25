#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "gdt.h"

static gdt_descriptor_t gdt[6] __attribute__((aligned (8)));
extern void gdt_flush(gdt_ptr_t*) __attribute__((cdecl));

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
  gdt_ptr_t gdtr __attribute__((aligned (8)));

  gdtr.limit = (sizeof(gdt_descriptor_t) * 6) - 1;
  gdtr.base  = (uint32_t)&gdt;
  gdt_setDescriptor(0, 0, 0, 0, 0); //null descriptor
  gdt_setDescriptor(1, 0, 0, NOT_PRESENT, 0);  //not used
  gdt_setDescriptor(2, 0, 0xFFFFF, PRESENT | RING_0 | CODE_DATA_STACK | CODE_EXEC_READ,  _4KB_ | BIT_32 | MODE_IA32);
  gdt_setDescriptor(3, 0, 0xFFFFF, PRESENT | RING_0 | CODE_DATA_STACK | DATA_READ_WRITE, _4KB_ | BIT_32 | MODE_IA32);
  gdt_setDescriptor(4, 0, 0xFFFFF, PRESENT | RING_3 | CODE_DATA_STACK | CODE_EXEC_READ,  _4KB_ | BIT_32 | MODE_IA32);
  gdt_setDescriptor(5, 0, 0xFFFFF, PRESENT | RING_3 | CODE_DATA_STACK | DATA_READ_WRITE, _4KB_ | BIT_32 | MODE_IA32);

  gdt_flush(&gdtr);
}

/* Copyright (c) */
