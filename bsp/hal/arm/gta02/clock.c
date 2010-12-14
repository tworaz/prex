/*-
 * Copyright (c) 2010, Peter Tworek<tworaz666@gmail.com>
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
 * clock.c - clock driver for Samsung S3C2442
 */

#include <kernel.h>
#include <timer.h>
#include <irq.h>
#include <cpufunc.h>
#include <sys/ipl.h>

#include "platform.h"

/* Interrupt vector for timer (TMR4) */
#define CLOCK_IRQ	14

/* Timer 1 registers */
#define TMR_TCFG0	(*(volatile uint32_t *)(TIMER_BASE + 0x00))
#define TMR_TCFG1	(*(volatile uint32_t *)(TIMER_BASE + 0x04))
#define TMR_TCON	(*(volatile uint32_t *)(TIMER_BASE + 0x08))
#define TMR_TCNTB0	(*(volatile uint32_t *)(TIMER_BASE + 0x0C))
#define TMR_TCMPB0	(*(volatile uint32_t *)(TIMER_BASE + 0x10))
#define TMR_TCNTO0	(*(volatile uint32_t *)(TIMER_BASE + 0x14))
#define TMR_TCNTB1	(*(volatile uint32_t *)(TIMER_BASE + 0x18))
#define TMR_TCMPB1	(*(volatile uint32_t *)(TIMER_BASE + 0x1C))
#define TMR_TCNTO1	(*(volatile uint32_t *)(TIMER_BASE + 0x20))
#define TMR_TCNTB2	(*(volatile uint32_t *)(TIMER_BASE + 0x24))
#define TMR_TCMPB2	(*(volatile uint32_t *)(TIMER_BASE + 0x28))
#define TMR_TCNTO2	(*(volatile uint32_t *)(TIMER_BASE + 0x2C))
#define TMR_TCNTB3	(*(volatile uint32_t *)(TIMER_BASE + 0x30))
#define TMR_TCMPB3	(*(volatile uint32_t *)(TIMER_BASE + 0x34))
#define TMR_TCNTO3	(*(volatile uint32_t *)(TIMER_BASE + 0x38))
#define TMR_TCNTB4	(*(volatile uint32_t *)(TIMER_BASE + 0x3C))
#define TMR_TCNTO4	(*(volatile uint32_t *)(TIMER_BASE + 0x40))

extern uint32_t get_PCLK(void);

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

	/* Reset */
	TMR_TCFG0 = 0x0;
	TMR_TCFG1 = 0;
	TMR_TCON = 0x0;

	/* Install ISR */
	clock_irq = irq_attach(CLOCK_IRQ, IPL_CLOCK, 0, &clock_isr,
			       IST_NONE, NULL);
	ASSERT(clock_irq != NULL);

	/* set prescaler for Timer 4 to 10 */
	TMR_TCFG0 = 0x0A00;

	/* Setup counter value */
	TMR_TCNTB4 = get_PCLK()/(2 * 10 * HZ);

	/* auto load, manual update of Timer 4 */
	TMR_TCON = (TMR_TCON & ~0x0700000) | 0x600000;
	/* auto load, start Timer 4 */
	TMR_TCON = (TMR_TCON & ~0x0700000) | 0x500000;

	DPRINTF(("Clock rate: %d ticks/sec\n", CONFIG_HZ));
}

