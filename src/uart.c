/* This file is part of OpenSint
 * Copyright (C) 2005-2012 Enrico Rossi
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

/*! \file uart.c
 * \brief UART IO functions
 *
 * \bug only partially implemented multi-port. Many things are still
 * on port0 only.
 */

#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "uart.h"

/*! \brief Interrupt rx.
 *
 * This function is triggered every incoming char from the serial
 * port.
 */
ISR(USART0_RX_vect)
{
	static char tmp;

	/*! First copy the rx char from the device rx buffer to
	 * a tmp var.
	 */
	tmp = UDR0;

	/*! if we have more space in the preallocated rx buffer */
	if ((uartPtr->rxIdx + 1) & UART0_RXBUF_MASK) {
		/*! * copy the rx char in the current rx buffer slot. */
		uartPtr->rx_buffer[uartPtr->rxIdx] = tmp;

		/*! * increment the pointer to the next slot. */
		uartPtr->rxIdx++;

		/*! * put a 0 in the next slot to eventually
		 * terminate the string.
		 */
		uartPtr->rx_buffer[uartPtr->rxIdx] = 0;

		/*! * if the rx char is an EOL then flag a new
		 * message need to be processed by the software.
		 */
		if (tmp == UART0_EOL)
			uartPtr->rx_flag++;
	} else {
		/*! else something wrong happened, then clear
		 * the buffer index, the buffer contents and
		 * restart.
		 */
		uartPtr->rxIdx = 0;
		uartPtr->rx_buffer[0] = 0;
	}
}

/* \brief Initialize the UART0 to 9600 Bd, tx/rx, 8N1.
 *
 * \parameters port the serial port number.
 * \bug port should be used with multiport.
 */
struct uartStruct *uart_init(const uint8_t port)
{
	struct uartStruct *tmp;

#if F_CPU < 2000000UL && defined(U2X0)
	/* improve baud rate error by using 2x clk */
	UCSR0A = _BV(U2X0);
	UBRR0L = (F_CPU / (8UL * UART0_BAUD)) - 1;
#else
	UBRR0L = (F_CPU / (16UL * UART0_BAUD)) - 1;
#endif

#if defined TXONLY
	/* Tx only without Rx */
	UCSR0B = _BV(TXEN0);
#endif

#if defined TXRX
	/* Tx and Rx only without interrupt */
	UCSR0B = _BV(TXEN0) | _BV(RXEN0);
#endif

#if defined RXIONLY
	/* Rx only with interrupt */
	UCSR0B = _BV(RXCIE0) | _BV(RXEN0);
#endif

#if defined TXRXI
	/* Rx with interrupt and Tx normal */
	UCSR0B = _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0);
#endif

#if defined TXIRXI
	/* Rx and Tx with interrupt */
	UCSR0B = _BV(RXCIE0) | _BV(RXEN0) | _BV(TXCIE0) | _BV(TXEN0);
#endif

	/* 8n2 */
	UCSR0C = _BV(USBS0) | _BV(UCSZ00) | _BV(UCSZ01);

	tmp = malloc(sizeof(struct uartStruct));
	tmp->rx_buffer = malloc(UART0_RXBUF_SIZE);
	tmp->tx_buffer = malloc(UART0_TXBUF_SIZE);
	tmp->rx_flag = 0;
	tmp->tx_flag = 0;
	tmp->rxIdx = 0;
	tmp->txIdx = 0;
	tmp->rx_buffer[0] = 0;
	tmp->tx_buffer[0] = 0;

	return (tmp);
}

/*! \brief shutdown the usart port. */
void uart_shutdown(const uint8_t port)
{
        if (port) {
                UCSR1C = 0;
                UCSR1B = 0;
                UBRR1L = 0;
                UCSR1A = 0;
        } else {
                UCSR0C = 0;
                UCSR0B = 0;
                UBRR0L = 0;
                UCSR0A = 0;
        }
}

/*! get the message from the RX buffer.
 *
 * \parameters port the serial port.
 * \parameters s the string to copy the message to.
 * \bug multiport not implemented.
 */
void uart_get_msg(const uint8_t port, char *s)
{
	strcpy(s, uartPtr->rx_buffer);
	uartPtr->rx_flag = 0;
	uartPtr->rxIdx = 0;
	uartPtr->rx_buffer[0] = 0;
}

/*! Send character c to the serial port, wait until tx holding register
 * is empty.
 *
 * \parameter port the serial port.
 * \parameter c the char to send.
 */
void uart_putchar(const uint8_t port, const char c)
{
  if (port) {
          loop_until_bit_is_set(UCSR1A, UDRE1);
          UDR1 = c;
  } else {
          loop_until_bit_is_set(UCSR0A, UDRE0);
          UDR0 = c;
  }
}

/*! Send a C (NUL-terminated) string to the serial port.
 *
 * \parameter port the serial port.
 * \parameter s the string to send.
 */
void uart_printstr(const uint8_t port, const char *s)
{
	while (*s) {
		if (*s == '\n')
			uart_putchar(port, '\r');
		uart_putchar(port, *s++);
	}
}
