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

#ifndef _MIPS_CONTEXT_H
#define _MIPS_CONTEXT_H

#ifndef __ASSEMBLY__
#include <sys/types.h>

/**
 * MIPS register reference:
 *
 *  Name    Number	MIPS Procedure Calling Standard Role
 *
 *  zero     0 		always return 0
 *  at       1		assembler temporary
 *  v0       2		value returned by subroutine
 *  v1       3		value returned by subroutine
 *  a0       4		1st parameter for a subroutine
 *  a1       5		2nd parameter for a subroutine
 *  a2       6		3rd parameter for a subroutine
 *  a3       7		4th parameter for a subroutine
 *  t0       8		1st subroutine temporary register
 *  t1       9		2nd subroutine temporary register
 *  t2      10		3rd subroutine temporary register
 *  t3      11		4th subroutine temporary register
 *  t4      12		5th subroutine temporary register
 *  t5      13		6th subroutine temporary register
 *  t6      14		7th subroutine temporary register
 *  t7      15		8th subroutine temporary register
 *  s0      16		1st subroutine saved register
 *  s1      17		2nd subroutine saved register
 *  s2      18		3rd subroutine saved register
 *  s3      19		4th subroutine saved register
 *  s4      20		5th subroutine saved register
 *  s5      21		6th subroutine saved register
 *  s6      22		7th subroutine saved register
 *  s7      23		8th subroutine saved register
 *  t8      24		9th subroutine remporary register
 *  t9      25		10th subroutine temporary register
 *  k0      26		1st kernel temporary register
 *  k1      27		2nd kernel temporary register
 *  gp      28		global pointer
 *  sp      29		stack pointer
 *  s8/fp   30		9th subroutine saved register / frame pointer
 *  ra	    31		subroutine return address
 */

/*
 * Common register frame for trap/interrupt.
 * These cpu state are saved into top of the kernel stack in
 * trap/interrupt entries. Since the arguments of system calls are
 * passed via registers, the system call library is completely
 * dependent on this register format.
 */
struct cpu_regs {
	uint32_t	epc;	/*   +0 (00) */
	uint32_t	vaddr;  /*   +4 (04) */
	uint32_t	status; /*   +8 (08) */
	uint32_t	cause;  /*  +12 (0C) */
	uint32_t	lo;     /*  +16 (10) */
	uint32_t	hi;     /*  +20 (14) */
	uint32_t	at;     /*  +24 (18) */
	uint32_t	v0;     /*  +28 (1C) */
	uint32_t	v1;     /*  +32 (20) */
	uint32_t	a0;     /*  +36 (24) */
	uint32_t	a1;     /*  +40 (28) */
	uint32_t	a2;     /*  +44 (2C) */
	uint32_t	a3;     /*  +48 (30) */
	uint32_t	t0;     /*  +52 (34) */
	uint32_t	t1;     /*  +56 (38) */
	uint32_t	t2;     /*  +60 (3C) */
	uint32_t	t3;     /*  +64 (40) */
	uint32_t	t4;     /*  +68 (44) */
	uint32_t	t5;     /*  +72 (48) */
	uint32_t	t6;     /*  +76 (4C) */
	uint32_t	t7;     /*  +80 (50) */
	uint32_t	s0;     /*  +84 (54) */
	uint32_t	s1;     /*  +88 (58) */
	uint32_t	s2;     /*  +92 (5C) */
	uint32_t	s3;     /*  +96 (60) */
	uint32_t	s4;     /* +100 (64) */
	uint32_t	s5;     /* +104 (68) */
	uint32_t	s6;     /* +108 (6C) */
	uint32_t	s7;     /* +112 (70) */
	uint32_t	t8;     /* +116 (74) */
	uint32_t	t9;     /* +120 (78) */
	uint32_t	gp;     /* +124 (7C) */
	uint32_t	sp;     /* +128 (80) */
	uint32_t	s8;     /* +132 (84) */
	uint32_t	ra;	/* +136 (88) */
};

/*
 * Kernel mode context for context switching.
 */
struct kern_regs {
	uint32_t	s0;	/*  +0 (00) */
	uint32_t	s1;	/*  +4 (04) */
	uint32_t	s2;	/*  +8 (08) */
	uint32_t	s3;	/* +12 (0C) */
	uint32_t	s4;	/* +16 (10) */
	uint32_t	s5;	/* +20 (14) */
	uint32_t	s6;	/* +24 (18) */
	uint32_t	s7;	/* +28 (1C) */
	uint32_t	s8;	/* +32 (20) */
	uint32_t	gp;	/* +36 (24) */
	uint32_t	sp;	/* +40 (28) */
	uint32_t	ra;	/* +44 (2C) */
	uint32_t	kstack;	/* +48 (30) */
};

/*
 * Processor context
 */
struct context {
	struct kern_regs kregs;		/* kernel mode registers */
	struct cpu_regs	*uregs;		/* user mode registers */
	struct cpu_regs	*saved_regs;	/* saved user mode registers */
};

typedef struct context *context_t;	/* context id */

#endif /* !__ASSEMBLY__ */

/*
 * Register offset in cpu_regs
 */
#define	CTX_EPC     0
#define	CTX_VADDR   4
#define	CTX_STATUS  8
#define	CTX_CAUSE  12
#define	CTX_LO     16
#define	CTX_HI     20
#define	CTX_AT     24
#define	CTX_V0     28
#define	CTX_V1     32
#define	CTX_A0     36
#define	CTX_A1     40
#define	CTX_A2     44
#define	CTX_A3     48
#define	CTX_T0     52
#define	CTX_T1     56
#define	CTX_T2     60
#define	CTX_T3     64
#define	CTX_T4     68
#define	CTX_T5     72
#define	CTX_T6     76
#define	CTX_T7     80
#define	CTX_S0     84
#define	CTX_S1     88
#define	CTX_S2     92
#define	CTX_S3     96
#define	CTX_S4    100
#define	CTX_S5    104
#define	CTX_S6    108
#define	CTX_S7    112
#define	CTX_T8    116
#define	CTX_T9    120
#define	CTX_GP    124
#define	CTX_SP    128
#define	CTX_S8    132
#define	CTX_RA    136

#define CTXREGS	  (4*35)

#define KREG_S0      0
#define KREG_S1      4
#define KREG_S2      8
#define KREG_S3     12
#define KREG_S4     16
#define KREG_S5     20
#define KREG_S6     24
#define KREG_S7     28
#define KREG_S8     32
#define KREG_GP     36
#define KREG_SP     40
#define KREG_RA     44
#define KREG_KSTACK 48

#define KREGS     (4*13)

#endif /* !_MIPS_CONTEXT_H */
