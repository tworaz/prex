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
 * head.S - generic header code for MIPS platforms.
 */

#include <conf/config.h>

#include <machine/asm.h>
#include <machine/memory.h>
#include <machine/syspage.h>

#include <mips/cacheops.h>

#undef ENTRY
#define ENTRY(x) .global x; .align; x##:

	.section ".text","ax"
	.set noreorder
	.set mips32

/*
 * Loader entry point
 */
ENTRY(boot_entry)
        bal	1f
        li	k0, 0x1 /* 1 can come in handy */
1:
	/* Disable interrupts */
	li	t0, 0x0000FC00
	mtc0	t0, COP_0_STATUS
	li	t1, 0x00800000
	mtc0	t1, COP_0_CAUSE

        /* Make kseg0 uncachable */
        mfc0	t3, COP_0_CONFIG
        li	t4, 0xfffffff8
        and	t3, t3, t4
        ori	t3, 0x2
        mtc0	t3, COP_0_CONFIG

        /* Setup sp and gp */
        li	sp, BOOTSTK
	la	gp, _gp

        /* Get D-cache params */
        mfc0	t9, COP_0_CONFIG, 1
	nop
        srl	t9, t9, 7
        andi	t0, t9, 0x7  /* DA */
        srl	t9, t9, 3
        andi	t1, t9, 0x7  /* DL */
        beq	t1, zero, 4f /* Assumption: No D-cache == No I-cache */
        srl	t9, t9, 3
        andi	t2, t9, 0x7  /* DS */

        addiu	t0, t0, 1  /* D-cache Assciativity */
        addiu	t1, t1, 1
        addiu	t2, t2, 6
        sllv	s0, k0, t1 /* D-cache line size */
        addu	t8, t1, t2
	sllv	s1, k0, t8
        multu	s1, t0

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
        sllv	s2, k0, t1 /* I-cache line size */
        addu	t8, t1, t2
	sllv	s3, k0, t8
        mflo	s1         /* D-cache size */
        multu	s3, t0
        mflo	s3         /* I-cache size */

	/* Flush caches */
        li	t0, MIPS_KSEG1
        add	t1, t0, s3
        or	t2, t0, t0
        add	t3, t2, s1
2:	/* I-cache */
	cache	Index_Invalidate_I, 0(t0)
        bne	t0, t1, 2b
        addu	t0, t0, s2
3:	/* D-cache */
	cache	Index_Writeback_Inv_D, 0(t2)
        bne	t2, t3, 3b
        addu	t2, t2, s0
4:
        /* Copy prex image to proper location if not already there */
        addiu	a0, ra, -8 /* ra holds real boot_entry addr + 8 */
        li	a1, CONFIG_LOADER_TEXT
        lui	a2, 0x800A /* FIXME: real image size not 640K */
        la	t0, main
        subu	t1, t0, a0
        bne	a0, a1, copy_image
        addu	a3, a1, t1

	/* Jump to main */
	j	main
	nop

        /* 
         * a0 - start of the image in memory
         * a1 - destination
         * a2 - end of image
         * a3 - address to jump to after relocation
         */
copy_image:
        lw	t9, 0(a0)
        addiu	a0, a0, 4
        sw	t9, 0(a1)
        bne	a1, a2, copy_image
        addiu	a1, a1, 4

        /* Jump to real main */
        jr	a3
        nop

	.section .tail,"ax"
dummy:
	.byte	0xff

	.set mips0
	.set reorder

