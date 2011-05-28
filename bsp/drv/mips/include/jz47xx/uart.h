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
 * Register definitions for Ingenic JZ47XX SOC UART
 */

#ifndef _JZ47XX_UART_H
#define _JZ47XX_UART_H

#include <mips/cpuregs.h>

#define JZ_UART_BASE	(0x10030000 | MIPS_KSEG1)
#define JZ_UART_IRQ	9
#define JZ_UART_CLK	3686400

/*
 * UART registers
 */
#define JZ_UART_RX(n)                   (JZ_UART_BASE + ((n) * 0x1000) + 0x00)
#define JZ_UART_TX(n)                   (JZ_UART_BASE + ((n) * 0x1000) + 0x00)
#define JZ_UART_LATCH_LO(n)             (JZ_UART_BASE + ((n) * 0x1000) + 0x00)
#define JZ_UART_LATCH_HI(n)             (JZ_UART_BASE + ((n) * 0x1000) + 0x04)
#define JZ_UART_IRQ_EN(n)               (JZ_UART_BASE + ((n) * 0x1000) + 0x04)
#define JZ_UART_IRQ_IDENT(n)            (JZ_UART_BASE + ((n) * 0x1000) + 0x08)
#define JZ_UART_FIFO_CTRL(n)            (JZ_UART_BASE + ((n) * 0x1000) + 0x08)
#define JZ_UART_LINE_CTRL(n)            (JZ_UART_BASE + ((n) * 0x1000) + 0x0C)
#define JZ_UART_MODEM_CTRL(n)           (JZ_UART_BASE + ((n) * 0x1000) + 0x10)
#define JZ_UART_LINE_STATUS(n)          (JZ_UART_BASE + ((n) * 0x1000) + 0x14)
#define JZ_UART_MODEM_STATUS(n)         (JZ_UART_BASE + ((n) * 0x1000) + 0x18)
#define JZ_UART_INFRARED_SEL(n)         (JZ_UART_BASE + ((n) * 0x1000) + 0x20)

/*
 * Interrupt enable register bits
 */
#define JZ_UART_IRQ_EN_RDRIE            (1 << 0)
#define JZ_UART_IRQ_EN_TDRIE            (1 << 1)
#define JZ_UART_IRQ_EN_RLSIE            (1 << 2)
#define JZ_UART_IRQ_EN_MSIE             (1 << 3)
#define JZ_UART_IRQ_EN_RTOIE            (1 << 4)

/*
 * Interrupt identification register
 */
#define JZ_UART_IRQ_IDENT_PEND          (1 << 0)
#define JZ_UART_IRQ_IDENT_ID_MASK       0x0E
#define JZ_UART_IRQ_IDENT_ID_SHIFT      1
#define JZ_UART_IRQ_IDENT_FFMSEL_MASK   0xC0
#define JZ_UART_IRQ_IDENT_FFMSEL_SHIFT  6
#define JZ_UART_IRQ_IDENT_ID_RX         0x04
#define JZ_UART_IRQ_IDENT_ID_TX         0x02

/*
 * FIFO Control register bits
 */
#define JZ_UART_FIFO_CTRL_FME           (1 << 0)
#define JZ_UART_FIFO_CTRL_RFRT          (1 << 1)
#define JZ_UART_FIFO_CTRL_TFRT          (1 << 2)
#define JZ_UART_FIFO_CTRL_DME           (1 << 3)
#define JZ_UART_FIFO_CTRL_UME           (1 << 4)
#define JZ_UART_FIFO_CTRL_RDTR_MASK     0xC0
#define JZ_UART_FIFO_CTRL_RDTR_SHIFT    6

/*
 * Line control register bits
 */
#define JZ_UART_LINE_CTRL_WLS_MASK      0x03
#define JZ_UART_LINE_CTRL_SBLS          (1 << 2)
#define JZ_UART_LINE_CTRL_PARE          (1 << 3)
#define JZ_UART_LINE_CTRL_PARM          (1 << 4)
#define JZ_UART_LINE_CTRL_STPAR         (1 << 5)
#define JZ_UART_LINE_CTRL_SBK           (1 << 6)
#define JZ_UART_LINE_CTRL_DLAB          (1 << 7)

/*
 * Line status register bits
 */
#define	JZ_UART_LINE_STATUS_DRY		(1 << 0)
#define	JZ_UART_LINE_STATUS_OVER	(1 << 1)
#define	JZ_UART_LINE_STATUS_PARER	(1 << 2)
#define	JZ_UART_LINE_STATUS_FMER	(1 << 3)
#define	JZ_UART_LINE_STATUS_BI		(1 << 4)
#define	JZ_UART_LINE_STATUS_TDRQ	(1 << 5)
#define	JZ_UART_LINE_STATUS_TEMP	(1 << 6)
#define	JZ_UART_LINE_STATUS_FIFOE	(1 << 7)

/*
 * Modem control register bits
 */
#define JZ_UART_MODEM_CTRL_RTS          (1 << 1)
#define JZ_UART_MODEM_CTRL_LOOP         (1 << 4)
#define JZ_UART_MODEM_CTRL_MDCE         (1 << 7)

/*
 * Modem status register bits
 */
#define JZ_UART_MODEM_STATUS_CCTS       (1 << 0)
#define JZ_UART_MODEM_STATUS_CTS        (1 << 4)

/*
 * Infrared selection register bits
 */
#define JZ_UART_INFRARED_SEL_XMITIR     (1 << 0)
#define JZ_UART_INFRARED_SEL_RCVEIR     (1 << 1)
#define JZ_UART_INFRARED_SEL_XMODE      (1 << 2)
#define JZ_UART_INFRARED_SEL_TDPL       (1 << 3)
#define JZ_UART_INFRARED_SEL_RDPL       (1 << 4)

#endif /* !_JZ47XX_UART_H */
