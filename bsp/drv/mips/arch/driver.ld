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
		*(.text.*)
	} : text = 0xff

	. = ALIGN(4);

	.rodata : {
		*(.rodata)
		*(.rodata.*)
	} : text

        . = ALIGN(4);

	.data . : {
		*(.data)
		*(.data.*)
		*(.data1)
		*(.data1.*)
		. = ALIGN(8);
		_gp = ABSOLUTE(.);
                *(.sdata)
                *(.sdata.*)
	} : data = 0xff

        . = ALIGN(4);

	.sbss . : {
		*(.sbss)
		*(.sbss.*)
		*(.dynsbss)
		*(.scommon)
	}

	. = ALIGN(4);

	.bss . : {
		*(.bss)
		*(.bss.*)
		*(.dynbss)
		*(COMMON)
	}

	/DISCARD/ : { *(.comment .pdr .note .debug_*) }
}
