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
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "default.h"
#include "uart.h"
#include "cell.h"

extern struct uartStruct *uartPtr;

void send(const char *s)
{
	uart_printstr(s);
}

int phone_valid_msg(const char *s1, const char *s2)
{
	if (strstr(s1, s2))
		return (1);
	else
		return (0);
}

int phone_msg(char *s)
{
	int i;

	i = 0;

	if (uartPtr->rx_flag) {
		uart_get_msg(s);

		if (strlen(s) > 2)
			i = 1;
	}

	return (i);
}

int waitfor(const char *s)
{
	int i, j;
	char *msg;

	msg = malloc(UART_RXBUF_SIZE);
	j = 1;

	for (i = 0; i < 50; i++)
		if (phone_msg(msg) && phone_valid_msg(msg, s))
		{
			j = 0;
			i = 50;
		} else
			_delay_ms(100);

	free(msg);
	return (j);
}

void phone_init(void)
{
	uartPtr = uart_init();
}

int phone_on(void)
{
	int i;

	_delay_ms(1000);
	PHONE_PORT |= _BV(PHONE_ON);
	_delay_ms(500);
	PHONE_PORT &= ~(_BV(PHONE_ON));
	_delay_ms(10000);
	send("AT&FE0&C0&D0");
	i = waitfor("OK");
	send("AT^SNFS=5");
	return (i | waitfor("OK"));
}

void phone_answer(void)
{
	send("ATA");
	waitfor("OK");
}

void phone_hangup(void)
{
	send("ATH0");
	waitfor("OK");
}
