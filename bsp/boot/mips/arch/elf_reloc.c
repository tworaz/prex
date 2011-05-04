/*-
 * Copyright (c) 2010, Peter Tworek
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of any co-contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/param.h>
#include <sys/elf.h>
#include <boot.h>

#define HI16_RELOC_CNT 4

struct reloc16hi {
	Elf32_Addr *addr;
	Elf32_Addr val;
};

struct reloc16hi r16hi_tmp[HI16_RELOC_CNT] = 
        {{NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}};

int
relocate_rel(Elf32_Rel *rel, Elf32_Addr sym_val, char *target_sect)
{
	Elf32_Addr *where;
	Elf32_Sword addend, addend_lo;
	int32_t i = 0;
	
        ELFDBG(("INFO: rel = %lx, target_sect = %lx, rel->r_offset = %lx\n",
                (long)rel, (long)target_sect, (long)rel->r_offset));
	where = (Elf32_Addr *)(target_sect + rel->r_offset);
	
	switch (ELF32_R_TYPE(rel->r_info)) {
		case R_MIPS_NONE:
			break;
		case R_MIPS_32:
			*where += sym_val;
			break;
		case R_MIPS_26:
			if (sym_val % 4)
				panic("R_MIPS_26: Unaligned symbol relocation");
			if ((sym_val & 0xf0000000) != 
			    ((Elf32_Addr)(where + 4) & 0xf0000000))
				panic("R_MIPS_26: Relocation overflow");
			
			*where = (*where & ~0x03ffffff) | 
			         ((*where + (sym_val >> 2)) & 0x03ffffff);
			ELFDBG(("R_MIPS_26: sym_val = %lx, %lx -> %lx\n",
			        (long)sym_val, (long)where, (long)*where));
			break;
		case R_MIPS_HI16:
			for (i = 0; i < HI16_RELOC_CNT; i++) {
				if (r16hi_tmp[i].addr == 0) {
					r16hi_tmp[i].addr = where;
					r16hi_tmp[i].val  = sym_val;
					return 0;
				}
			}
			panic("To many consecutive R_MIPS_HI16 relocs");
			break;
		case R_MIPS_LO16:
			/* Sign extended addend from LO16 */
			addend_lo = ((*where & 0xffff) ^ 0x8000) - 0x8000;
			
			while (r16hi_tmp[i].addr != NULL) {
				/* HI16 relocation */
				addend = ((*r16hi_tmp[i].addr) << 16) + addend_lo;
				addend += sym_val;
				
				/* Account for the sign extension in the low bits */
				addend = ((addend >> 16) + ((addend & 0x8000) != 0)) & 0xffff;
				*r16hi_tmp[i].addr = ((*r16hi_tmp[i].addr) & ~0xffff) | addend;
				
				ELFDBG(("R_MIPS_HI16: sym_val = %lx, %lx -> %lx\n",
				         (long)sym_val, (long)r16hi_tmp[i].addr, 
				         (long)*r16hi_tmp[i].addr));
				if (++i == HI16_RELOC_CNT)
					break;
			}
			/* Time for LO16 */
			
			addend = addend_lo + sym_val;
			*where = (*where & ~0xffff) | (addend & 0xffff);
			
			memset(&r16hi_tmp, 0, HI16_RELOC_CNT * sizeof(struct reloc16hi));
			ELFDBG(("R_MIPS_LO16: sym_val = %lx, %lx -> %lx\n",
			        (long)sym_val, (long)where, (long)*where));
			break;
		default:
			ELFDBG(("Unkown relocation type=%d\n",
			        ELF32_R_TYPE(rel->r_info)));
			panic("relocation fail");
			return -1;
	}
	return 0;
}

int
relocate_rela(Elf32_Rela *rela, Elf32_Addr sym_val, char *target_sec)
{
	panic("invalid relocation type");
	return -1;
}
