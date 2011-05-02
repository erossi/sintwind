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

/*! \file default.h
 * \brief default definition for all functions.
 */
#ifndef DEFAULT_WIND_H_
#define DEFAULT_WIND_H_

/* ---- Delay.h CPU speed definition ---- */
/*
 * MUST BE IN MAKEFILE OR IN GCC -D F_CPU=4000000UL

 #define F_CPU 1000000UL 1Mhz
 #define F_CPU 4000000UL 4Mhz
 #define F_CPU 14.7456E6

 maximum delay possible is: (see util/delay.h)
 The maximal possible delay is 262.14 ms / F_CPU in MHz.
 so we have 4Mhz CPU, maximum is 65ms. (65.535 actually).
 Over this range you loose precision.
*/

/* ---- Cell and modem ---- */

#define UART_HAVE_DEFAULT
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

struct uartStruct {
	char *rx_buffer;
	char *tx_buffer;
	volatile uint8_t rx_flag, tx_flag, rxIdx, txIdx;
};

#define PHONE_HAVE_DEFAULT
#define PHONE_PORT PORTB
#define PHONE_ON PB0

#endif
