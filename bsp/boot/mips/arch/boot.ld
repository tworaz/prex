/*
 * Linker script for boot loader
 */
INCLUDE config.ld
OUTPUT_FORMAT("elf32-littlemips", "elf32-bigmips", "elf32-littlemips")
OUTPUT_ARCH(mips)
SECTIONS
{
	. = CONFIG_LOADER_TEXT ;

	.text . : {
		*(.text)
		*(.text.*)
	}

	. = ALIGN(4);

	.rodata . : {
		*(.rodata)
		*(.rodata.*)
	}

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
	}

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

	. = CONFIG_LOADER_TEXT + 0x1fff;

	.tail : {
		*(.tail)
	}
}
