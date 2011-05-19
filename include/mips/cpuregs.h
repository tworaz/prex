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

#define	MIPS_KUSEG			0x0
#define	MIPS_KSEG0			0x80000000
#define	MIPS_KSEG1			0xa0000000
#define	MIPS_KSEG2			0xc0000000
#define	MIPS_MAX_MEM_ADDR		0xbe000000
#define	MIPS_RESERVED_ADDR		0xbfc80000

#define	MIPS_PHYS_MASK			0x1fffffff

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

/*
 * MIPS32 coprocessor 0 status register bits
 */
#define MIPS_STATUS_COP_USABILITY 0xf0000000
#define MIPS_STATUS_RP            0x08000000
#define MIPS_STATUS_FR            0x04000000
#define MIPS_STATUS_RE            0x02000000
#define MIPS_STATUS_MX            0x01000000
#define MIPS_STATUS_PX            0x00800000
#define MIPS_STATUS_BEV           0x00400000
#define MIPS_STATUS_TS            0x00200000
#define MIPS_STATUS_SR            0x00100000
#define MIPS_STATUS_NMI           0x00080000
#define MIPS_STATUS_INT_MASK      0x0000ff00
#define MIPS_STATUS_KX            0x00000080
#define MIPS_STATUS_SX            0x00000040
#define MIPS_STATUS_UX            0x00000020
#define MIPS_STATUS_KSU_MASK      0x00000018
#define MIPS_STATUS_KSU_USER      0x00000010
#define MIPS_STATUS_KSU_SUPER     0x00000008
#define MIPS_STATUS_KSU_KERNEL    0x00000000
#define MIPS_STATUS_ERL           0x00000004
#define MIPS_STATUS_EXL           0x00000002
#define MIPS_STATUS_INT_IE        0x00000001

/*
 * MIPS32 coprocessor 0 cause register bits
 */
#define	MIPS_CAUSE_BR_DELAY       0x80000000
#define MIPS_CAUSE_TI             0x40000000
#define MIPS_CAUSE_CE_MASK        0x30000000
#define MIPS_CAUSE_DC             0x08000000
#define MIPS_CAUSE_PCI            0x04000000
#define MIPS_CAUSE_IV             0x00800000
#define MIPS_CAUSE_WP             0x00400000
#define MIPS_CAUSE_IP_MASK        0x0000ff00
#define MIPS_CAUSE_EXC_CODE_MASK  0x0000007c
#define MIPS_CAUSE_EXC_CODE_SHIFT 2

/*
 * MIPS32 exception codes
 */
#define MIPS_CAUSE_EXC_CODE_IRQ         0
#define MIPS_CAUSE_EXC_CODE_MOD         1
#define MIPS_CAUSE_EXC_CODE_TLBL        2
#define MIPS_CAUSE_EXC_CODE_TLBS        3
#define MIPS_CAUSE_EXC_CODE_ADEL        4
#define MIPS_CAUSE_EXC_CODE_ADES        5
#define MIPS_CAUSE_EXC_CODE_IBE         6
#define MIPS_CAUSE_EXC_CODE_DBE         7
#define MIPS_CAUSE_EXC_CODE_SYS         8
#define MIPS_CAUSE_EXC_CODE_BP          9
#define MIPS_CAUSE_EXC_CODE_RI         10
#define MIPS_CAUSE_EXC_CODE_CPU        11
#define MIPS_CAUSE_EXC_CODE_OV         12
#define MIPS_CAUSE_EXC_CODE_TRAP       13
#define MIPS_CAUSE_EXC_CODE_FPE        15
#define MIPS_CAUSE_EXC_CODE_C2E        18
#define MIPS_CAUSE_EXC_CODE_MDMX       22
#define MIPS_CAUSE_EXC_CODE_WATCH      23
#define MIPS_CAUSE_EXC_CODE_MCHECK     24
#define MIPS_CAUSE_EXC_CODE_THREAD     25
#define MIPS_CAUSE_EXC_CODE_DSP        26
#define MIPS_CAUSE_EXC_CODE_CACHE_ERR  30

#endif /* _MIPS_CPUREGS_H */
