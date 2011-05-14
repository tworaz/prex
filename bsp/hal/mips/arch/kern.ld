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
		*(.text.*)
	} : text = 0xff

	. = ALIGN(4);

	.vectors . : {
		_vector_start = .;
		*(.vector.000);
		. = 0x100;
		*(.vector.100);
		. = 0x180;
		*(.vector.180);
		. = 0x200;
		*(.vector.200);
		_vector_end = .;
	} :text

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

	__bss = .;
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
	. = ALIGN(4);
	__end = .;

	/DISCARD/ : { *(.comment .pdr .note) }
}
