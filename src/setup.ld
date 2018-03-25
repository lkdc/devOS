OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

ENTRY(start32)
SECTIONS{
	/*located 1 MB + 3GB; will be loaded at 1MB*/
	. = 0xC0100000;
	_start = . - 0xC0000000;
	/* align at 4 KB and load at 1 MB*/
	.text ALIGN (0x1000) : AT(ADDR(.text) - 0xC0000000){
			*(.text)
			*(.rodata*)
		}
	.data ALIGN (0x1000) : AT(ADDR(.data) - 0xC0000000){
			*(.data)
	}
	.bss ALIGN (0x1000)	:	AT(ADDR(.bss) - 0xC0000000){
			_bss = . - 0xC0000000;
			*(COMMON)
			*(.bss)
	}
	_end = . - 0xC0000000;
}
