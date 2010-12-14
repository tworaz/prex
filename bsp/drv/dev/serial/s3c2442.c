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
 * s3c2442.c - Samsung S3C2442 serial driver
 */

#include <driver.h>
#include <tty.h>
#include <serial.h>

/* #define DEBUG_S3C2442_SERIAL 1 */

#ifdef DEBUG_S3C2442_SERIAL
#define DPRINTF(a) printf a
#else
#define DPRINTF(a)
#endif

#ifdef CONFIG_MMU
#define UART_BASE	(0xc0000000 + 0x50008000)
#define ICU_BASE	(0xc0000000 + 0x4A000000)
#else
#define UART_BASE	(0x50008000)
#define ICU_BASE	(0x4A000000)
#endif

#define UART_IRQ	15
#define TX_SUB_IRQ	7
#define RX_SUB_IRQ	6
#define BAUD_RATE	115200

/* UART Registers */
#define UART_ULCON	(UART_BASE + 0x00)
#define UART_UCON	(UART_BASE + 0x04)
#define UART_UFCON	(UART_BASE + 0x08)
#define UART_UMCON	(UART_BASE + 0x0C)
#define UART_UTRSTAT	(UART_BASE + 0x10)
#define UART_UERSTAT	(UART_BASE + 0x14)
#define UART_UFSTAT	(UART_BASE + 0x18)
#define UART_UTXH	(UART_BASE + 0x20)
#define UART_URXH	(UART_BASE + 0x24)
#define UART_UBRDIV	(UART_BASE + 0x28)

/* Registers for interrupt control unit */
#define ICU_SRCPND      (ICU_BASE + 0x00)
#define ICU_INTMOD      (ICU_BASE + 0x04)
#define ICU_INTMSK      (ICU_BASE + 0x08)
#define ICU_PRIORITY    (ICU_BASE + 0x0C)
#define ICU_INTPND      (ICU_BASE + 0x10)
#define ICU_INTOFFSET   (ICU_BASE + 0x14)
#define ICU_SUBSRCPND   (ICU_BASE + 0x18)
#define ICU_SUBINTMSK   (ICU_BASE + 0x1C)

/* Forward functions */
static int	s3c2442_serial_init(struct driver *);
static void	s3c2442_serial_xmt_char(struct serial_port *, char);
static char	s3c2442_serial_rcv_char(struct serial_port *);
static void	s3c2442_serial_set_poll(struct serial_port *, int);
static void	s3c2442_serial_start(struct serial_port *);
static void	s3c2442_serial_stop(struct serial_port *);


struct driver s3c2442_serial_driver = {
	/* name */	"s3c2442_serial",
	/* devops */	NULL,
	/* devsz */	0,
	/* flags */	0,
	/* probe */	NULL,
	/* init */	s3c2442_serial_init,
	/* detach */	NULL,
};

static struct serial_ops s3c2442_serial_ops = {
	/* xmt_char */	s3c2442_serial_xmt_char,
	/* rcv_char */	s3c2442_serial_rcv_char,
	/* set_poll */	s3c2442_serial_set_poll,
	/* start */	s3c2442_serial_start,
	/* stop */	s3c2442_serial_stop,
};

static struct serial_port s3c2442_serial_port;


static void
s3c2442_serial_xmt_char(struct serial_port *sp, char c)
{
	while (bus_read_32(UART_UFSTAT) & (1<<14))
		;
	bus_write_8(UART_UTXH, c);
}

static char
s3c2442_serial_rcv_char(struct serial_port *sp)
{
	char c;

	while ((bus_read_32(UART_UFSTAT) & 0x1f) == 0)
		;
	c = bus_read_32(UART_URXH) & 0xff;
	return c;
}

static int
s3c2442_serial_isr(void *arg)
{
	struct serial_port *sp = arg;
	int c;

	if (bus_read_32(ICU_SUBSRCPND) & (1<<RX_SUB_IRQ)) {
		/*
		 * Receive interrupt
		 */
		do {
			c = bus_read_32(UART_URXH) & 0xff;
			serial_rcv_char(sp, c);
		} while((bus_read_32(UART_UFSTAT) & 0x1f) != 0);

		/* Clear interrupt status */
		bus_write_32(ICU_SUBSRCPND, (1<<RX_SUB_IRQ));
	}
	else if (bus_read_32(ICU_SUBSRCPND) & (1<<TX_SUB_IRQ)) {
		/*
		 * Transmit interrupt
		 */
		serial_xmt_done(sp);

		/* Clear interrupt status */
		bus_write_32(ICU_SUBSRCPND, (1<<TX_SUB_IRQ));
	}
	return 0;
}

static void
s3c2442_serial_set_poll(struct serial_port *sp, int on)
{
	uint32_t msk;
	msk = bus_read_32(ICU_SUBINTMSK);
	if (on) {
		/* Disable TX/RX interrupt */
		msk |= ((1<<TX_SUB_IRQ)|(1<<RX_SUB_IRQ));
		bus_write_32(ICU_SUBINTMSK, msk);
	} else {
		/* Enable TX/RX interrupt */
		msk &= ~(/* (1<<TX_SUB_IRQ)|*/ (1<<RX_SUB_IRQ));
		bus_write_32(ICU_SUBINTMSK, msk);
	}
}

static void
s3c2442_serial_start(struct serial_port *sp)
{
	uint32_t tmp;

        /* FIFO enable */
	bus_write_32(UART_UFCON, 0x07);
	bus_write_32(UART_UMCON, 0x00);

	/* Normal,No parity,1 stop,8 bit */
	bus_write_32(UART_ULCON, 0x03);

	/*
	 * tx=level,rx=level,disable timeout int.,disable rx error int.,
	 * normal,interrupt or polling
	 */
	bus_write_32(UART_UCON, (1<<9)|(1<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(1<<2)|(1<<0));

	/* Install interrupt handler */
	sp->irq = irq_attach(UART_IRQ, IPL_COMM, 0, s3c2442_serial_isr, IST_NONE, sp);

	/* Clear sub pending */
	bus_write_32(ICU_SUBSRCPND, ((1<<TX_SUB_IRQ)|(1<<RX_SUB_IRQ)));

	/* Enable TX/RX interrupt */
	tmp = bus_read_32(ICU_SUBINTMSK);
	tmp &= ~((1<<TX_SUB_IRQ)|(1<<RX_SUB_IRQ));
	bus_write_32(ICU_SUBINTMSK, tmp);

	/*
	 * TODO: baud rate
	 */
}

static void
s3c2442_serial_stop(struct serial_port *sp)
{
	uint32_t tmp;

	/* Clear sub pending */
	bus_write_32(ICU_SUBSRCPND, ((1<<TX_SUB_IRQ)|(1<<RX_SUB_IRQ)));

	/* Disable TX/RX interrupt */
	tmp = bus_read_32(ICU_SUBINTMSK);
	tmp |= ((1<<TX_SUB_IRQ)|(1<<RX_SUB_IRQ));
	bus_write_32(ICU_SUBINTMSK, tmp);

	/* Detach interrupt handler */
	irq_detach(sp->irq);
}

static int
s3c2442_serial_init(struct driver *self)
{
	serial_attach(&s3c2442_serial_ops, &s3c2442_serial_port);
	return 0;
}

