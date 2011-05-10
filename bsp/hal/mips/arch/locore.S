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
#include <sys/errno.h>
#include <context.h>
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
	li	t0, 0x0000FC00
	mtc0	t0, COP_0_STATUS
	li	t1, 0x00800000
	mtc0	t1, COP_0_CAUSE

	/* Setup sp and gp */
	li	sp, SYSSTK
	la	gp, _gp

	/* Clear kernel BSS */
	la      t2, __bss
	la      t3, __end
1:
	sw      zero, 0(t2)
	bne     t2, t3, 1b
	addiu   t2, t2, 4

	/* Jump to main */
	j	main
	nop

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

/*
 * Copy data from user to kernel space.
 * Returns 0 on success, or EFAULT on page fault.
 *
 *  syntax - int copyin(const void *uaddr, void *kaddr, size_t len)
 */
ENTRY(copyin)
	la	a0, copyin_todo
	jal	panic
	nop

/*
 * Copy data to user from kernel space.
 * Returns 0 on success, or EFAULT on page fault.
 *
 *  syntax - int copyout(const void *kaddr, void *uaddr, size_t len)
 */
ENTRY(copyout)
	la	a0, copyout_todo
	jal	panic
	nop

/*
 * copyinstr - Copy string from user space.
 * Returns 0 on success, or EFAULT on page fault, or ENAMETOOLONG.
 *
 *  syntax - int copyinstr(const char *uaddr, void *kaddr, size_t len);
 */
ENTRY(copyinstr)
	la	a0, copyinstr_todo
	jal	panic
	nop

/*
 * Switch register context.
 * a0 = previous kern_regs, a1 = next kern_regs
 * Interrupts must be disabled by caller.
 *
 * syntax - void cpu_switch(kern_regs *prev, kern_regs *next)
 *
 */
ENTRY(cpu_switch)
	la	a0, cpu_switch_todo
	jal	panic
	nop

/*
 * TLB refill entry point
 */
ENTRY(tlb_refill_entry)
	la	a0, tlb_rf_ent_todo
	jal	panic
	nop

/*
 * Cache error entry point
 */
ENTRY(cache_error_entry)
	la	a0, cache_err_todo
	jal	panic
	nop

/*
 * Interrunt entry point
 */
ENTRY(interrupt_entry)
	la	a0, irq_ent_todo
	jal	panic
	nop

/*
 * System call entry
 */
	.global syscall_ret
ENTRY(syscall_entry)
	la	a0, syscall_ent_todo
	jal	panic
        nop
syscall_ret:
	la	a0, syscall_ret_todo
	jal	panic
	nop

/*
 * Relocate exception vector
 *
 * void vector_copy(paddr_t dest);
 */
ENTRY(vector_copy)
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

/*
 * Exception/Interrunt vectors
 */
	.section ".vector.000", "ax", %progbits
	j	tlb_refill_entry
	nop
	.section ".vector.100", "ax", %progbits
	j	cache_error_entry
	nop
	.section ".vector.180", "ax", %progbits
	j	interrupt_entry
	nop
	.section ".vector", "ax", %progbits
	j	interrupt_entry
	nop

	.set reorder
	.section ".data"

copyin_todo:		.asciz "TODO: implement copuin\n"
copyout_todo:		.asciz "TODO: implement copyout\n"
copyinstr_todo:		.asciz "TODO: implement copyinstr\n"
cpu_switch_todo:	.asciz "TODO: implement cpu_switch\n"
tlb_rf_ent_todo:	.asciz "TODO: implement tlb_refill_entry\n"
cache_err_todo:		.asciz "TODO: implement cache_error_entry\n"
irq_ent_todo:		.asciz "TODO: implement interrupt_entry"
syscall_ret_todo:	.asciz "TODO: implement syscal_reg\n"
syscall_ent_todo:	.asciz "TODO: implement syscall_entry\n"
