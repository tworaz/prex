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

#ifndef _MIPS_LOCORE_H
#define _MIPS_LOCORE_H

#include <sys/cdefs.h>
#include <context.h>

#ifndef __ASSEMBLY__

__BEGIN_DECLS
void	kernel_thread_entry(void);
void	cpu_switch(struct kern_regs *, struct kern_regs *);
void	vector_copy(paddr_t);
void	exception_entry(void);
void	syscall_ret(void);
__END_DECLS

#else

/*
 * Macro to store struct cpu_regs on the stack
 * k0 = stack pointer to actualy save
 */
.macro CONTEXT_SAVE
	.set push
	.set noat
	.set reorder
	sub	sp, sp, CTXREGS
	sw	ra, CTX_RA(sp)
	sw	s8, CTX_S8(sp)
	sw	k0, CTX_SP(sp)
	sw	gp, CTX_GP(sp)
	sw	t9, CTX_T9(sp)
	sw	t8, CTX_T8(sp)
	sw	s7, CTX_S7(sp)
	sw	s6, CTX_S6(sp)
	sw	s5, CTX_S5(sp)
	sw	s4, CTX_S4(sp)
	sw	s3, CTX_S3(sp)
	sw	s2, CTX_S2(sp)
	sw	s1, CTX_S1(sp)
	sw	s0, CTX_S0(sp)
	sw	t7, CTX_T7(sp)
	sw	t6, CTX_T6(sp)
	sw	t5, CTX_T5(sp)
	sw	t4, CTX_T4(sp)
	sw	t3, CTX_T3(sp)
	sw	t2, CTX_T2(sp)
	sw	t1, CTX_T1(sp)
	sw	t0, CTX_T0(sp)
	sw	a3, CTX_A3(sp)
	sw	a2, CTX_A2(sp)
	sw	a1, CTX_A1(sp)
	sw	a0, CTX_A0(sp)
	sw	v1, CTX_V1(sp)
	sw	v0, CTX_V0(sp)
	sw	AT, CTX_AT(sp)
	mfhi	t0
	mflo	t1
	sw	t0, CTX_HI(sp)
	sw	t1, CTX_LO(sp)
	mfc0	t2, COP_0_CAUSE
	sw	t2, CTX_CAUSE(sp)
	mfc0	t3, COP_0_STATUS
	sw	t3, CTX_STATUS(sp)
	mfc0	t4, COP_0_BADVADDR
	sw	t4, CTX_VADDR(sp)
	mfc0	t5, COP_0_EPC
	sw	t5, CTX_EPC(sp)
	.set pop
.endm

/*
 * Macro to restore struct cpu_regs from the stack
 */
.macro CONTEXT_RESTORE
	.set push
	.set noat
	.set reorder
	lw	t0, CTX_EPC(sp)
	lw	t1, CTX_STATUS(sp)
	lw	t2, CTX_LO(sp)
	lw	t3, CTX_HI(sp)
	mtc0	t0, COP_0_EPC
	mtlo	t2
	mtc0	t1, COP_0_STATUS
	mthi	t3
	/* No need to restore BADVADDR and CAUSE */
	lw	AT, CTX_AT(sp)
	lw	v0, CTX_V0(sp)
	lw	v1, CTX_V1(sp)
	lw	a0, CTX_A0(sp)
	lw	a1, CTX_A1(sp)
	lw	a2, CTX_A2(sp)
	lw	a3, CTX_A3(sp)
	lw	t0, CTX_T0(sp)
	lw	t1, CTX_T1(sp)
	lw	t2, CTX_T2(sp)
	lw	t3, CTX_T3(sp)
	lw	t4, CTX_T4(sp)
	lw	t5, CTX_T5(sp)
	lw	t6, CTX_T6(sp)
	lw	t7, CTX_T7(sp)
	lw	s0, CTX_S0(sp)
	lw	s1, CTX_S1(sp)
	lw	s2, CTX_S2(sp)
	lw	s3, CTX_S3(sp)
	lw	s4, CTX_S4(sp)
	lw	s5, CTX_S5(sp)
	lw	s6, CTX_S6(sp)
	lw	s7, CTX_S7(sp)
	lw	t8, CTX_T8(sp)
	lw	t9, CTX_T9(sp)
	lw	gp, CTX_GP(sp)
	lw	s8, CTX_S8(sp)
	lw	ra, CTX_RA(sp)
	/* Stack goes last */
	lw	sp, CTX_SP(sp)
	.set pop
.endm

#endif /* __ASSEMBLY */

#endif /* !_MIPS_LOCORE_H */
