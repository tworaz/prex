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
 * cpufunc.S - MIPS specific CPU functions
 */

#include <conf/config.h>
#include <machine/asm.h>
#include <cpu.h>

	.section ".text","ax"
	.set noreorder
	.set mips32

/*
 * Sit idle doing nothing until some interrupt arrives
 */
ENTRY(cpu_idle)
	wait
        jr	ra
        nop

/*
 * Disable interrupts by clearing IE bit in SR
 */
ENTRY(irq_off)
	mfc0	t0, COP_0_STATUS
	nop
	and	t0, t0, ~0x1
	mtc0	t0, COP_0_STATUS
	jr	ra
	nop

/*
 * Enable interrupts by setting IE bit in SR
 */
ENTRY(irq_on)
	mfc0	t0, COP_0_STATUS
	nop
	or	t0, t0, 0x1
	mtc0	t0, COP_0_STATUS
	jr	ra
	nop

	.set reorder
	.set mips0
