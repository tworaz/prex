/*
 * Linker script for driver
 */
INCLUDE config.ld
OUTPUT_FORMAT("elf32-littlemips", "elf32-bigmips", "elf32-littlemips")
OUTPUT_ARCH(mips)
ENTRY(driver_main)
PHDRS
{
	text PT_LOAD FILEHDR PHDRS;
	data PT_LOAD;
}
SECTIONS
{
	. = SIZEOF_HEADERS ;

	.text : {
		*(.text)
	}
	. = ALIGN(4);
	.rodata : {
		*(.rodata)
		*(.rodata.*)
	} : text
        . = ALIGN(4);
	.data : {
                *(.sdata)
                *(.sdata2)
		*(.data)
	} : data
	_gp = ABSOLUTE(.);
        . = ALIGN(4);
	.bss . : {
		*(.sbss)
		*(.bss)
		*(COMMON)
	}

	/DISCARD/ : { *(.comment .pdr .note .debug_*) }
}
