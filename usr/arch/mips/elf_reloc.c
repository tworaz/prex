/*-
 * Copyright (c) 2011, Peter Tworek
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
#include <sys/elf.h>
#include <sys/syslog.h>
#include <stdlib.h>


struct reloc16hi {
	Elf32_Addr *addr;
	Elf32_Addr val;
	struct reloc16hi* prev;
};

struct reloc16hi *reloc_head = NULL;

int
relocate_rel(Elf32_Rel *rel, Elf32_Addr sym_val, char *target_sect)
{
	Elf32_Addr *where;
	Elf32_Sword addend, addend_lo;
	struct reloc16hi* rtmp;

	where = (Elf32_Addr *)(target_sect + rel->r_offset);

	switch (ELF32_R_TYPE(rel->r_info)) {
		case R_MIPS_NONE:
			break;
		case R_MIPS_32:
			*where += sym_val;
			break;
		case R_MIPS_26:
			if (sym_val % 4) {
				syslog(LOG_ERR, "R_MIPS_26: Unaligned symbol relocation\n");
				return -1;
			}
			if ((sym_val & 0xf0000000) !=
			    ((Elf32_Addr)(where + 4) & 0xf0000000)) {
				syslog(LOG_ERR, "R_MIPS_26: Relocation overflow\n");
				return -1;
			}

			*where = (*where & ~0x03ffffff) |
			         ((*where + (sym_val >> 2)) & 0x03ffffff);
			break;
		case R_MIPS_HI16:
			rtmp = malloc(sizeof(struct reloc16hi));
			if (NULL == rtmp) {
				syslog(LOG_CRIT, "OOM: Relocation failed!\n");
				return -1;
			}
			rtmp->addr = where;
			rtmp->val = sym_val;
			rtmp->prev = reloc_head;
			reloc_head = rtmp;
			break;
		case R_MIPS_LO16:
			/* Sign extended addend from LO16 */
			addend_lo = ((*where & 0xffff) ^ 0x8000) - 0x8000;
			while(NULL != reloc_head) {
				rtmp = reloc_head;
				reloc_head = rtmp->prev;

				/* HI16 relocation */
				addend = ((*rtmp->addr) << 16) + addend_lo;
				addend += sym_val;

				/* Account for the sign extension in the low bits */
				addend = ((addend >> 16) + ((addend & 0x8000) != 0)) & 0xffff;
				*(rtmp->addr) = ((*(rtmp->addr)) & ~0xffff) | addend;

				free(rtmp);
			}

			/* Time for LO16 */
			addend = addend_lo + sym_val;
			*where = (*where & ~0xffff) | (addend & 0xffff);

			break;
		default:
#ifdef DEBUG
			syslog(LOG_ERR, "Unsupported relocation type=%d\n",
			       ELF32_R_TYPE(rel->r_info));
#endif
			return -1;
	}
	return 0;
}

int
relocate_rela(Elf32_Rela *rela, Elf32_Addr sym_val, char *target_sec)
{
	/* printf("Invalid relocation type\n"); */
	return -1;
}
