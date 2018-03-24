#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "util.h"
#include "multiboot.h"
#include "gdt.h"
#include "page.h"

uint32_t mem_lower;
uint32_t mem_upper;

uint32_t zone_normal;

multiboot_info_t *mbi;
pgd_t	*pgd0;

void kmain(pgd_t *ptr_pgd0, multiboot_info_t *ptr_mbi){

	mbi = __va(ptr_mbi);
	pgd0 = ptr_pgd0;
	mem_lower = mbi->mem_lower * 1024;
  mem_upper = mbi->mem_upper * 1024;
	if (mem_upper >> 30) zone_normal = KADDR_MAX; else zone_normal = mem_upper;

  gdt_install();
	identity_map();
  clear_scr ();

  prints ("mem_lower = %u Bytes, mem_upper = %u Bytes\n", mem_lower, mem_upper);
  prints ("KRAM = %u MB\n", (zone_normal+1) / 1024 / 1024);

	uint32_t *x;
	x = (uint32_t*)0xCF000000;
	*x=1234;
  prints ("x= %u\n", *x);


  for(;;){}
}
