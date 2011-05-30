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

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "cell.h"

/*! \bug define this in its files */
extern struct uartStruct *uartPtr;

/*! \brief send the string to the phone */
void phone_send(const char *s)
{
	uart_printstr(s);
}

/*! compare the 2 strings */
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

		if (strlen(s) > 2)
			return(1);
	}

	return (0);
}

/* Wait for string from rs232, if locked, wait forever */
int phone_waitfor(const char *s, const int locked)
{
	int i, j;
	char *msg;

	msg = malloc(UART_RXBUF_SIZE);
	j = 1;

	if (locked)
		while (!(phone_msg(msg) && phone_valid_msg(msg, s)))
			_delay_ms(100);
	else
		for (i = 0; i < 50; i++)
			if (phone_msg(msg) && phone_valid_msg(msg, s)) {
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

/* Turning on the modem will take from 11sec to 16sec */
int phone_on(void)
{
	int i;

	_delay_ms(1000);
	PHONE_PORT |= _BV(PHONE_ON);
	_delay_ms(500);
	PHONE_PORT &= ~(_BV(PHONE_ON));
	_delay_ms(10000);
	phone_send("AT&FE0&C0&D0\n");
	i = phone_waitfor("OK", 0);
#ifdef CELL_FIXED_OPERATOR
	phone_send(CELL_FIXED_OPERATOR);
	i = phone_waitfor("OK", 0);
#endif
	phone_send("AT^SNFS=5\n");
	return (i | phone_waitfor("OK", 0));
}

void phone_answer(void)
{
	phone_send("ATA\n");
	phone_waitfor("OK", 0);
}

void phone_hangup(void)
{
	phone_send("ATH0\n");
	phone_waitfor("OK", 0);
}
