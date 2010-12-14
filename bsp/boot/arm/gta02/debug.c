/*-
 * Copyright (c) 2010, Peter Tworek <tworaz666@gmail.com>
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

#include <sys/param.h>
#include <boot.h>

/* GTA02 has serial console attached to UART2 */
#define UART2_OFFSET 	0x8000
#define UART_BASE	(0x50000000 + UART2_OFFSET)

#define UART_ULCON	(*(volatile uint32_t *)(UART_BASE + 0x00))
#define UART_UCON	(*(volatile uint32_t *)(UART_BASE + 0x04))
#define UART_UFCON	(*(volatile uint32_t *)(UART_BASE + 0x08))
#define UART_UMCON	(*(volatile uint32_t *)(UART_BASE + 0x0C))
#define UART_UTRSTAT	(*(volatile uint32_t *)(UART_BASE + 0x10))
#define UART_UERSTAT	(*(volatile uint32_t *)(UART_BASE + 0x14))
#define UART_UFSTAT	(*(volatile uint32_t *)(UART_BASE + 0x18))
#define UART_UTXH	(*(volatile uint32_t *)(UART_BASE + 0x20))
#define UART_URXH	(*(volatile uint32_t *)(UART_BASE + 0x24))
#define UART_UBRDIV	(*(volatile uint32_t *)(UART_BASE + 0x28))

/*
 * Print one chracter
 */
void
debug_putc(int c)
{
#if defined(DEBUG) && defined(CONFIG_DIAG_SERIAL)
	if (c == '\n')
		c = '\r';

	/* wait for room in the tx FIFO */
	while (!(UART_UTRSTAT & 0x2));

	UART_UTXH = c;
#endif
}

/*
 * Initialize debug port.
 */
void
debug_init(void)
{
#if defined(DEBUG) && defined(CONFIG_DIAG_SERIAL)
	/* unsigned int reg; */
	int i;

	/* FIFO enable, Tx/Rx FIFO clear */
	UART_UFCON = 0x07;
	UART_UMCON = 0x0;

	/* Normal,No parity,1 stop,8 bit */
	UART_ULCON = 0x3;

	/*
	 * tx=level,rx=edge,disable timeout int.,enable rx error int.,
	 * normal,interrupt or polling
	 */
	UART_UCON = 0x245;

	/*
	 * Set baud rate:
	 * value is calculated so : (int)(PCLK/16./baudrate) -1
	 * PCLK = 50MHz
	 * baudrate = 115200
	 */
	UART_UBRDIV = 0x1A;
	for (i = 0; i < 100; i++);
#endif
}

