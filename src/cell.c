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
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "default.h"
#include "uart.h"
#include "cell.h"

extern struct uartStruct *uartPtr;

void phone_on(void)
{
	uint8_t i;

	/*
	 * Press the red button for 1 sec
	 * then release it and wait another second.
	 */

	PHONE_PORT |= _BV(PHONE_ON);
	for (i = 0; i < 100; i++)
		_delay_ms(10);

	PHONE_PORT &= ~(_BV(PHONE_ON));
	for (i = 0; i < 100; i++)
		_delay_ms(10);
}

void send(const char *s)
{
	uart_printstr(s);
}

int waitfor(const char *s)
{
	int i, j;

	j = 1;

	for (i = 0; i < 50; i++)
		if (uartPtr->rx_flag) {
			j = strcmp(uartPtr->rx_buffer, s);
			uartPtr->rx_flag = 0;
			uartPtr->rxIdx = 0;
			uartPtr->rx_buffer[0] = 0;
			i = 50;
		} else
			_delay_ms(10);

	return (j);
}

int phone_init(void)
{
	uartPtr = uart_init();
	phone_on();
	send("AT");
	return (waitfor("OK"));
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
	if (uartPtr->rx_flag) {
		uart_get_msg(s);
		return (1);
	} else
		return (0);
}

void phone_answer(void)
{
	send("ATA");
	waitfor("OK");
}

void phone_hangup(void)
{
	send("ATH");
	waitfor("OK");
}
