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

#include <inttypes.h>
#include <avr/io.h>
#include "default.h"
#include "uart.h"
#include "cell.h"
/* put this after default because we have to set F_CPU */
#include <util/delay.h>

void phone_init(void)
{
  uint8_t i;

  uart_init();

/*
 * Press the red button for 1 sec
 * then release it and wait another second.
 */

  _PHONE_PORT |= _BV (_PHONE_ON);
  for (i=0; i<100; i++)
    _delay_ms (10);

  _PHONE_PORT &= ~(_BV (_PHONE_ON));
  for (i=0; i<100; i++)
    _delay_ms (10);
}

uint8_t phone_message(char *s) {
	if (uart->rx_flag) {
		/*
		   copia string
		   uartPtr->rx_buffer
		 */
		uart->rx_flag = 0;
		return(1);
	}
	else
		return(0);
}

void send(const char *s) {
	uart_printstr(s);
}

void waitfor(const char *s) {
}

uint8_t ring(void)
{
  return (bit_is_set (_PHONE_IN, _PHONE_RING));
}

void answer_phone (void)
{
	send('ATA');
	waitfor('OK');
}

void hangup_phone (void)
{
	send('ATH');
	waitfor('OK');
}

