.section .text
.global gdt_flush
.type gdt_flush, @function
gdt_flush:
    movl 4(%esp), %eax  # address of gdtr is send on the stack
    lgdt (%eax)         # load the new gdtr

    mov $0x18, %ax      # 0x18 is the offset in the gdt to our data segment
    mov %ax, %ds        # load all data segment selectors
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    jmp $0x10,$flush    #  0x10 is the offset to our code segment: far jump

flush:
    ret
