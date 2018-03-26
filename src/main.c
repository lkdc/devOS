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

void cmain(pgd_t *ptr_pg_dir, multiboot_info_t *ptr_mbi){

	mbi = ptr_mbi;
	pg_dir = ptr_pg_dir;
	mem_lower = mbi->mem_lower * 1024;
	mem_upper = mbi->mem_upper * 1024;
	if (mem_upper >> 30) zone_normal = KADDR_MAX; else zone_normal = mem_upper;

	identity_map();
	gdt_install();
	idt_install();
	clear_scr ();

	prints ("mem_lower = %u Bytes, mem_upper = %u Bytes\n", mem_lower, mem_upper);
	prints ("RAM = %u MB\n", (zone_normal+1) / 1024 / 1024);





















/* Tests for system.h
*	char *src;
*	char *dest_c;
*	uint16_t *dest_w;
*
*	src = (char*)0xCA000100;
*	dest_c = (char*)0xCA000200;
*	dest_w = (uint16_t*)0xCA000300;
*
*	for(char i = 0; i < 10; i++){
*		src[i] = i + (char)65;
*	}
*	dest_c = memcpy(dest_c,src, strlen(src));
*	prints("length_c = %u\n", strlen(dest_c));
*	prints("%c ", dest_c[i]);
*	for(uint32_t i = 0; i < strlen(dest_c); i++){
*	}
*	prints("\n");
*	dest_c = memset(dest_c, (char)65, strlen(dest_c));
*	for(uint32_t i = 0; i < strlen(dest_c); i++){
*			prints("%c ", dest_c[i]);
*	}
*	prints("\n");
*	for(uint16_t i = 0; i < 10; i++){
*		dest_w[i] = i+257;
*	}
*	prints("length_w = %u\n", strlen((char*)dest_w)/2);
*	for(uint32_t i = 0; i < strlen((char*)dest_w)/2; i++){
*			prints("%u ", dest_w[i]);
*	}
*	prints("\n");
*	dest_w = memsetw(dest_w, 257, strlen((char*)dest_w)/2);
*	for(uint32_t i = 0; i < strlen((char*)dest_w)/2; i++){
*			prints("%u ", dest_w[i]);
*	}
*	prints("\n");
*/
  for(;;){
	}
}
