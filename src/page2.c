#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "page.h"
#include "util.h"



extern uint32_t kernel_start;
extern uint32_t kernel_end;
extern uint32_t mem_lower;
extern uint32_t mem_upper;
extern pgd_t *pgd0;

void *mem_align(uint32_t count, void *ptr_mem)
{
  uint32_t mem;
  mem = (uint32_t)ptr_mem;
  mem -= 1;
  mem /= count;
  mem *= count;
  mem += count;
  return (void*)mem;
}
addr addr_align(uint32_t count, addr mem)
{
  mem -= 1;
  mem /= count;
  mem *= count;
  mem += count;
  return mem;
}

void identity_map()
{
  pgd_t *pgd = pgd0;
  pgt_t *pgt;

  addr pgt_mem;
  addr mem_mem;

  pgt_mem = (addr)__va(&kernel_end);
  mem_mem = 0xC0400000;

  pgt_mem = addr_align(PAGE_SIZE,pgt_mem);


  pgt = (pgt_t*)pgt_mem;
  pgd += KERNEL_PGD_ENTR;

  while(mem_mem < (addr)__va(mem_upper)){

    if (*pgd == 0){
      *pgd = __pa(pgt_mem) | 7;
      while((addr)pgt < (pgt_mem + PAGE_SIZE)){
        *pgt = __pa(mem_mem) | __pa(pgt) | 7;
        pgt++;
      }
      pgt_mem += PAGE_SIZE;
      pgt = (pgt_t*)pgt_mem;
    }
    pgd++;
    mem_mem += 0x400000;
  }



}
