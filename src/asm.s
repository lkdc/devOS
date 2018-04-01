.section .text
.globl gdt_load
.type gdt_load, @function
gdt_load:
  cli
  movl 4(%esp), %eax  # address of gdtr is send on the stack
  lgdt (%eax)         # load the new gdtr

  mov $0x18, %ax      # 0x18 is the offset in the gdt to data segment
  mov %ax, %ds        # load all data segment selectors
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs
  mov %ax, %ss
  jmp $0x10,$gdt_flush    # 0x10 is the offset in the gdt to code segment: far jump
gdt_flush:
  ret

.globl idt_load
.type idt_load, @function
idt_load:
  cli
  movl 4(%esp), %eax  # address of idtr is send on the stack
  lidt (%eax)         # load the new idtr
  ret

.globl tss_flush
.type tss_flush, @function
tss_flush:
  cli
  mov $0x33, %ax    # load the index of our TSS structure - The index is
                    # 0x30, as it is the 6th selector and each is 8 bytes
                    # long, but we set the bottom two bits (making 0x33)
                    # so that it has an RPL of 3, not zero.
 ltr %ax            # load 0x33 into the task state register.
 ret

.globl jump_user
.type jump_user, @function
jump_user:
  cli
  mov $0x2B, %ax    # load 0x28 + 3 (RPL ring_3) into the segment selectors
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs
  mov %esp, %eax
  push $0x2B        # user data segment with bottom 2 bits set for ring_3
  push %eax         # push our current stack
  pushf
  pop %eax
  or $0x200, %eax
  push %eax
  push $0x23        # user code segment with bottom 2 bits set for ring_3
  push $user_function
  iret

.globl isr0
.type isr0, @function
# 0: Divide Error
isr0:
  cli
  push $0
  push $0
  jmp isr_common_stub

.globl isr1
.type isr1, @function
# 1: Debug Exception
isr1:
  cli
  push $0
  push $1
  jmp isr_common_stub

.globl isr2
.type isr2, @function
# 2: NMI Interrupt
isr2:
  cli
  push $0
  push $2
  jmp isr_common_stub

.globl isr3
.type isr3, @function
# 3: Breakpoint
isr3:
  cli
  push $0
  push $3
  jmp isr_common_stub

.globl isr4
.type isr4, @function
# 4: Overflow
isr4:
  cli
  push $0
  push $4
  jmp isr_common_stub

.globl isr5
.type isr5, @function
# 5: BOUND Range Exceeded
isr5:
  cli
  push $0
  push $5
  jmp isr_common_stub

.globl isr6
.type isr6, @function
# 6: Invalid Opcode (Undefined Opcode)
isr6:
  cli
  push $0
  push $6
  jmp isr_common_stub

.globl isr7
.type isr7, @function
# 7: Device Not Available (No Math Coprocessor)
isr7:
  cli
  push $0
  push $7
  jmp isr_common_stub

.globl isr8
.type isr8, @function
# 8: Double Fault
isr8:
  cli
#  push $0
  push $8
  jmp isr_common_stub

.globl isr9
.type isr9, @function
# 9: Coprocessor Segment Overrun (reserved)
isr9:
  cli
  push $0
  push $9
  jmp isr_common_stub

.globl isr10
.type isr10, @function
# 10: Invalid TSS
isr10:
  cli
#  push $0
  push $10
  jmp isr_common_stub

.globl isr11
.type isr11, @function
# 11: Segment Not Present
isr11:
  cli
#  push $0
  push $11
  jmp isr_common_stub

.globl isr12
.type isr12, @function
# 12: Stack-Segment Fault
isr12:
  cli
#  push $0
  push $12
  jmp isr_common_stub

.globl isr13
.type isr13, @function
# 13: General Protection
isr13:
  cli
#  push $0
  push $13
  jmp isr_common_stub

.globl isr14
.type isr14, @function
# 14: Page Fault
isr14:
  cli
# push $0
  push $14
  jmp isr_common_stub

.globl isr16
.type isr16, @function
# 16: x87 FPU Floating-Point Error (Math Fault)
isr16:
  cli
  push $0
  push $16
  jmp isr_common_stub

.globl isr17
.type isr17, @function
# 17: Alignment Check
isr17:
  cli
#  push $0
  push $17
  jmp isr_common_stub

.globl isr18
.type isr18, @function
# 18: Machine Check
isr18:
  cli
  push $0
  push $18
  jmp isr_common_stub

.globl isr19
.type isr19, @function
# 19: SIMD Floating-Point Exception
isr19:
  cli
  push $0
  push $19
  jmp isr_common_stub

.globl isr20
.type isr20, @function
# 20: Virtualization Exception
isr20:
  cli
  push $0
  push $20
  jmp isr_common_stub

.extern isr_handler
isr_common_stub:
  pusha
  push %ds
  push %es
  push %fs
  push %gs
  mov $0x18, %ax      # load the kernel data segment descriptor
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs
  mov %esp, %eax      # push us the stack
  push %eax
  call isr_handler
  pop %eax
  pop %gs
  pop %fs
  pop %es
  pop %ds
  popa
  addl $0x8, %esp     # cleans up the pushed error code and pushed ISR number
  iret                # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
