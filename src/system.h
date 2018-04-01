#ifndef SYSTEM_H_
#define SYSTEM_H_

char inb (uint16_t _port);
void outb (uint16_t _port, char _data);
void *memcpy(void *ptr_dest, const void *ptr_src, uint32_t count);
char *memset(char *ptr_dest, char val, uint32_t count);
uint16_t *memsetw(uint16_t *ptr_dest, uint16_t val, uint32_t count);
uint32_t *memsetd(uint32_t *ptr_dest, uint32_t val, uint32_t count);
uint32_t strlen(const char *ptr_str);

#endif
