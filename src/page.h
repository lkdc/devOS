#ifndef PAGE_H_
#define PAGE_H_

#define PAGE_BITS 12
#define PAGE_SIZE ((uint32_t)1<<PAGE_BITS)
#define PAGE_MASK (~(PAGE_SIZE-1))
//#define PAGE_ALIGN(addr)		(((addr)+PAGE_SIZE-1)&PAGE_MASK)

#define PGT_BITS  10
#define PGT_ENTR  ((uint32_t)1<<PGT_BITS)

#define PGD_BITS  10
#define PGD_ENTR  ((uint32_t)1<<PGD_BITS)


#define PAGE_OFFSET 0xC0000000
#define __pa(x) ((uint32_t)(x)-PAGE_OFFSET)
#define __va(x) ((void*)(PAGE_OFFSET+(uint32_t)(x)))

#define KPGD_ENTR (PAGE_OFFSET>>(PGT_BITS+PAGE_BITS))


#define KADDR_MAX (((uint32_t)1<<30)-1)

typedef uint32_t addr;
typedef uint32_t pgd_t;
typedef uint32_t pgt_t;

void *mem_align(uint32_t count, void *ptr_mem);
addr addr_align(uint32_t count, addr mem);
void identity_map();


#endif
