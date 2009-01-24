/*
 * Copyright (C) 2009 Enrico Rossi
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <avr/interrupt.h>
#include "default.h"
#include "uart.h"
#include <util/delay.h>

/* Globals */
struct uartStruct *uartPtr;

int
main (void)
{
  int i;

  DDRB = 127;
  PORTB = 0;

  uartPtr = uart_init();
  sei();

  for (;;)
    {
      if (uartPtr->rx_flag)
	{
          PORTB = ~(uartPtr->rxIdx);
	  uart_printstr(uartPtr->rx_buffer);
	  uartPtr->rx_flag = 0;
          uartPtr->rxIdx = 0;
          uartPtr->rx_buffer[0] = 0;
	}

      for (i = 0; i < 50; i++)
	_delay_ms (10);
    }
}
