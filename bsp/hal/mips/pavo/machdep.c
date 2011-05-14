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
 * machdep.c - machine-dependent routines for Ingenic JZ47xx
 */

#include <machine/syspage.h>
#include <sys/power.h>
#include <sys/bootinfo.h>
#include <kernel.h>
#include <page.h>
#include <mmu.h>
#include <cpu.h>
#include <cpufunc.h>
#include <locore.h>
#include <cache.h>

#include "platform.h"

#ifdef CONFIG_MMU
/*
 * Virtual and physical address mapping
 *
 *      { virtual, physical, size, type }
 */
struct mmumap mmumap_table[] =
{
#if 0
	/*
	 * Internal SRAM (4K)
	 */
	{ 0x80000000, 0x00000000, 0x1000, VMT_RAM },

	/*
	 * Watchdog timer control (1K)
	 */
	{ 0xD0000000, 0x53000000, 0x400, VMT_IO },

	/*
	 * Counter/Timers (1K)
	 */
	{ 0xD3000000, 0x51000000, 0x400, VMT_IO },

	/*
	 * Interrupt controller (1K)
	 */
	{ 0xD4000000, 0x4A000000, 0x400, VMT_IO },

	/*
	 * Real-time clock (1K)
	 */
	{ 0xD5000000, 0x57000040, 0x400, VMT_IO },

	/*
	 * UART 2 (1K)
	 */
	{ 0xD6000000, 0x50008000, 0x400, VMT_IO },
#endif

	{ 0,0,0,0 }
};
#endif

/*
 * Idle
 */
void
machine_idle(void)
{
	cpu_idle();
}

/*
 * Reset system.
 */
static void
machine_reset(void)
{
        panic("TODO: implement machine_reset()");
#if 0
        /* Disable watchdog */
	WT_WTCON = 0x0000;

        /* Initialize watchdog timer count register */
        WT_WTCNT = 0x0001;

        /* Enable watchdog timer; assert reset at timer timeout */
        WT_WTCON = 0x0021;

	for (;;) ;
	/* NOTREACHED */
#endif
}

/*
 * Set system power
 */
void
machine_powerdown(int state)
{
	splhigh();

	DPRINTF(("Power down machine\n"));

	switch (state) {
	case PWR_OFF:
		for (;;)
			cpu_idle();
		/* NOTREACHED */
		break;
	case PWR_REBOOT:
		machine_reset();
		/* NOTREACHED */
		break;
	}
}

/*
 * Return pointer to the boot information.
 */
void
machine_bootinfo(struct bootinfo **bip)
{
	*bip = (struct bootinfo *)BOOTINFO;
}

void
machine_abort(void)
{
	for (;;)
		cpu_idle();
}

/*
 * Machine-dependent startup code
 */
void
machine_startup(void)
{
	/*
	 * Initialize CPU and basic hardware.
	 */
	cpu_init();

#ifdef CONFIG_CACHE
	/*
	 * Initialize CPU cache
	 */
	cache_init();
#endif

	/*
	 * Reserve system pages.
	 */
	page_reserve(kvtop(SYSPAGE), SYSPAGESZ);

	/*
	 * Setup vector page.
	 */
	vector_copy(kvtop(SYSPAGE));

#ifdef CONFIG_MMU
	panic("TODO: initialize MMU\n");
	/*
	 * Initialize MMU
	 */
	mmu_init(mmumap_table);
#endif
}
