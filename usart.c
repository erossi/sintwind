/* This file is part of OpenSint
 * Copyright (C) 2005-2007 Enrico Rossi
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

/*
  AVR306: Using the AVR UART in C
  Routines for polled USART
*/

/* Includes */
#include <inttypes.h>
#include <stdio.h>
#include <avr/io.h>
#include "usart.h"

/* Initialize UART */
void
USART_Init (uint16_t baudrate)
{
  /* Set the baud rate */
  UBRRH = (uint8_t) (baudrate >> 8);
  UBRRL = (uint8_t) baudrate;

  /* Enable UART receiver and transmitter */
  UCSRB = _BV(RXEN) | _BV(TXEN);

  /* 8N2 */
  UCSRC = _BV(URSEL) | _BV(USBS) | _BV(UCSZ0) | _BV(UCSZ1);
}


/* Read and write functions */
char
USART_Receive (void)
{
  /* Wait for incomming data */
  while ( !(UCSRA & _BV(RXC)) )
    ;
  /* Return the data */
  return UDR;
}

int
uart_putchar(char c)
{
  if (c == '\n')
    uart_putchar('\r');
  loop_until_bit_is_set(UCSRA, UDRE);
  UDR = c;
  return 0;
}
