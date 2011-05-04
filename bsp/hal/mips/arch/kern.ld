/*
 * Linker script for kernel
 */
INCLUDE config.ld
OUTPUT_FORMAT("elf32-littlemips", "elf32-bigmips", "elf32-littlemips")
OUTPUT_ARCH(mips)
ENTRY(kernel_start)
PHDRS
{
	text PT_LOAD FILEHDR PHDRS;
	data PT_LOAD;
}
SECTIONS
{
	. = CONFIG_KERNEL_TEXT + SIZEOF_HEADERS;

	.text : {
		*(.text)
	} : text = 0xff
	. = ALIGN(4);
	.rodata : {
		*(.rodata)
		*(.rodata.*)
	} : text
	.data ALIGN(4) : {
                *(.sdata)
                *(.sdata2)
		*(.data)
	} : data = 0xff
	_gp = ABSOLUTE(.);
	. = ALIGN(4);
	__bss = . ;
	.bss .    : {
                *(.sbss)
		*(.bss)
		*(COMMON)
	}
	. = ALIGN(4);
	__end = . ;

	/DISCARD/ : { *(.comment .pdr .note) }
}
