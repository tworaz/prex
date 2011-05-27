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
 * copy.S - MIPS specific copy routines
 */

#include <conf/config.h>
#include <sys/errno.h>
#include <machine/asm.h>
#include <machine/memory.h>

	.section ".text","ax"
	.set noreorder

/*
 * Copy data from user to kernel space.
 * Returns 0 on success, or EFAULT on page fault.
 *
 *  syntax - int copyin(const void *uaddr, void *kaddr, size_t len)
 */
ENTRY(copyin)
	li	t0, USERLIMIT
	add	t1, a0, a2

	blt	t1, t0, 1f
	nop
	j	ra
	li	v0, EFAULT
1:
	lb	t3, 0(a0)
known_fault1:
	addiu	a2, a2, -1
	sb	t3, 0(a1)
	addiu	a0, a0, 1
	bne	a2, zero, 1b
	addiu	a1, a1, 1

	j	ra
	move	v0, zero
END(copyin)

/*
 * Copy data to user from kernel space.
 * Returns 0 on success, or EFAULT on page fault.
 *
 *  syntax - int copyout(const void *kaddr, void *uaddr, size_t len)
 */
ENTRY(copyout)
	li	t0, USERLIMIT
	add	t1, a1, a2

	blt	t1, t0, 1f
	nop
	j	ra
	li	v0, EFAULT
1:
	lb	t3, 0(a0)
known_fault2:
	addiu	a2, a2, -1
	sb	t3, 0(a1)
	addiu	a0, a0, 1
	bne	a2, zero, 1b
	addiu	a1, a1, 1

	j	ra
	move	v0, zero
END(copyout)

/*
 * copyinstr - Copy string from user space.
 * Returns 0 on success, or EFAULT on page fault, or ENAMETOOLONG.
 *
 *  syntax - int copyinstr(const char *uaddr, void *kaddr, size_t len);
 */
ENTRY(copyinstr)
	li	t0, USERLIMIT
	add	t1, a0, a2

	blt	t1, t0, 1f
	nop
	j	ra
	li	v0, EFAULT
1:
	lbu	t3, 0(a0)
known_fault3:
	addiu	a2, a2, -1
	beq	t3, zero, 2f
	sb	t3, 0(a1)
	addiu	a0, a0, 1
	bne	a2, zero, 1b
	addiu	a1, a1, 1

	j	ra
	li	v0, ENAMETOOLONG
2:
	j	ra
	move	v0, zero
END(copyinstr)

	.set reorder
