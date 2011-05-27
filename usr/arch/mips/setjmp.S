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

#include <machine/asm.h>
#include <machine/setjmp.h>

	.text
	.set noreorder

/*
 * C library -- setjmp, longjmp
 *
 *	longjmp(a,v)
 * will generate a "return(v)" from the last call to
 *	setjmp(a)
 * by restoring registers from the stack.
 * The previous signal state is restored.
 */
ENTRY(setjmp)
	addiu	sp, sp, -32
	sw	ra, 28(sp)
	sw	s8, 24(sp)
	sw	a0, 32(sp)

	/* Get the signal mask */
	move	a0, zero
	jal	_C_LABEL(sigblock)
	nop

	/* Restore ra and a0 */
	lw	a0, 32(sp)
	lw	ra, 28(sp)
	lw	s8, 24(sp)
	addiu	sp, sp, 32

	li	v1, _JB_MAGIC_SETJMP
	sw	v1, 0(a0) /* setjmp magic */
	sw	ra, 4(a0)
	sw	v0, 8(a0) /* Old signal mask */
	sw	s0, 16(a0)
	sw	s1, 20(a0)
	sw	s2, 24(a0)
	sw	s3, 28(a0)
	sw	s4, 32(a0)
	sw	s5, 36(a0)
	sw	s6, 40(a0)
	sw	s7, 44(a0)
	sw	sp, 48(a0)
	sw	s8, 52(a0)

	jr	ra
	move	v0, zero
END(setjmp)


ENTRY(longjmp)
	/* Check setjmp magic */
	lw	v0, 0(a0)
	li	t0, _JB_MAGIC_SETJMP
	bne	v0, t0, botch

	/* Load values from jmp_buf */
	lw	ra, 4(a0)
	lw	s0, 16(a0)
	lw	s1, 20(a0)
	lw	s2, 24(a0)
	lw	s3, 28(a0)
	lw	s4, 32(a0)
	lw	s5, 36(a0)
	lw	s6, 40(a0)
	lw	s7, 44(a0)
	lw	sp, 48(a0)
	lw	s8, 52(a0)

	addiu	sp, sp, -32
	sw	ra, 28(sp)
	sw	s8, 24(sp)
	sw	a1, 36(sp)

	/* Fetch signal mask */
	lw	a0, 8(a0)

	/* Restore signal mask */
	jal	_C_LABEL(sigsetmask)
	nop

	lw	ra, 28(sp)
	lw	s8, 24(sp)
	lw	v0, 36(sp)

	jr	ra
	addiu	sp, sp, 36

botch:
	jal	_C_LABEL(abort)
	nop
	j botch
	nop
END(longjmp)

	.set reorder
