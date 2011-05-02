/* This file is part of OpenSint
 * Copyright (C) 2005-2011 Enrico Rossi
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

#ifndef UART_H_
#define UART_H_

#define UART_BAUD 9600
/* Keep in mind that you don't have enought RAM */
#define UART_RXBUF_SIZE 64
#define UART_TXBUF_SIZE 64
#define UART_RXBUF_MASK ( UART_RXBUF_SIZE - 1 )
#define UART_TXBUF_MASK ( UART_TXBUF_SIZE - 1 )

#if ( UART_RXBUF_SIZE & UART_RXBUF_MASK )
#error RX buffer size is not a power of 2
#endif

#if ( UART_TXBUF_SIZE & UART_TXBUF_MASK )
#error TX buffer size is not a power of 2
#endif

#define UART_EOL '\r'

struct uartStruct {
	char *rx_buffer;
	char *tx_buffer;
	volatile uint8_t rx_flag, tx_flag, rxIdx, txIdx;
};

struct uartStruct *uart_init(void);
void uart_get_msg(char *s);
void uart_putchar(const char c);
void uart_printstr(const char *s);

#endif
