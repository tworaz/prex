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
        .set mips32

/*
 * Kernel start point
 */
ENTRY(kernel_start)
	.set noreorder	

        /* 
         * At this point (done in bootldr):
         * - Exceptions are disabled,
         * - kseg0 is uncachable.
         */

        /* Setup sp and gp */
        li	sp, SYSSTK
        la	gp, _gp

        /* Make kseg0 cachable */
        mfc0	t0, COP_0_CONFIG
        li	t1, 0xfffffff8
        and	t0, t0, t1
        or	t0, 0x00000003
        mtc0	t0, COP_0_CONFIG

        /* Initialize caches */


	/* Jump to main */
	j	main
	nop
        .set reorder

/*
 * Copy data from user to kernel space.
 * Returns 0 on success, or EFAULT on page fault.
 *
 *  syntax - int copyin(const void *uaddr, void *kaddr, size_t len)
 */
ENTRY(copyin)
/* TODO: implement */
        nop
        j copyin
        nop

/*
 * Copy data to user from kernel space.
 * Returns 0 on success, or EFAULT on page fault.
 *
 *  syntax - int copyout(const void *kaddr, void *uaddr, size_t len)
 */
ENTRY(copyout)
/* TODO: implement */
        nop
        j	copyout
        nop

/*
 * copyinstr - Copy string from user space.
 * Returns 0 on success, or EFAULT on page fault, or ENAMETOOLONG.
 *
 *  syntax - int copyinstr(const char *uaddr, void *kaddr, size_t len);
 */
ENTRY(copyinstr)
/* TODO: implement */
        nop
        j	copyinstr
        nop
/*
 * int spl0(void);
 */
ENTRY(spl0)
/* TODO: implement */
        nop
        j	spl0
        nop

/*
 * int splhigh(void);
 */
ENTRY(splhigh)
/* TODO: implement */
        nop
        jr	ra
        nop

/*
 * void splx(int s);
 */
ENTRY(splx)
/* TODO: implement */
        jr	ra
        nop

/*
 * void sploff(void);
 */
ENTRY(sploff)
/* TODO: implement */
        nop
        j	sploff
        nop

/*
 * void splon(void);
 */
ENTRY(splon)
/* TODO: implement */
        nop
        j	splon
        nop

/*
 * Switch register context.
 * r0 = previous kern_regs, r1 = next kern_regs
 * Interrupts must be disabled by caller.
 *
 * syntax - void cpu_switch(kern_regs *prev, kern_regs *next)
 *
 */
ENTRY(cpu_switch)
        nop
        j	cpu_switch
        nop

/*
 * System call entry
 */
	.global syscall_ret
ENTRY(syscall_entry)
        nop
syscall_ret:
        nop
        j	syscall_entry
        nop

