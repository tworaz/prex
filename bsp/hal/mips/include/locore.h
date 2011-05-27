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

#include <conf/config.h>
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
 * Bit from coprocesor 0 status register
 * that should actually be saved
 */
#define STATUS_SAVE_MASK 0x1f

/*
 * Macro to store struct cpu_regs in memory
 * pointed by the argument register
 * k0 = stack pointer to actualy save
 */
.macro CONTEXT_SAVE r
	.set push
	.set noat
	.set noreorder
	sw	ra, CTX_RA(\r)
	sw	s8, CTX_S8(\r)
	sw	k0, CTX_SP(\r)
	sw	gp, CTX_GP(\r)
	sw	t9, CTX_T9(\r)
	sw	t8, CTX_T8(\r)
	sw	s7, CTX_S7(\r)
	sw	s6, CTX_S6(\r)
	sw	s5, CTX_S5(\r)
	sw	s4, CTX_S4(\r)
	sw	s3, CTX_S3(\r)
	sw	s2, CTX_S2(\r)
	sw	s1, CTX_S1(\r)
	sw	s0, CTX_S0(\r)
	sw	t7, CTX_T7(\r)
	sw	t6, CTX_T6(\r)
	sw	t5, CTX_T5(\r)
	sw	t4, CTX_T4(\r)
	sw	t3, CTX_T3(\r)
	sw	t2, CTX_T2(\r)
	sw	t1, CTX_T1(\r)
	sw	t0, CTX_T0(\r)
	sw	a3, CTX_A3(\r)
	sw	a2, CTX_A2(\r)
	sw	a1, CTX_A1(\r)
	sw	a0, CTX_A0(\r)
	sw	v1, CTX_V1(\r)
	sw	v0, CTX_V0(\r)
	sw	AT, CTX_AT(\r)

	mfhi	AT
	sw	AT, CTX_HI(\r)
	mflo	AT
	sw	AT, CTX_LO(\r)

	mfc0	t0, COP_0_STATUS
	mfc0	t1, COP_0_EPC

	/* save only KSU, EXL, ERL, IE */
	andi	t2, t0, STATUS_SAVE_MASK

#ifdef CONFIG_MMU
	/* clear KSU, EXL, ERL, IE */
	li	t3, ~(STATUS_SAVE_MASK)
	and	t0, t0, t3
#endif /* CONFIG_MMU */

	sw	t2, CTX_STATUS(\r)
	sw	t1, CTX_EPC(\r)
	mtc0	t0, COP_0_STATUS
	.set pop
.endm

/*
 * Macro to restore struct cpu_regs from the stack
 */
.macro CONTEXT_RESTORE r
	.set push
	.set noat
	.set noreorder
	mfc0	t0, COP_0_STATUS
	lw	t1, CTX_STATUS(\r)

	/* Mask UM, EXL, ERL, IE */
	li	t2, ~(STATUS_SAVE_MASK)
	and	t0, t0, t2

	/* Copy UM, EXL, ERL, IE from saved status */
	or	t0, t0, t1
	mtc0	t0, COP_0_STATUS

	lw	t3, CTX_EPC(\r)
	lw	t4, CTX_LO(\r)
	lw	t5, CTX_HI(\r)
	mtc0	t3, COP_0_EPC
	mtlo	t4
	mthi	t5

	lw	AT, CTX_AT(\r)
	lw	v0, CTX_V0(\r)
	lw	v1, CTX_V1(\r)
	lw	a0, CTX_A0(\r)
	lw	a1, CTX_A1(\r)
	lw	a2, CTX_A2(\r)
	lw	a3, CTX_A3(\r)
	lw	t0, CTX_T0(\r)
	lw	t1, CTX_T1(\r)
	lw	t2, CTX_T2(\r)
	lw	t3, CTX_T3(\r)
	lw	t4, CTX_T4(\r)
	lw	t5, CTX_T5(\r)
	lw	t6, CTX_T6(\r)
	lw	t7, CTX_T7(\r)
	lw	s0, CTX_S0(\r)
	lw	s1, CTX_S1(\r)
	lw	s2, CTX_S2(\r)
	lw	s3, CTX_S3(\r)
	lw	s4, CTX_S4(\r)
	lw	s5, CTX_S5(\r)
	lw	s6, CTX_S6(\r)
	lw	s7, CTX_S7(\r)
	lw	t8, CTX_T8(\r)
	lw	t9, CTX_T9(\r)
	lw	gp, CTX_GP(\r)
	lw	s8, CTX_S8(\r)
	lw	ra, CTX_RA(\r)
	/* Stack goes last */
	lw	sp, CTX_SP(\r)
	.set pop
.endm

/*
 * Adjust given variable by val
 */
.macro ADJUST_VAR var val
	.set push
	.set noat
	.set noreorder
	la	k0, \var
	lw	k1, 0(k0)
	nop /* Load delay */
	addiu	k1, k1, \val
	sw	k1, 0(k0)
	.set pop
.endm

#endif /* __ASSEMBLY */

#endif /* !_MIPS_LOCORE_H */
