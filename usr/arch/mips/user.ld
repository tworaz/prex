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
	} : text = 0xff
	. = ALIGN(4);
	.rodata : {
		*(.rodata)
		*(.rodata.*)
	} : text
	. = ALIGN(0x1000);
	.data : {
                *(.sdata)
                *(.sdata2)
		*(.data)
	} : data
        _gp = ABSOLUTE(.);
	. = ALIGN(4);
	.bss .  : {
                *(.sbss)
		*(.bss)
		*(COMMON)
	}

	/DISCARD/ : { *(.comment .note) }
}
