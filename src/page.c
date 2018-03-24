#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "page.h"
#include "util.h"



extern uint32_t kernel_start;
extern uint32_t kernel_end;
extern uint32_t mem_lower;
extern uint32_t mem_upper;
extern uint32_t zone_normal;
extern pgd_t *pgd0;

void *mem_align(uint32_t count, void *ptr_mem)
{
  uint32_t mem = (uint32_t)ptr_mem;
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

  pgt = (pgt_t*)mem_align(PAGE_SIZE,(void*)__va(&kernel_end));
  for(uint32_t i = KPGD_ENTR;
          i < ((uint32_t)__va(zone_normal)>>(PGT_BITS+PAGE_BITS)); i++){
    if (pgd[i] == 0){
      pgd[i] = __pa((addr)pgt | 7);
      for(uint32_t j = 0; j < PGT_ENTR; j++){
        pgt[j] = __pa((i<<(PGT_BITS+PAGE_BITS)) | (j<<PAGE_BITS) | 7);
      }
      pgt += PGT_ENTR;

    }
  }

}
