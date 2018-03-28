#ifndef IDT_H_
#define IDT_H_

struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
};

// IDT gate descriptor
typedef struct idt_gate
{
    uint16_t  base_lo;
    uint16_t  sel;          /* Our kernel segment goes here! */
    uint8_t   always0;      /* This will ALWAYS be set to 0! */
    uint8_t   flags;        /* Set using the above table! */
    uint16_t  base_hi;
} __attribute__((packed)) idt_gate_t ;

// IDT pointer
typedef struct idt_ptr
{
    uint16_t  limit;
    uint32_t  base;
} __attribute__((packed)) idt_ptr_t;

void idt_install(void);
void idt_set_gate(uint8_t idx, uint32_t base, uint16_t sel, uint8_t flags);
void fault_handler(struct regs *r);
void isr_install(void);

#endif
