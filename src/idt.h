#ifndef IDT_H_
#define IDT_H_

// IDT gate descriptor
typedef struct idt_gate
{
    uint16_t  base_lo;
    uint16_t  sel;          /* Our kernel segment goes here! */
    uint8_t   always0;      /* This will ALWAYS be set to 0! */
    uint8_t   flags;        /* Set using the above table! */
    uint16_t  base_hi;
} idt_gate_t __attribute__((packed));

// IDT pointer
typedef struct idt_ptr
{
    uint16_t  limit;
    uint32_t  base;
} idt_ptr_t __attribute__((packed));

void idt_install(void);

#endif
