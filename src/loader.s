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
	cld
	/* identity-map the kernel in low 4MB memory for ease of transition */
	movl $boot_pgt-offset+7, boot_pgd-offset		/* set present bit/user r/w */
	/* but the real place is at offset=0xC0000000 */
	movl $boot_pgt-offset+7, boot_pgd-offset+(768*4)	/* set present bit/user r/w */
	movl $7,%eax
	movl $boot_pgt-offset, %edi
	movl $1024, %ecx
1:
	stosl
	addl $0x1000, %eax
	loop 1b

	movl $boot_pgd - offset, %eax
	movl %eax, %cr3		/* cr3 - page directory start */
	movl %cr0, %eax
	orl $0x80000000, %eax
	movl %eax, %cr0		/* set paging (PG) bit */
	movl %cr3, %eax
	movl %eax, %cr3

	movl $kernel_offset, %eax
	jmpl *%eax
kernel_offset:
	movl $0, boot_pgd-offset	/* clear first entry in page dir*/
	movl $boot_pgd-offset, %eax
	invlpg (%eax)
	# set the stack
	movl $boot_stack, %esp
	# clear eflags
	pushl $0
	popfl
	#	push multiboot structure pointer
	addl $offset, %ebx
	pushl %ebx
	#	push page directory pointer
	movl $boot_pgd, %eax
	pushl %eax
	# call "C" main
	call cmain
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
