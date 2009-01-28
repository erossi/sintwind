/* This file is part of OpenSint
 * Copyright (C) 2005-2009 Enrico Rossi
 * 
 * OpenSint is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenSint is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include "default.h"
#include "uart_isr.h"
#include "uart.h"

extern struct uartStruct *uartPtr;

/*
 * Initialize the UART to 9600 Bd, tx/rx, 8N1.
 */
struct uartStruct *uart_init(void)
{
	struct uartStruct *tmp;

#if F_CPU < 2000000UL && defined(U2X)
	/* improve baud rate error by using 2x clk */
	UCSRA = _BV(U2X);
	UBRRL = (F_CPU / (8UL * UART_BAUD)) - 1;
#else
	UBRRL = (F_CPU / (16UL * UART_BAUD)) - 1;
#endif

#if defined TXONLY
	/* Tx only without Rx */
	UCSRB = _BV(TXEN);
#endif

#if defined TXRX
	/* Tx and Rx only without interrupt */
	UCSRB = _BV(TXEN) | _BV(RXEN);
#endif

#if defined RXIONLY
	/* Rx only with interrupt */
	UCSRB = _BV(RXCIE) | _BV(RXEN);
#endif

#if defined TXRXI
	/* Rx with interrupt and Tx normal */
	UCSRB = _BV(RXCIE) | _BV(RXEN) | _BV(TXEN);
#endif

#if defined TXIRXI
	/* Rx and Tx with interrupt */
	UCSRB = _BV(RXCIE) | _BV(RXEN) | _BV(TXCIE) | _BV(TXEN);
#endif

	/* 8n2 */
	UCSRC = _BV(URSEL) | _BV(USBS) | _BV(UCSZ0) | _BV(UCSZ1);

	tmp = malloc(sizeof(struct uartStruct));
	tmp->rx_buffer = malloc(UART_RXBUF_SIZE);
	tmp->tx_buffer = malloc(UART_TXBUF_SIZE);
	tmp->rx_flag = 0;
	tmp->tx_flag = 0;
	tmp->rxIdx = 0;
	tmp->txIdx = 0;
	tmp->rx_buffer[0] = 0;
	tmp->tx_buffer[0] = 0;

	return (tmp);
}

void uart_get_msg(char *s)
{
	strcpy(s, uartPtr->rx_buffer);
	uartPtr->rx_flag = 0;
	uartPtr->rxIdx = 0;
	uartPtr->rx_buffer[0] = 0;
}

/*
 * Send character c down the UART Tx, wait until tx holding register
 * is empty.
 */

void uart_putchar(const char c)
{
	loop_until_bit_is_set(UCSRA, UDRE);
	UDR = c;
}

/*
 * Send a C (NUL-terminated) string down the UART Tx.
 */
void uart_printstr(const char *s)
{
	while (*s) {
		if (*s == '\n')
			uart_putchar ('\r');
		uart_putchar(*s++);
	}
}
