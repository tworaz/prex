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

#ifndef _NANONOTE_PLATFORM_H
#define _NANONOTE_PLATFORM_H

#include <mips/cpuregs.h>

#define UART_BASE 	(0x10030000 | MIPS_KSEG1_START)

/* UART Registers */
#define UART_THR	(*(volatile uint8_t *)(UART_BASE + 0x00))
#define UART_IER	(*(volatile uint8_t *)(UART_BASE + 0x04))
#define UART_FCR	(*(volatile uint8_t *)(UART_BASE + 0x08))
#define UART_LSR 	(*(volatile uint8_t *)(UART_BASE + 0x14))

/* LSR Register Bits */
#define UART_LSR_DRY	(1 << 0)
#define UART_LSR_OVER	(1 << 1)
#define UART_LSR_PARER	(1 << 2)
#define UART_LSR_FMER	(1 << 3)
#define UART_LSR_BI	(1 << 4)
#define UART_LSR_TDRQ	(1 << 5)
#define UART_LSR_TEMP	(1 << 6)
#define UART_LSR_FIFOE	(1 << 7)

#endif /* !_NANONOTE_PLATFORM_H */
