/*-
 * Copyright (c) 2009, Sean Chiang<n2i911@gmail.com>
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
 * diag.c - diagnostic message support
 */

#include <sys/bootinfo.h>
#include <kernel.h>
#include <cpufunc.h>

#include "platform.h"

/*
 * UART Registers, For gta02
 * UART0 - gsm modem
 * UART1 - gps
 * UART2 - serial console
 */
#define UART_UTRSTAT    (*(volatile uint32_t *)(UART_BASE + 0x10))
#define UART_UTXH       (*(volatile uint32_t *)(UART_BASE + 0x20))

static void
serial_putc(char c)
{
	/* wait for room in the tx FIFO */
	while (!(UART_UTRSTAT & 0x2));

	UART_UTXH = c;
}

void
diag_puts(char *buf)
{
	while (*buf) {
		if (*buf == '\n')
			serial_putc('\r');
		serial_putc(*buf++);
	}
}

void
diag_init(void)
{
#ifdef CONFIG_MMU
	mmu_premap(0x50008000, UART_BASE);
#endif
}
