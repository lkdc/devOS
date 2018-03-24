OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

ENTRY(start32)           	/* the name of the entry symbol */
SECTIONS{
	/* the code should be located 1 MB + 3GB*/
	. = 0xC0100000;

  /* these labels get exported to the code files */
  kernel_start = . - 0xC0000000;

  /* align at 4 KB and load at 1 MB + 3GB*/
  .text ALIGN (0x1000)	:	AT(ADDR(.text) - 0xC0000000){
		*(.multiboot)
		*(.text)            /* text sections */
  }

  /* align at 4 KB and load at 1 MB + 3GB*/
  .rodata ALIGN (0x1000)	:	AT(ADDR(.rodata) - 0xC0000000){
      *(.rodata*)         /* read-only data sections */
  }

  /* align at 4 KB and load at 1 MB + 3GB*/
  .data ALIGN (0x1000)	:	AT(ADDR(.data) - 0xC0000000){
      *(.data)            /* all data sections */
  }

	/* align at 4 KB and load at 1 MB + 3GB*/
	.bss ALIGN (0x1000)	:	AT(ADDR(.bss) - 0xC0000000){
			*(COMMON)           /* all COMMON sections */
      *(.bss)             /* all bss sections */
			*(.page0)
			*(.stack0)
  }
	kernel_end = . - 0xC0000000;
}
