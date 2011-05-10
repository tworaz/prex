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
/*	uint32_t	zero; */
	uint32_t	at;	/*   +0 (00) */
	uint32_t	v0;	/*   +4 (04) */
	uint32_t	v1;	/*   +8 (08) */
	uint32_t	a0;	/*  +12 (0C) */
	uint32_t	a1;	/*  +16 (10) */
	uint32_t	a2;	/*  +20 (14) */
	uint32_t	a3;	/*  +24 (18) */
	uint32_t	t0;	/*  +28 (1C) */
	uint32_t	t1;	/*  +32 (20) */
	uint32_t	t2;	/*  +36 (24) */
	uint32_t	t3;	/*  +40 (28) */
	uint32_t	t4; 	/*  +44 (2C) */
	uint32_t	t5;	/*  +48 (30) */
	uint32_t	t6;	/*  +52 (34) */
	uint32_t	t7;	/*  +56 (38) */
	uint32_t	s0;	/*  +60 (3C) */
	uint32_t	s1;	/*  +64 (40) */
	uint32_t	s2;	/*  +68 (44) */
	uint32_t	s3;	/*  +72 (48) */
	uint32_t	s4;	/*  +76 (4C) */
	uint32_t	s5;	/*  +80 (50) */
	uint32_t	s6;	/*  +84 (54) */
	uint32_t	s7;	/*  +88 (58) */
	uint32_t	t8;	/*  +92 (5C) */
	uint32_t	t9;	/*  +96 (60) */
/*	uint32_t	ko; */
/*	uint32_t	k1; */
	uint32_t	gp;	/* +100 (64) */
	uint32_t	sp;	/* +104 (68) */
	uint32_t	fp;	/* +108 (6C) */
	uint32_t	ra;	/* +112 (70) */
	uint32_t	cause;  /* +116 (74) */
	uint32_t	epc;	/* +120 (78) */
	uint32_t	sr;	/* +124 (7C) */
};

/*
 * Kernel mode context for context switching.
 */
struct kern_regs {
	uint32_t	s0;
	uint32_t	s1;
	uint32_t	s2;
	uint32_t	s3;
	uint32_t	s4;
	uint32_t	s5;
	uint32_t	s6;
	uint32_t	s7;
	uint32_t	gp;
	uint32_t	sp;
	uint32_t	fp;
	uint32_t	ra;
};

/*
 * Processor context
 */
struct context {
	struct kern_regs kregs;		/* kernel mode registers */
	struct cpu_regs	*uregs;		/* user mode registers */
	struct cpu_regs	*saved_regs;	/* savecd user mode registers */
};

typedef struct context *context_t;	/* context id */

#endif /* !__ASSEMBLY__ */

/*
 * Register offset in cpu_regs
 */
#define REG_AT		0x00
#define REG_V0		0x04
#define REG_V1		0x08
#define REG_A0		0x0C
#define REG_A1		0x10
#define REG_A2		0x14
#define REG_A3		0x18
#define REG_T0		0x1C
#define REG_T1		0x20
#define REG_T2		0x24
#define REG_T3		0x28
#define REG_T4		0x2C
#define REG_T5		0x30
#define REG_T6		0x34
#define REG_T7		0x38
#define REG_S0		0x3C
#define REG_S1		0x40
#define REG_S2		0x44
#define REG_S3		0x48
#define REG_S4		0x4C
#define REG_S5		0x50
#define REG_S6		0x54
#define REG_S7		0x58
#define REG_T8		0x5C
#define REG_T9		0x60
#define REG_GP		0x64
#define REG_SP		0x68
#define REG_FP		0x6C
#define REG_RA		0x70
#define REG_CAUSE	0x74
#define REG_EPC		0x78
#define REG_SR		0x7c

#define CTXREGS		(4*32)

#endif /* !_MIPS_CONTEXT_H */
