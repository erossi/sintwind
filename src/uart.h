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

/*! \file uart.h
 * \brief Serial I/O.
 */

#ifndef _UART_H_
#define _UART_H_

/*! UART 0 baud rate */
#define UART0_BAUD 9600
/*! RX buffers size */
#define UART0_RXBUF_SIZE 64
/*! TX buffers size */
#define UART0_TXBUF_SIZE 64
/*! RX buffers mask, used to check the proper size and if
 * we are going out of size. */
#define UART0_RXBUF_MASK ( UART0_RXBUF_SIZE - 1 )
/*! TX buffers mask. */
#define UART0_TXBUF_MASK ( UART0_TXBUF_SIZE - 1 )

/* Check if something is wrong. */
#if ( UART0_RXBUF_SIZE & UART0_RXBUF_MASK )
#error RX0 buffer size is not a power of 2
#endif

#if ( UART0_TXBUF_SIZE & UART0_TXBUF_MASK )
#error TX0 buffer size is not a power of 2
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

/*! Structure with IO buffers and indexes */
struct uartStruct {
	char *rx_buffer;
	char *tx_buffer;
	volatile uint8_t rx_flag, tx_flag, rxIdx, txIdx;
};

/*! Global UART rxtx buffers pointer used inside the ISR routine. */
volatile struct uartStruct *uartPtr;

struct uartStruct *uart_init(const uint8_t port);
void uart_shutdown(const uint8_t port);
void uart_get_msg(const uint8_t port, char *s);
void uart_putchar(const uint8_t port, const char c);
void uart_printstr(const uint8_t port, const char *s);

#endif
