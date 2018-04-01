.set offset, 0xC0000000
/* Align all boot modules on i386 page (4KB) boundaries. */
.set MULTIBOOT_PAGE_ALIGN, 1<<0
/* Must pass memory information to OS. */
.set MULTIBOOT_MEMORY_INFO, 1<<1
/* Must pass video information to OS. */
.set MULTIBOOT_VIDEO_MODE, 1<<2
/* This flag indicates the use of the address fields in the header. */
.set MULTIBOOT_AOUT_KLUDGE, 1<<16

.set MULTIBOOT_HEADER_FLAGS,			MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
.set MULTIBOOT_HEADER_MAGIC,			0x1BADB002
.set MULTIBOOT_HEADER_CHECKSUM,		-(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

.section .text
.globl start_32
.type start_32, @function
start_32:
	jmp multiboot_entry

.align  4
multiboot_header:
.long MULTIBOOT_HEADER_MAGIC
.long MULTIBOOT_HEADER_FLAGS
.long MULTIBOOT_HEADER_CHECKSUM

.long multiboot_header
.long _start
.long _bss
.long _end
.long multiboot_entry

multiboot_entry:
	cli
	cld
	/* identity-map the kernel in low 4MB memory for ease of transition */
	movl $boot_pgt-offset+7, boot_pgd-offset		/* set present bit/user r/w */
	/* but the real place is at offset=0xC0000000 */
	movl $boot_pgt-offset+7, boot_pgd-offset+(768*4)	/* set present bit/user r/w */
	mov $7,%eax
	mov $boot_pgt-offset, %edi
	mov $1024, %ecx
1:
	stosl
	add $0x1000, %eax
	loop 1b

	mov $boot_pgd-offset, %eax
	mov %eax, %cr3		/* cr3 - page directory start */
	mov %cr0, %eax
	or $0x80000000, %eax
	mov %eax, %cr0		/* set paging (PG) bit */
	mov %cr3, %eax
	mov %eax, %cr3

	mov $kernel_offset, %eax
	jmp *%eax
kernel_offset:
	movl $0, boot_pgd-offset	/* clear first entry in page dir*/
	mov $boot_pgd-offset, %eax
	invlpg (%eax)
	# set the stack
	mov $boot_stack, %esp
	# clear eflags
	push $0
	popf
	#	push multiboot structure pointer
	add $offset, %ebx
	push %ebx
	#	push page directory pointer
	mov $boot_pgd, %eax
	push %eax
	# call "C" main
	call main
end:
	cli
	hlt
	jmp end

.section .bss, "aw", @nobits
.align 0x1000
boot_pgd:
.skip 0x1000
boot_pgt:
.skip 0x1000

# Stack
.skip 0x4000
boot_stack:
.globl tss_stack
.skip 0x4000
tss_stack:
.globl user_stack
.skip 0x4000
user_stack:
