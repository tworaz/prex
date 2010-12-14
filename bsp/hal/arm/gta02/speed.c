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
 * freq.c - CPU Frequency routines for Openmoko GTA02
 */

#include <types.h>
#include <conf/config.h>

#define CLK_BASE 	0x4C000000
#define MPLLCON 	(*(volatile uint32_t *)(CLK_BASE + 0x04))
#define UPLLCON 	(*(volatile uint32_t *)(CLK_BASE + 0x08))
#define CLKDIVN 	(*(volatile uint32_t *)(CLK_BASE + 0x14))
#define CAMDIVN 	(*(volatile uint32_t *)(CLK_BASE + 0x18))

/* static */ uint32_t
get_MPLL(void)
{
	uint32_t m, p, s;

	/* m = (MDIV + 8) */
	m = ((MPLLCON & 0xFF000) >> 12) + 8;
	/* p = (PDIV + 2) */
	p = ((MPLLCON & 0x3F0) >> 4) + 2;
	/* s = SDIV */
	s = (MPLLCON & 0x03);

	/* MPLL = (2 * m * Fin) / (p * 2^s) */
	return (2 * m * (CONFIG_SYS_CLK_FREQ / (p << s)));
}

/* static */ uint32_t
get_FCLK(void)
{
	return get_MPLL();
}

/* static */ uint32_t
get_HCLK(void)
{
	switch (CLKDIVN & 0x6) {
		case 0x0:
			return get_FCLK();
		case 0x2:
			return get_FCLK()/2;
		case 0x4:
			return (CAMDIVN & (1<<9)) ? get_FCLK()/8 : get_FCLK()/4;
		case 0x6:
			return (CAMDIVN & (1<<8)) ? get_FCLK()/6 : get_FCLK()/3;
		default:
			return 0;
	}
}

/* static */ uint32_t
get_PCLK(void)
{
	return (CLKDIVN & 0x1) ? get_HCLK()/2 : get_HCLK();
}

