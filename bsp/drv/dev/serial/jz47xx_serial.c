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
 * jz47xx.c - Ingenic JZ47XX SOC UART
 */

#include <jz47xx/uart.h>
#include <jz47xx/cpm.h>
#include <driver.h>
#include <tty.h>
#include <serial.h>

#if 0
#define UART_CLK	14745600
#define BAUD_RATE	115200
#endif 

/* Forward functions */
static int	jz47xx_serial_init(struct driver *);
static void	jz47xx_serial_xmt_char(struct serial_port *, char);
static char	jz47xx_serial_rcv_char(struct serial_port *);
static void	jz47xx_serial_set_poll(struct serial_port *, int);
static void	jz47xx_serial_start(struct serial_port *);
static void	jz47xx_serial_stop(struct serial_port *);


struct driver jz47xx_serial_driver = {
	/* name */	"jz47xx_serial",
	/* devops */	NULL,
	/* devsz */	0,
	/* flags */	0,
	/* probe */	NULL,
	/* init */	jz47xx_serial_init,
	/* detach */	NULL,
};

static struct serial_ops jz47xx_serial_ops = {
	/* xmt_char */	jz47xx_serial_xmt_char,
	/* rcv_char */	jz47xx_serial_rcv_char,
	/* set_poll */	jz47xx_serial_set_poll,
	/* start */	jz47xx_serial_start,
	/* stop */	jz47xx_serial_stop,
};

static struct serial_port jz47xx_serial_port;


static void
jz47xx_serial_xmt_char(struct serial_port *sp, char c)
{
	/* wait for room in the tx FIFO */
	while (!((bus_read_8(JZ_UART_LSR) &
	          (JZ_UART_LSR_TDRQ | JZ_UART_LSR_TEMP)) == 0x60))
	    continue;

	bus_write_8(JZ_UART_THR, (uint8_t)c);
}

static char
jz47xx_serial_rcv_char(struct serial_port *sp)
{
#if 0
	char c;

	while (bus_read_32(UART_FR) & FR_RXFE)
		;
	c = bus_read_32(UART_DR) & 0xff;
	return c;
#endif
	return 'a';
}

static void
jz47xx_serial_set_poll(struct serial_port *sp, int on)
{
#if 0
	if (on) {
		/*
		 * Disable interrupt for polling mode.
		 */
		bus_write_32(UART_IMSC, 0);
	} else
		bus_write_32(UART_IMSC, (IMSC_RX | IMSC_TX));
#endif
}

static int
jz47xx_serial_isr(void *arg)
{
#if 0
	struct serial_port *sp = arg;
	int c;
	uint32_t mis;

	mis = bus_read_32(UART_MIS);

	if (mis & MIS_RX) {
		/*
		 * Receive interrupt
		 */
		while (bus_read_32(UART_FR) & FR_RXFE)
			;
		do {
			c = bus_read_32(UART_DR);
			serial_rcv_char(sp, c);
		} while ((bus_read_32(UART_FR) & FR_RXFE) == 0);

		/* Clear interrupt status */
		bus_write_32(UART_ICR, ICR_RX);
	}
	if (mis & MIS_TX) {
		/*
		 * Transmit interrupt
		 */
		serial_xmt_done(sp);

		/* Clear interrupt status */
		bus_write_32(UART_ICR, ICR_TX);
	}
#endif
	return 0;
}

static void
jz47xx_serial_start(struct serial_port *sp)
{
#if 0
	uint32_t divider, remainder, fraction;

	bus_write_32(UART_CR, 0);	/* Disable everything */
	bus_write_32(UART_ICR, 0x07ff);	/* Clear all interrupt status */

	/*
	 * Set baud rate:
	 * IBRD = UART_CLK / (16 * BAUD_RATE)
	 * FBRD = ROUND((64 * MOD(UART_CLK,(16 * BAUD_RATE))) / (16 * BAUD_RATE))
	 */
	divider = UART_CLK / (16 * BAUD_RATE);
	remainder = UART_CLK % (16 * BAUD_RATE);
	fraction = (8 * remainder / BAUD_RATE) >> 1;
	fraction += (8 * remainder / BAUD_RATE) & 1;
	bus_write_32(UART_IBRD, divider);
	bus_write_32(UART_FBRD, fraction);

	/* Set N, 8, 1, FIFO enable */
	bus_write_32(UART_LCRH, (LCRH_WLEN8 | LCRH_FEN));

	/* Enable UART */
	bus_write_32(UART_CR, (CR_RXE | CR_TXE | CR_UARTEN));

	/* Install interrupt handler */
	sp->irq = irq_attach(UART_IRQ, IPL_COMM, 0, pl011_isr, IST_NONE, sp);

	/* Enable TX/RX interrupt */
	bus_write_32(UART_IMSC, (IMSC_RX | IMSC_TX));
#endif
}

static void
jz47xx_serial_stop(struct serial_port *sp)
{
#if 0
	bus_write_32(UART_IMSC, 0);	/* Disable all interrupts */
	bus_write_32(UART_CR, 0);	/* Disable everything */
#endif
}

static int
jz47xx_serial_init(struct driver *self)
{
	serial_attach(&jz47xx_serial_ops, &jz47xx_serial_port);
	return 0;
}
