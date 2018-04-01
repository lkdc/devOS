#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "system.h"
#include "util.h"
#include "multiboot.h"
#include "gdt.h"
#include "idt.h"
#include "page.h"

uint32_t mem_lower;
uint32_t mem_upper;
uint32_t zone_normal;

multiboot_info_t *mbi;
pgd_t	*pg_dir;
extern gdt_ptr_t gdtr;
extern void jump_user();

void user_function()
{
		uint32_t *x;
		x=(uint32_t*)0xCF100000;
		*x = 10;
		printf ("x=%u\n", *x);
		for(;;){}
}

void main(pgd_t *ptr_pg_dir, multiboot_info_t *ptr_mbi){

	mbi = ptr_mbi;
	pg_dir = ptr_pg_dir;
	mem_lower = mbi->mem_lower * 1024;
	mem_upper = mbi->mem_upper * 1024;
	if (mem_upper >> 30) zone_normal = KADDR_MAX; else zone_normal = mem_upper;

	identity_map();
	gdt_install();
	tss_install();
	idt_install();
	isr_install();
	irq_install();

	clear_scr ();
	printf ("mem_lower = %u Bytes, mem_upper = %u Bytes\n", mem_lower, mem_upper);
	printf ("RAM = %u MB\n", (zone_normal+1) / 1024 / 1024);

	jump_user();

  for(;;){}
}
