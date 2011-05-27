/*
 * Linker script for userspace
 */
OUTPUT_FORMAT("elf32-littlemips", "elf32-bigmips", "elf32-littlemips")
OUTPUT_ARCH(mips)
ENTRY(_start)
PHDRS
{
	text PT_LOAD FILEHDR PHDRS;
	data PT_LOAD;
}
SECTIONS
{
	. = 0x10000 + SIZEOF_HEADERS;

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

	.bss . : {
		*(.bss)
		*(.bss.*)
		*(.dynbss)
		*(COMMON)
		*(.sbss)
		*(.sbss.*)
		*(.dynsbss)
		*(.scommon)
	}

	. = ALIGN(4);

	/DISCARD/ : { *(.comment .note) }
}
