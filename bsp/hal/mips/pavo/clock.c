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
 * clock.c - clock driver for Ingenic JZ47xx
 */

#include <kernel.h>
#include <timer.h>
#include <irq.h>
#include <cpufunc.h>
#include <sys/ipl.h>

#include "platform.h"

/*
 * Clock interrupt service routine.
 * No H/W reprogram is required.
 */
static int
clock_isr(void *arg)
{
	splhigh();
	timer_handler();
	spl0();
	return INT_DONE;
}

/*
 * Initialize clock H/W chip.
 * Setup clock tick rate and install clock ISR.
 */
void
clock_init(void)
{
	irq_t clock_irq;
	uint16_t tmp16;

	/* Enable TCU clock */
	JZ_CPM_CLK_GATE &= ~(1 << JZ_CPM_CLK_GATE_TCU);

	/* Stop counter 0 */
	JZ_TCU_ENABLE_CLEAR = (1 << 0);

	tmp16  = JZ_TCU_CONTROL(0);
	/* Select EXTAL as the timer clock input */
	tmp16 &= ~(JZ_TCU_CONTROL_EXT_EN | JZ_TCU_CONTROL_RTC_EN |
	           JZ_TCU_CONTROL_PCK_EN | JZ_TCU_CONTROL_PWM_EN);
	tmp16 |= JZ_TCU_CONTROL_EXT_EN;
	/* Set Prescale CLK/4 */
	tmp16 &= ~(JZ_TCU_CONTROL_PRESCALE_MASK);
	tmp16 |= (1 << JZ_TCU_CONTROL_PRESCALE_BIT);
	JZ_TCU_CONTROL(0) = tmp16;

	/* Set full data */
	JZ_TCU_DATA_FULL(0) = CONFIG_JZ_EXTAL / HZ / 4;
	/* Mask half-match IRQ */
	JZ_TCU_MASK_SET = (1 << 16);
	/* Unmask full-match IRQ */
	JZ_TCU_MASK_CLEAR = (1 << 0);
	/* Clear full match flag */
	JZ_TCU_FLAG_CLEAR = (1 << 0);
	/* Clear counter value */
	JZ_TCU_COUNT(0) = 0;

	/* Install ISR */
	clock_irq = irq_attach(JZ_IRQ_TCU0, IPL_CLOCK, 0, &clock_isr,
			       IST_NONE, NULL);
	ASSERT(clock_irq != NULL);

	/* Enable timer */
	JZ_TCU_ENABLE_SET = (1 << 0);

	DPRINTF(("Clock rate: %d ticks/sec\n", CONFIG_HZ));
}

