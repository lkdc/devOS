/* Align all boot modules on i386 page (4KB) boundaries. */
.set MULTIBOOT_PAGE_ALIGN,		1<<0
/* Must pass memory information to OS. */
.set MULTIBOOT_MEMORY_INFO,		1<<1
/* Must pass video information to OS. */
# .set MULTIBOOT_VIDEO_MODE,		1<<2
/* This flag indicates the use of the address fields in the header. */
.set MULTIBOOT_AOUT_KLUDGE,		1<<16

.set MULTIBOOT_HEADER_FLAGS,			MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
.set MULTIBOOT_HEADER_MAGIC,			0x1BADB002
.set MULTIBOOT_HEADER_CHECKSUM,		-(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

# Multiboot header
.section .multiboot
.align 4
.long MULTIBOOT_HEADER_MAGIC
.long MULTIBOOT_HEADER_FLAGS
.long MULTIBOOT_HEADER_CHECKSUM

.section .text
.global start32
.type start32, @function
start32:
fill_pgd0:
	/* Identity-map the kernel in low 4MB memory for ease of transition */
	movl $pgt0 - 0xC0000000 + 7, pgd0 - 0xC0000000		/* set present bit/user r/w */
	/* But the real place is at 0xC0000000 */
	movl $pgt0 - 0xC0000000 + 7, pgd0 - 0xC0000000 + (768 * 4)	/* set present bit/user r/w */
	movl $7,%eax
	leal (pgt0 - 0xC0000000), %edi
	movl $1024, %ecx
fill_pgt0:
	stosl
	addl $0x1000, %eax
	loop fill_pgt0

	leal (pgd0 - 0xC0000000), %eax
	movl %eax, %cr3			/* cr3 - page directory start */
	movl %cr0, %eax
	orl $0x80000000, %eax
	movl %eax, %cr0		/* set paging (PG) bit */
	movl %cr3, %eax
	movl %eax, %cr3

	movl $kernel_va, %eax
	jmpl *%eax
kernel_va:

	movl $0, pgd0 - 0xC0000000	/* clear first entry in pgd0 */
	# movl %cr3, %eax
	# movl %eax, %cr3
	leal stack0, %esp
	# clear eflags
	pushl $0
	popfl
	#	Push multiboot structure pointer
	# addl $0xC0000000, %ebx
	pushl %ebx

	leal pgd0, %eax
	pushl %eax

	call kmain
end:
	cli
	hlt
	jmp end

.section .page0, "aw", @nobits
.align 0x1000
pgt0:
.skip 0x1000
pgd0:
.skip 0x1000

# Stack
.section .stack0, "aw", @nobits
.skip 0x4000
stack0:
