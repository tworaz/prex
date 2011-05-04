/*
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

#ifndef _MIPS_CPUREGS_H
#define _MIPS_CPUREGS_H

/*
 * Address space.
 * 32-bit mips CPUS partition their 32-bit address space into four segments:
 *
 * kuseg   0x00000000 - 0x7fffffff  User virtual mem,  mapped
 * kseg0   0x80000000 - 0x9fffffff  Physical memory, cached, unmapped
 * kseg1   0xa0000000 - 0xbfffffff  Physical memory, uncached, unmapped
 * kseg2   0xc0000000 - 0xffffffff  kernel-virtual,  mapped
 *
 * mips1 physical memory is limited to 512Mbytes, which is
 * doubly mapped in kseg0 (cached) and kseg1 (uncached.)
 * Caching of mapped addresses is controlled by bits in the TLB entry.
 */

#define	MIPS_KUSEG_START		0x0
#define	MIPS_KSEG0_START		0x80000000
#define	MIPS_KSEG1_START		0xa0000000
#define	MIPS_KSEG2_START		0xc0000000
#define	MIPS_MAX_MEM_ADDR		0xbe000000
#define	MIPS_RESERVED_ADDR		0xbfc80000

#define	MIPS_PHYS_MASK			0x1fffffff

#if 0
#define	MIPS_KSEG0_TO_PHYS(x)	((uintptr_t)(x) & MIPS_PHYS_MASK)
#define	MIPS_PHYS_TO_KSEG0(x)	((uintptr_t)(x) | MIPS_KSEG0_START)
#define	MIPS_KSEG1_TO_PHYS(x)	((uintptr_t)(x) & MIPS_PHYS_MASK)
#define	MIPS_PHYS_TO_KSEG1(x)	((uintptr_t)(x) | MIPS_KSEG1_START)
#endif /* __ASSEMBLER__ */

/*
 * Coprocessor 0 register names
 */
#define COP_0_INDEX 		$0
#define COP_0_RANDOM	 	$1
#define COP_0_ENTRYLO0 		$2
#define COP_0_ENTRYLO1 		$3
#define COP_0_CONF 		$3
#define COP_0_CONTEXT 		$4
#define COP_0_PAGEMASK 		$5
#define COP_0_WIRED 		$6
#define COP_0_INFO 		$7
#define COP_0_BADVADDR 		$8
#define COP_0_COUNT 		$9
#define COP_0_ENTRYHI 		$10
#define COP_0_COMPARE 		$11
#define COP_0_STATUS 		$12
#define COP_0_CAUSE 		$13
#define COP_0_EPC 		$14
#define COP_0_PRID 		$15
#define COP_0_CONFIG 		$16
#define COP_0_LLADDR 		$17
#define COP_0_WATCHLO 		$18
#define COP_0_WATCHHI 		$19
#define COP_0_XCONTEXT 		$20
#define COP_0_FRAMEMASK 	$21
#define COP_0_DIAGNOSTIC 	$22
#define COP_0_DEBUG 		$23
#define COP_0_DEPC 		$24
#define COP_0_PERFORMANCE 	$25
#define COP_0_ECC 		$26
#define COP_0_CACHEERR 		$27
#define COP_0_TAGLO 		$28
#define COP_0_TAGHI 		$29
#define COP_0_ERROREPC 		$30
#define COP_0_DESAVE 		$31

#endif /* _MIPS_CPUREGS_H */
