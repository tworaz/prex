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
#define	CPM_BASE   (0x10000000 | MIPS_KSEG1)
#define	INTC_BASE  (0x10001000 | MIPS_KSEG1)
#define	TCU_BASE   (0x10002000 | MIPS_KSEG1)
#define	UART_BASE  (0x10030000 | MIPS_KSEG1)

/* Clock Reset and Power Controller registers */
#define	JZ_CPM_CPCCR             (*(volatile uint32_t *)(CPM_BASE + 0x00))
#define	JZ_CPM_LOW_POWER_CONTROL (*(volatile uint32_t *)(CPM_BASE + 0x04))
#define	JZ_CPM_CPPCR             (*(volatile uint32_t *)(CPM_BASE + 0x10))
#define	JZ_CPM_CLK_GATE          (*(volatile uint32_t *)(CPM_BASE + 0x20))
#define	JZ_CPM_SLEEP_CONTROL     (*(volatile uint32_t *)(CPM_BASE + 0x24))
#define	JZ_CPM_I2SCDR            (*(volatile uint32_t *)(CPM_BASE + 0x60))
#define	JZ_CPM_LPCDR             (*(volatile uint32_t *)(CPM_BASE + 0x64))
#define	JZ_CPM_MSCCDR            (*(volatile uint32_t *)(CPM_BASE + 0x68))
#define	JZ_CPM_UHCCDR            (*(volatile uint32_t *)(CPM_BASE + 0x6C))
#define	JZ_CPM_UHCTST            (*(volatile uint32_t *)(CPM_BASE + 0x70))
#define	JZ_CPM_SSICDR            (*(volatile uint32_t *)(CPM_BASE + 0x74))

/* Clock Gate Register Bits */
#define	JZ_CPM_CLK_GATE_UART0	0
#define	JZ_CPM_CLK_GATE_TCU	1
#define	JZ_CPM_CLK_GATE_RTC	2
#define	JZ_CPM_CLK_GATE_I2C	3
#define	JZ_CPM_CLK_GATE_SSI	4
#define	JZ_CPM_CLK_GATE_AIC	5
#define	JZ_CPM_CLK_GATE_AIC2	6
#define	JZ_CPM_CLK_GATE_MSC	7
#define	JZ_CPM_CLK_GATE_SADC	8
#define	JZ_CPM_CLK_GATE_CIM	9
#define	JZ_CPM_CLK_GATE_LCD	10
#define	JZ_CPM_CLK_GATE_UDC	11
#define	JZ_CPM_CLK_GATE_DMAC	12
#define	JZ_CPM_CLK_GATE_IPU	13
#define	JZ_CPM_CLK_GATE_UHC	14
#define	JZ_CPM_CLK_GATE_UART1	15

/* Interrupt controller registers */
#define	JZ_INTC_SOURCE     (*(volatile uint32_t *)(INTC_BASE + 0x00))
#define	JZ_INTC_MASK       (*(volatile uint32_t *)(INTC_BASE + 0x04))
#define	JZ_INTC_MASK_SET   (*(volatile uint32_t *)(INTC_BASE + 0x08))
#define	JZ_INTC_MASK_CLEAR (*(volatile uint32_t *)(INTC_BASE + 0x0C))
#define	JZ_INTC_PENDING    (*(volatile uint32_t *)(INTC_BASE + 0x10))

/* Interrupt controller bits */
#define	JZ_IRQ_I2C     1
#define	JZ_IRQ_EMC     2
#define	JZ_IRQ_UHC     3
#define	JZ_IRQ_UART0   9
#define	JZ_IRQ_SADC    12
#define	JZ_IRQ_MSC     14
#define	JZ_IRQ_RTC     15
#define	JZ_IRQ_SSI     16
#define	JZ_IRQ_CIM     17
#define	JZ_IRQ_AIC     18
#define	JZ_IRQ_DMA     20
#define	JZ_IRQ_TCU2    21
#define	JZ_IRQ_TCU1    22
#define	JZ_IRQ_TCU0    23
#define	JZ_IRQ_UDC     24
#define	JZ_IRQ_GPIO3   25
#define	JZ_IRQ_GPIO2   26
#define	JZ_IRQ_GPIO1   27
#define	JZ_IRQ_GPIO0   28
#define	JZ_IRQ_IPU     29
#define	JZ_IRQ_LCD     30

/* Timer control unit registers */
#define	JZ_TCU_STOP          (*(volatile uint8_t  *)(TCU_BASE + 0x1C))
#define	JZ_TCU_STOP_SET      (*(volatile uint8_t  *)(TCU_BASE + 0x2C))
#define	JZ_TCU_STOP_CLEAR    (*(volatile uint8_t  *)(TCU_BASE + 0x3C))
#define	JZ_TCU_ENABLE        (*(volatile uint8_t  *)(TCU_BASE + 0x10))
#define	JZ_TCU_ENABLE_SET    (*(volatile uint8_t  *)(TCU_BASE + 0x14))
#define	JZ_TCU_ENABLE_CLEAR  (*(volatile uint8_t  *)(TCU_BASE + 0x18))
#define	JZ_TCU_FLAG          (*(volatile uint32_t *)(TCU_BASE + 0x20))
#define	JZ_TCU_FLAG_SET      (*(volatile uint32_t *)(TCU_BASE + 0x24))
#define	JZ_TCU_FLAG_CLEAR    (*(volatile uint32_t *)(TCU_BASE + 0x28))
#define	JZ_TCU_MASK          (*(volatile uint32_t *)(TCU_BASE + 0x30))
#define	JZ_TCU_MASK_SET      (*(volatile uint32_t *)(TCU_BASE + 0x34))
#define	JZ_TCU_MASK_CLEAR    (*(volatile uint32_t *)(TCU_BASE + 0x38))
#define JZ_TCU_DATA_FULL(x)  (*(volatile uint16_t *)(TCU_BASE + (x * 0x10) + 0x40))
#define JZ_TCU_DATA_HALF(x)  (*(volatile uint16_t *)(TCU_BASE + (x * 0x10) + 0x44))
#define JZ_TCU_COUNT(x)      (*(volatile uint16_t *)(TCU_BASE + (x * 0x10) + 0x48))
#define JZ_TCU_CONTROL(x)    (*(volatile uint16_t *)(TCU_BASE + (x * 0x10) + 0x4C))

/* TCSR register bits */
#define	JZ_TCU_CONTROL_PCK_EN  (1 << 0)
#define	JZ_TCU_CONTROL_RTC_EN  (1 << 1)
#define	JZ_TCU_CONTROL_EXT_EN  (1 << 2)
#define	JZ_TCU_CONTROL_PWM_EN  (1 << 7)
#define	JZ_TCU_CONTROL_INITL   (1 << 8)
#define	JZ_TCU_CONTROL_SD      (1 << 9)

#define	JZ_TCU_CONTROL_PRESCALE_MASK 0x0038
#define	JZ_TCU_CONTROL_PRESCALE_BIT  3

/* UART Registers */
#define	JZ_UART_THR	(*(volatile uint8_t *)(UART_BASE + 0x00))
#define	JZ_UART_IER	(*(volatile uint8_t *)(UART_BASE + 0x04))
#define	JZ_UART_FCR	(*(volatile uint8_t *)(UART_BASE + 0x08))
#define	JZ_UART_LSR 	(*(volatile uint8_t *)(UART_BASE + 0x14))

/* LSR Register Bits */
#define	JZ_UART_LSR_DRY		(1 << 0)
#define	JZ_UART_LSR_OVER	(1 << 1)
#define	JZ_UART_LSR_PARER	(1 << 2)
#define	JZ_UART_LSR_FMER	(1 << 3)
#define	JZ_UART_LSR_BI		(1 << 4)
#define	JZ_UART_LSR_TDRQ	(1 << 5)
#define	JZ_UART_LSR_TEMP	(1 << 6)
#define	JZ_UART_LSR_FIFOE	(1 << 7)

#endif /* !_JZ47XX_PLATFORM_H */
