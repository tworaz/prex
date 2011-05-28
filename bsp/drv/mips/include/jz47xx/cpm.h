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

#ifndef _JZ_CPM_H
#define _JZ_CPM_H

#define	JZ_CPM_BASE   (0x10000000 | MIPS_KSEG1)

/* Clock Reset and Power Controller registers */
#define	JZ_CPM_CLOCK_CTRL        (JZ_CPM_BASE + 0x00)
#define	JZ_CPM_LOW_POWER_CTRL    (JZ_CPM_BASE + 0x04)
#define	JZ_CPM_PLL_CTRL          (JZ_CPM_BASE + 0x10)
#define	JZ_CPM_CLK_GATE          (JZ_CPM_BASE + 0x20)
#define	JZ_CPM_SLEEP_CTRL        (JZ_CPM_BASE + 0x24)
#define	JZ_CPM_I2S_DEV_CLOCK_DIV (JZ_CPM_BASE + 0x60)
#define	JZ_CPM_LCD_PIX_CLK_DIV   (JZ_CPM_BASE + 0x64)
#define	JZ_CPM_MSC_DEV_CLK_DIV   (JZ_CPM_BASE + 0x68)
#define	JZ_CPM_UHC_48M_CLK_DIV   (JZ_CPM_BASE + 0x6C)
#define	JZ_CPM_UHC_PHY_TST       (JZ_CPM_BASE + 0x70)
#define	JZ_CPM_SSI_CLK_DIV       (JZ_CPM_BASE + 0x74)

/* Clock Gate Register Bits */
#define	JZ_CPM_CLK_GATE_UART0	(1 << 0)
#define	JZ_CPM_CLK_GATE_TCU	(1 << 1)
#define	JZ_CPM_CLK_GATE_RTC	(1 << 2)
#define	JZ_CPM_CLK_GATE_I2C	(1 << 3)
#define	JZ_CPM_CLK_GATE_SSI	(1 << 4)
#define	JZ_CPM_CLK_GATE_AIC	(1 << 5)
#define	JZ_CPM_CLK_GATE_AIC2	(1 << 6)
#define	JZ_CPM_CLK_GATE_MSC	(1 << 7)
#define	JZ_CPM_CLK_GATE_SADC	(1 << 8)
#define	JZ_CPM_CLK_GATE_CIM	(1 << 9)
#define	JZ_CPM_CLK_GATE_LCD	(1 << 10)
#define	JZ_CPM_CLK_GATE_UDC	(1 << 11)
#define	JZ_CPM_CLK_GATE_DMAC	(1 << 12)
#define	JZ_CPM_CLK_GATE_IPU	(1 << 13)
#define	JZ_CPM_CLK_GATE_UHC	(1 << 14)
#define	JZ_CPM_CLK_GATE_UART1	(1 << 15)

#endif /* !JZ_CPM_H */
