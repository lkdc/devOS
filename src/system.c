#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "system.h"

char inportb (uint16_t _port)
{
    char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb (uint16_t _port, char _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void *memcpy(void *ptr_dest, const void *ptr_src, uint32_t count)
{
    char *dest = (char*)ptr_dest;
    const char *src = (char*)ptr_src;
    for (uint32_t i = 0; i < count; i++){
        dest[i] = src[i];
    }
    return (void*)dest;
}

char *memset(char *ptr_dest, char val, uint32_t count)
{
		char *dest = ptr_dest;
  	for (uint32_t i = 0; i < count; i++){
    		dest[i] = val;
  	}
		return dest;
}

uint16_t *memsetw(uint16_t *ptr_dest, uint16_t val, uint32_t count)
{
    uint16_t *dest = ptr_dest;
    for (uint32_t i = 0; i < count; i++){
        dest[i] = val;
    }
    return dest;
}

uint32_t *memsetd(uint32_t *ptr_dest, uint32_t val, uint32_t count)
{
    uint32_t *dest = ptr_dest;
    for (uint32_t i = 0; i < count; i++){
        dest[i] = val;
    }
    return dest;
}


uint32_t strlen(const char *ptr_str)
{
  const char *str = ptr_str;
  uint32_t count = 0;
  while(1){
    if (!str[count]) break;
    count += 1;
  }
  return count;
}
