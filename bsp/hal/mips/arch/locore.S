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

/*
 * locore.S - low level platform support
 */

#include <conf/config.h>
#include <machine/asm.h>
#include <machine/syspage.h>
#include <machine/memory.h>
#include <locore.h>
#include <trap.h>
#include <cpu.h>

	.section ".text","ax"
	.set noreorder
	.set mips32

/*
 * Kernel entry point
 */
ENTRY(kernel_start)
	/* Disable interrupts */
	li	t0, MIPS_STATUS_INT_MASK
	mtc0	t0, COP_0_STATUS

	li	t1, MIPS_CAUSE_IV
	mtc0	t1, COP_0_CAUSE

	/* Setup sp and gp */
	li	sp, BOOTSTKTOP
	la	gp, _gp

	/* Clear kernel BSS */
	la      t2, __bss
	la      t3, __end
1:
	sw      zero, 0(t2)
	bne     t2, t3, 1b
	addiu   t2, t2, 4

	li	k1, SYSSTKTOP
	la	k0, curkstack
	sw	k1, 0(k0)

	/* Jump to main */
	j	main
	ehb	/* Just in case */
END(kernel_start)

/*
 * Entry point for kernel thread
 */
ENTRY(kernel_thread_entry)
	move	a0, s1	/* Set argument */
	j	s0	/* Jump to kernel entry point */
	nop
1:
	j	1b
	nop
END(kernel_thread_entry)

/*
 * Switch register context.
 * a0 = previous kern_regs, a1 = next kern_regs
 * Interrupts must be disabled by caller.
 *
 * syntax - void cpu_switch(kern_regs *prev, kern_regs *next)
 *
 */
ENTRY(cpu_switch)
	/* Get current kstack pointer */
	la	t0, curkstack
	/* lw	t1, 0(t0) */

	/* Save regs in prev */
	sw	ra, KREG_RA(a0)
	sw	sp, KREG_SP(a0)
	sw	gp, KREG_GP(a0)
	sw	s8, KREG_S8(a0)
	sw	s7, KREG_S7(a0)
	sw	s6, KREG_S6(a0)
	sw	s5, KREG_S5(a0)
	sw	s4, KREG_S4(a0)
	sw	s3, KREG_S3(a0)
	sw	s2, KREG_S2(a0)
	sw	s1, KREG_S1(a0)
	sw	s0, KREG_S0(a0)
	/* sw	t1, KREG_KSTACK(a0) */

	/* Load regs from next */
	lw	t1, KREG_KSTACK(a1)
	lw	ra, KREG_RA(a1)
	lw	sp, KREG_SP(a1)
	lw	gp, KREG_GP(a1)
	lw	s8, KREG_S8(a1)
	lw	s7, KREG_S7(a1)
	lw	s6, KREG_S6(a1)
	lw	s5, KREG_S5(a1)
	lw	s4, KREG_S4(a1)
	lw	s3, KREG_S3(a1)
	lw	s2, KREG_S2(a1)
	lw	s1, KREG_S1(a1)
	lw	s0, KREG_S0(a1)

	/* Jump to return address */
	j	ra
	sw	t1, 0(t0) /* Set new curkstack value */
END(cpu_switch)

/*
 * Relocate exception vector
 *
 * void vector_copy(paddr_t dest);
 */
ENTRY(vector_copy)
#if SYSPAGE != MIPS_KSEG0
	mtc0	a0, COP_0_PRID, 1 /* EBase */
#endif
	la	t0, _vector_start
	la	t1, _vector_end
1:
	lw	t3, 0(t0)
	addiu	t0, t0, 4
	sw	t3, 0(a0)
	bne	t0, t1, 1b
	addiu	a0, a0, 4

	jr	ra
	nop
END(vector_copy)

/*
 * Low level exception entry point.
 */
ENTRY(exception_entry)
	mfc0	k0, COP_0_CAUSE
	nop
	and	k0, k0, MIPS_CAUSE_EXC_CODE_MASK
	srl	k0, k0, MIPS_CAUSE_EXC_CODE_SHIFT
	sub	k0, k0, MIPS_CAUSE_EXC_CODE_SYS

	beq	k0, zero, syscall_entry
	move	k0, sp

	li	sp, SYSSTKTOP
	CONTEXT_SAVE

	move	a0, sp
	jal	trap_handler
	addiu	sp, sp, -16
	addiu	sp, sp, 16

	/* Restore previous context */
	CONTEXT_RESTORE

	/* Return to user mode */
	eret
END(exception_entry)

/*
 * Low level syscall entry point
 */
	.global syscall_ret
ENTRY(syscall_entry)
	/* Set sp to current thread kstack */
	la	k0, curkstack
	lw	k1, 0(k0)
	move	k0, sp
	move	sp, k1

	/* Save CPU registers on the stack */
	CONTEXT_SAVE

	/*
	 * Leave EXL bit set in status register
	 * This way interrupts can happen during
	 * kernel part of the syscall
	 */

	/* Call syscall handler */
	addiu	sp, sp, -20
	jal	syscall_handler
	sw	v0, 16(sp)
	addiu	sp, sp, 20

	lw	s1, CTX_EPC(sp)
	addiu	s1, s1, 4
	sw	s1, CTX_EPC(sp)

	/*
	 * Skip storing return value for syscall
	 * if it was exception_return(0)
	 */
	lw	s0, CTX_V0(sp)
	nop
	beq	s0, zero, syscall_ret
	nop

	sw	v0, CTX_V0(sp)
syscall_ret:
	/* Restore CPU registers from the stack */
	CONTEXT_RESTORE

	eret
END(syscall_entry)

/*
 * Low lever interrupt entry point
 */
ENTRY(interrupt_entry)
	/* Increment irq nesting variable */
	la	k0, irq_nesting
	lw	k1, 0(k0)
	nop
	addiu	k1, k1, 1
	sw	k1, 0(k0)
	addiu	k1, k1, -1

	bgtz	k1, 1f
	move	k0, sp /* Save original sp for CONTEXT_SAVE */
	
	/* If irq_nesting is 0 set sp to curkstack */
	la	k1, curkstack
	j	2f
	lw	sp, 0(k1)

1:	/*
	 * If irq_nesting is 1 set sp to INTSTKTOP
	 * else leave as it is.
	 */
	sltiu	k1, k1, 2
	beq	k1, zero, 2f
	nop
	li	sp, INTSTKTOP
2:
	/* Save CPU registers on the stack */
	CONTEXT_SAVE

	/* Get initial irq_nesting value */
	la	t0, irq_nesting
	lw	s0, 0(t0)
	addiu	s0, s0, -1

	/*
	 * Clear EXL bit to allow further irq nesting.
	 * Clear IE bit to disable interrupts for now.
	 */
	and	t3, t3, ~(MIPS_STATUS_EXL|MIPS_STATUS_INT_IE)
	bgtz	s0, 3f
	mtc0	t3, COP_0_STATUS

	/* Lock scheduler only if intial irq_nesting > 0 */
	jal	sched_lock
	nop
3:
	jal	interrupt_handler
	addiu	sp, sp, -16

	la	k0, irq_nesting
	sw	s0, 0(k0)

	/* Unlock scheduler only if initial irq_nesting == 0 */
	bgtz	s0, 4f
	addiu	sp, sp, 16

	jal	sched_unlock
	nop
4:
	/* Restore saved CPU registers from the stack */
	CONTEXT_RESTORE

	eret
END(interrupt_entry)

	.section ".vector.tlb", "ax", %progbits
/*
 * TLB Refill exception entry point
 */
	mfc0	k0, COP_0_EPC
	la	a0, tlb_rf_ent_todo
	jal	panic
	nop

	.section ".vector.cache", "ax", %progbits
/*
 * Cache error exception entry point
 */
	la	a0, cache_err_todo
	jal	panic
	nop

	.section ".vector.exc", "ax", %progbits
/*
 * General exception entry point.
 */
 	j	exception_entry
	nop

	.section ".vector.irq", "ax", %progbits
/*
 * Interrupt handler.
 */
	j	interrupt_entry
	nop

	.set reorder
	.section ".data"

tlb_rf_ent_todo:	.asciz "TODO: implement tlb_refill_entry\n"
cache_err_todo:		.asciz "TODO: implement cache_error_entry\n"

	.section ".bss"

/*
 * Interrupt nesting counter.
 *
 * This counter is incremented on every interrupt.
 * It's used to decide if stack should be switched.
 */
irq_nesting:	.word	0

/*
 * Kernel stack for the current thread
 */
curkstack:	.word	0
