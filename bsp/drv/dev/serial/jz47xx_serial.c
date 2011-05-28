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

/* JZ47XX UART port to use for serial interface */
#ifdef CONFIG_JZ47XX_SERIAL_UART_PORT
#define JZ_UART_PORT CONFIG_JZ47XX_SERIAL_UART_PORT
#else
#define JZ_UART_PORT 0
#endif /* CONFIG_JZ47XX_SERIAL_UART_PORT */

/* Baud rate for serial port */
#ifdef CONFIG_SERIAL_BAUD
#define BAUD_RATE CONFIG_SERIAL_BAUD
#else
#define BAUD_RATE 115200
#endif /* CONFIG_SERIAL_BAUD */


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
	while (! ((bus_read_8( JZ_UART_LINE_STATUS(JZ_UART_PORT) ) &
	           (JZ_UART_LINE_STATUS_TDRQ | JZ_UART_LINE_STATUS_TEMP)) == 0x60) )
	    continue;

	bus_write_8(JZ_UART_TX(JZ_UART_PORT), (uint8_t)c);
}

static char
jz47xx_serial_rcv_char(struct serial_port *sp)
{
	char c;

	while (!(bus_read_8(JZ_UART_LINE_STATUS(JZ_UART_PORT)) &
	         JZ_UART_LINE_STATUS_DRY))
	    continue;

	c = bus_read_8(JZ_UART_RX(JZ_UART_PORT));
	return c;
}

static void
jz47xx_serial_set_poll(struct serial_port *sp, int on)
{
	if (on) {
		/* Disable interrupt for polling mode */
		bus_write_8(JZ_UART_IRQ_EN(JZ_UART_PORT), 0x00);
	} else {
		/* Enable tx/rx interrupt */
		bus_write_8(JZ_UART_IRQ_EN(JZ_UART_PORT),
		            (JZ_UART_IRQ_EN_RDRIE | JZ_UART_IRQ_EN_TDRIE));
	}
}

static int
jz47xx_serial_isr(void *arg)
{
	struct serial_port *sp = arg;
	int c;
	uint8_t id, tmp;

	id = bus_read_8(JZ_UART_IRQ_IDENT(JZ_UART_PORT));

	/* Spurious interrupt? */
	if (id & JZ_UART_IRQ_IDENT_PEND) {
		printf("Spurious UART interrupt\n");
		return -1;
	}

	id &= JZ_UART_IRQ_IDENT_ID_MASK;
	switch (id) {
	case JZ_UART_IRQ_IDENT_ID_RX:
		/*
		 * Receive interrupt
		 */
		tmp = bus_read_8(JZ_UART_LINE_STATUS(JZ_UART_PORT));
		while (tmp & JZ_UART_LINE_STATUS_DRY) {
			c = bus_read_8(JZ_UART_RX(JZ_UART_PORT));
			serial_rcv_char(sp, c);
			tmp = bus_read_8(JZ_UART_LINE_STATUS(JZ_UART_PORT));
		}
		break;
	case JZ_UART_IRQ_IDENT_ID_TX:
		/*
		 * Transmit interrupt
		 */
		serial_xmt_done(sp);
		break;
	default:
		/*
		sys_log("Unhandled interrupt type 0x%x\n", id);
		*/
		return -1;
	}

	return 0;
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
}

static void
jz47xx_serial_start(struct serial_port *sp)
{
	uint8_t  lcr;
	uint16_t div;
	uint32_t clkgr;

	/* Enable UART module clock */
	clkgr = bus_read_32(JZ_CPM_CLK_GATE);
	clkgr &= ~(JZ_CPM_CLK_GATE_UART0);
	bus_write_32(JZ_CPM_CLK_GATE, clkgr);

	/* Disable UART module */
	bus_write_8(JZ_UART_FIFO_CTRL(JZ_UART_PORT), 0x00);

	/* Enable access to latch regs, set char 8bit*/
	lcr = JZ_UART_LINE_CTRL_DLAB | 0x03;
	bus_write_8(JZ_UART_LINE_CTRL(JZ_UART_PORT), lcr);

	/* Set baud rate */
	div = JZ_UART_CLK/(16 * BAUD_RATE);
	bus_write_8(JZ_UART_LATCH_LO(JZ_UART_PORT), (uint8_t)div);
	bus_write_8(JZ_UART_LATCH_HI(JZ_UART_PORT), (uint8_t)(div >> 8));

	/* Disable access to latch regs */
	lcr &= ~(JZ_UART_LINE_CTRL_DLAB);
	bus_write_8(JZ_UART_LINE_CTRL(JZ_UART_PORT), lcr);

	/* Enable FIFO and reset it */
	bus_write_8(JZ_UART_FIFO_CTRL(JZ_UART_PORT), JZ_UART_FIFO_CTRL_FME);
	bus_write_8(JZ_UART_FIFO_CTRL(JZ_UART_PORT), JZ_UART_FIFO_CTRL_RFRT);
	bus_write_8(JZ_UART_FIFO_CTRL(JZ_UART_PORT), JZ_UART_FIFO_CTRL_TFRT);
	bus_write_8(JZ_UART_FIFO_CTRL(JZ_UART_PORT),
	            (0x00 << JZ_UART_FIFO_CTRL_RDTR_SHIFT));

	/* Register interrupt handler */
	sp->irq = irq_attach(JZ_UART_IRQ, IPL_COMM, 0,
	                     jz47xx_serial_isr, IST_NONE, sp);

	/* Enable tx/rx interrupt */
	bus_write_8(JZ_UART_IRQ_EN(JZ_UART_PORT),
	            (JZ_UART_IRQ_EN_RDRIE | JZ_UART_IRQ_EN_TDRIE));

	/* Enable UART */
	bus_write_8(JZ_UART_FIFO_CTRL(JZ_UART_PORT), JZ_UART_FIFO_CTRL_UME);
}

static void
jz47xx_serial_stop(struct serial_port *sp)
{
	uint32_t clkgr;

	/* Disable UART port */
	bus_write_8(JZ_UART_FIFO_CTRL(JZ_UART_PORT), 0x00);

	/* Disable clock for UART module */
	clkgr = bus_read_32(JZ_CPM_CLK_GATE);
	clkgr |= JZ_CPM_CLK_GATE_UART0;
	bus_write_32(JZ_CPM_CLK_GATE, clkgr);
}

static int
jz47xx_serial_init(struct driver *self)
{
	serial_attach(&jz47xx_serial_ops, &jz47xx_serial_port);
	return 0;
}
