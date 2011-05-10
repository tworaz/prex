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

#ifndef _JZ47XX_PLATFORM_H
#define _JZ47XX_PLATFORM_H

#include <mips/cpuregs.h>

/* Base adresses of jz47xx peripherials */
#define	CPM_BASE	(0x10000000 | MIPS_KSEG1)
#define	INTC_BASE	(0x10001000 | MIPS_KSEG1)
#define	TCU_BASE	(0x10002000 | MIPS_KSEG1)
#define	UART_BASE 	(0x10030000 | MIPS_KSEG1)

/* Clock Reset and Power Controller registers */
#define	CPM_CPCCR	(*(volatile uint32_t *)(CPM_BASE + 0x00))
#define	CPM_LCR		(*(volatile uint32_t *)(CPM_BASE + 0x04))
#define	CPM_CPPCR	(*(volatile uint32_t *)(CPM_BASE + 0x10))
#define	CPM_CLKGR	(*(volatile uint32_t *)(CPM_BASE + 0x20))
#define	CPM_SCR		(*(volatile uint32_t *)(CPM_BASE + 0x24))
#define	CPM_I2SCDR	(*(volatile uint32_t *)(CPM_BASE + 0x60))
#define	CPM_LPCDR	(*(volatile uint32_t *)(CPM_BASE + 0x64))
#define	CPM_MSCCDR	(*(volatile uint32_t *)(CPM_BASE + 0x68))
#define	CPM_UHCCDR	(*(volatile uint32_t *)(CPM_BASE + 0x6C))
#define	CPM_UHCTST	(*(volatile uint32_t *)(CPM_BASE + 0x70))
#define	CPM_SSICDR	(*(volatile uint32_t *)(CPM_BASE + 0x74))

/* Clock Gate Register Bits */
#define	CPM_CLKGR_UART0	0
#define	CPM_CLKGR_TCU	1
#define	CPM_CLKGR_RTC	2
#define	CPM_CLKGR_I2C	3
#define	CPM_CLKGR_SSI	4
#define	CPM_CLKGR_AIC	5
#define	CPM_CLKGR_AIC2	6
#define	CPM_CLKGR_MSC	7
#define	CPM_CLKGR_SADC	8
#define	CPM_CLKGR_CIM	9
#define	CPM_CLKGR_LCD	10
#define	CPM_CLKGR_UDC	11
#define	CPM_CLKGR_DMAC	12
#define	CPM_CLKGR_IPU	13
#define	CPM_CLKGR_UHC	14
#define	CPM_CLKGR_UART1	15

/* Interrupt controller registers */
#define	INTC_ICSR	(*(volatile uint32_t *)(INTC_BASE + 0x00))
#define	INTC_ICMR	(*(volatile uint32_t *)(INTC_BASE + 0x04))
#define	INTC_ICMSR	(*(volatile uint32_t *)(INTC_BASE + 0x08))
#define	INTC_ICMCR	(*(volatile uint32_t *)(INTC_BASE + 0x0C))
#define	INTC_ICPR	(*(volatile uint32_t *)(INTC_BASE + 0x10))

/* Interrupt controller bits */
#define	IRQ_I2C		1
#define	IRQ_EMC		2
#define	IRQ_UHC		3
#define	IRQ_UART0	9
#define	IRQ_SADC	12
#define	IRQ_MSC		14
#define	IRQ_RTC		15
#define	IRQ_SSI		16
#define	IRQ_CIM		17
#define	IRQ_AIC		18
#define	IRQ_DMA		20
#define	IRQ_TCU2	21
#define	IRQ_TCU1	22
#define	IRQ_TCU0	23
#define	IRQ_UDC		24
#define	IRQ_GPIO3	25
#define	IRQ_GPIO2	26
#define	IRQ_GPIO1	27
#define	IRQ_GPIO0	28
#define	IRQ_IPU		29
#define	IRQ_LCD		30

/* Timer control unit registers */
#define	TCU_TSR		(*(volatile uint8_t  *)(TCU_BASE + 0x1C))
#define	TCU_TSSR	(*(volatile uint8_t  *)(TCU_BASE + 0x2C))
#define	TCU_TSCR	(*(volatile uint8_t  *)(TCU_BASE + 0x3C))
#define	TCU_TER		(*(volatile uint8_t  *)(TCU_BASE + 0x10))
#define	TCU_TESR	(*(volatile uint8_t  *)(TCU_BASE + 0x14))
#define	TCU_TECR	(*(volatile uint8_t  *)(TCU_BASE + 0x18))
#define	TCU_TFR		(*(volatile uint32_t *)(TCU_BASE + 0x20))
#define	TCU_TFSR	(*(volatile uint32_t *)(TCU_BASE + 0x24))
#define	TCU_TFCR	(*(volatile uint32_t *)(TCU_BASE + 0x28))
#define	TCU_TMR		(*(volatile uint32_t *)(TCU_BASE + 0x30))
#define	TCU_TMSR	(*(volatile uint32_t *)(TCU_BASE + 0x34))
#define	TCU_TMCR	(*(volatile uint32_t *)(TCU_BASE + 0x38))
#define	TCU_TDFR0	(*(volatile uint16_t *)(TCU_BASE + 0x40))
#define	TCU_TDHR0	(*(volatile uint16_t *)(TCU_BASE + 0x44))
#define	TCU_TCNT0	(*(volatile uint16_t *)(TCU_BASE + 0x48))
#define	TCU_TCSR0	(*(volatile uint16_t *)(TCU_BASE + 0x4C))
#define	TCU_TDFR1	(*(volatile uint16_t *)(TCU_BASE + 0x50))
#define	TCU_TDHR1	(*(volatile uint16_t *)(TCU_BASE + 0x54))
#define	TCU_TCNT1	(*(volatile uint16_t *)(TCU_BASE + 0x58))
#define	TCU_TCSR1	(*(volatile uint16_t *)(TCU_BASE + 0x5C))
#define	TCU_TDFR2	(*(volatile uint16_t *)(TCU_BASE + 0x60))
#define	TCU_TDHR2	(*(volatile uint16_t *)(TCU_BASE + 0x64))
#define	TCU_TCNT2	(*(volatile uint16_t *)(TCU_BASE + 0x68))
#define	TCU_TCSR2	(*(volatile uint16_t *)(TCU_BASE + 0x6C))
#define	TCU_TDFR3	(*(volatile uint16_t *)(TCU_BASE + 0x70))
#define	TCU_TDHR3	(*(volatile uint16_t *)(TCU_BASE + 0x74))
#define	TCU_TCNT3	(*(volatile uint16_t *)(TCU_BASE + 0x78))
#define	TCU_TCSR3	(*(volatile uint16_t *)(TCU_BASE + 0x7C))
#define	TCU_TDFR4	(*(volatile uint16_t *)(TCU_BASE + 0x80))
#define	TCU_TDHR4	(*(volatile uint16_t *)(TCU_BASE + 0x84))
#define	TCU_TCNT4	(*(volatile uint16_t *)(TCU_BASE + 0x88))
#define	TCU_TCSR4	(*(volatile uint16_t *)(TCU_BASE + 0x8C))
#define	TCU_TDFR5	(*(volatile uint16_t *)(TCU_BASE + 0x90))
#define	TCU_TDHR5	(*(volatile uint16_t *)(TCU_BASE + 0x94))
#define	TCU_TCNT5	(*(volatile uint16_t *)(TCU_BASE + 0x98))
#define	TCU_TCSR5	(*(volatile uint16_t *)(TCU_BASE + 0x9C))
#define	TCU_TDFR6	(*(volatile uint16_t *)(TCU_BASE + 0xA0))
#define	TCU_TDHR6	(*(volatile uint16_t *)(TCU_BASE + 0xA4))
#define	TCU_TCNT6	(*(volatile uint16_t *)(TCU_BASE + 0xA8))
#define	TCU_TCSR6	(*(volatile uint16_t *)(TCU_BASE + 0xAC))
#define	TCU_TDFR7	(*(volatile uint16_t *)(TCU_BASE + 0xB0))
#define	TCU_TDHR7	(*(volatile uint16_t *)(TCU_BASE + 0xB4))
#define	TCU_TCNT7	(*(volatile uint16_t *)(TCU_BASE + 0xB8))
#define	TCU_TCSR7	(*(volatile uint16_t *)(TCU_BASE + 0xBC))

/* TCSR register bits */
#define	TCU_TCSR_PCK_EN		(1 << 0)
#define	TCU_TCSR_RTC_EN		(1 << 1)
#define	TCU_TCSR_EXT_EN 	(1 << 2)
#define	TCU_TCSR_PWM_EN		(1 << 7)
#define	TCU_TCSR_INITL		(1 << 8)
#define	TCU_TCSR_SD		(1 << 9)

#define	TCU_TCSR_PRESCALE_MASK	0x0038
#define	TCU_TCSR_PRESCALE_BIT	3

/* UART Registers */
#define	UART_THR	(*(volatile uint8_t *)(UART_BASE + 0x00))
#define	UART_IER	(*(volatile uint8_t *)(UART_BASE + 0x04))
#define	UART_FCR	(*(volatile uint8_t *)(UART_BASE + 0x08))
#define	UART_LSR 	(*(volatile uint8_t *)(UART_BASE + 0x14))

/* LSR Register Bits */
#define	UART_LSR_DRY	(1 << 0)
#define	UART_LSR_OVER	(1 << 1)
#define	UART_LSR_PARER	(1 << 2)
#define	UART_LSR_FMER	(1 << 3)
#define	UART_LSR_BI	(1 << 4)
#define	UART_LSR_TDRQ	(1 << 5)
#define	UART_LSR_TEMP	(1 << 6)
#define	UART_LSR_FIFOE	(1 << 7)

#endif /* !_JZ47XX_PLATFORM_H */
