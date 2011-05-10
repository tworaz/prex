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
 * cache.S - low level cache management routines
 */

#include <conf/config.h>
#include <machine/asm.h>
#include <mips/cacheops.h>

#ifdef CONFIG_CACHE

	.section ".text","ax"
	.set noreorder
	.set mips32

/*
 * Initialize CPU caches
 */
ENTRY(cache_init)
	mtc0	zero, COP_0_TAGLO
	mtc0	zero, COP_0_TAGHI

	/* Get D-cache params */
	mfc0	t9, COP_0_CONFIG, 1
	li	t7, 0x1
	srl	t9, t9, 7
	andi	t0, t9, 0x7  /* DA */
	srl	t9, t9, 3
	andi	t1, t9, 0x7  /* DL */
	beq	t1, zero, 9f
	srl	t9, t9, 3
	andi	t2, t9, 0x7  /* DS */

	addiu	t0, t0, 1  /* D-cache Assciativity */
	addiu	t1, t1, 1
	addiu	t2, t2, 6
	sllv	a0, t7, t1 /* D-cache line size */
	addu	t8, t1, t2
	sllv	a1, t7, t8
	multu	a1, t0

	/* Get I-cache params */
	srl	t9, t9, 3
	andi	t0, t9, 0x7 /* IA */
	srl	t9, t9, 3
	andi	t1, t9, 0x7 /* IL */
	srl	t9, t9, 3
	andi	t2, t9, 0x7 /* IS */

	addiu	t0, t0, 1  /* I-cache Assciativity */
	addiu	t1, t1, 1
	addiu	t2, t2, 6
	sllv	a2, t7, t1 /* I-cache line size */
	addu	t8, t1, t2
	sllv	a3, t7, t8
	mflo	a1         /* D-cache size */
	multu	a3, t0
	mflo	a3         /* I-cache size */

	/*Initialize caches */
	li	t0, MIPS_KSEG1
	add	t1, t0, a3
	or	t2, t0, t0
	add	t3, t2, a1
1:	/* I-cache */
	cache	Index_Store_Tag_I, 0(t0)
	bne	t0, t1, 1b
	addu	t0, t0, a2
2:	/* D-cache */
	cache	Index_Store_Tag_D, 0(t2)
	bne	t2, t3, 2b
	addu	t2, t2, a0

	/* Make kseg0 cachable */
	mfc0	t0, COP_0_CONFIG
	li	t1, 0xfffffff8
	and	t0, t0, t1
	or	t0, 0x00000003

	jr	ra
	mtc0	t0, COP_0_CONFIG

9:	/* Panic in case coprpcessor 0 config register reports
	 * there are not caches installed (DL == 0) */
	la	a0, cache_fail_str
	jal	panic
	nop

	.section ".data"
	.set reorder
	.set mips0

cache_fail_str:
	.asciz "Cache init failed!\n"

#endif /* CONFIG_CACHE */

