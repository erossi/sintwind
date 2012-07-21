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

/*! file cell.c
 * \brief the cellular phone.
 */

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "cell.h"

/*! \brief send the string to the phone.
 *
 * \bug should be implemeted with PGM_P string to spare
 * some memory.
 */
void phone_send(const char *s)
{
	uart_printstr(MODEM_USART_PORT, s);
}

/*! \brief check for a message from the phone.
 * \param s pre-allocated string space.
 * \return 1 - ok message is presente, 0 - no message.
 */
uint8_t phone_msg(char *s)
{
	if (uartPtr->rx_flag) {
		uart_get_msg(MODEM_USART_PORT, s);
		return(TRUE);
	} else {
		return(FALSE);
	}
}

/*! \brief Wait for string from the modem.
 *
 * Used to wait a specific string from the modem in a locked or
 * tmout way. For example used after sending an AT command to 
 * the modem and wait for the OK string.
 * In a non-locked case, the waitfor will be 50*100msec long.
 *
 * \param s the string to wait for.
 * \param locked if locked, wait forever.
 * \return true - ok, false - error
 * \bug the string s should be checked not to be larger than the
 * allocated RX buffer size or this function will always fail.
 * \bug delay 100msec to wait for a new message should be
 * releated to the serial speed.
 *
 */
uint8_t phone_waitfor(const char *s, const uint8_t locked)
{
	uint8_t i, err;
	char *msg;

	/*! \bug non-multiport ready. works only to port 0.
	 */
	msg = malloc(UART0_RXBUF_SIZE);
	err = TRUE;

	if (locked)
		while (!(phone_msg(msg) && (!strcmp(msg, s))))
			_delay_ms(100);
	else
		for (i = 0; i < 50; i++)
			if (phone_msg(msg) && (!strcmp(msg, s))) {
				err = FALSE;
				i = 50;
			} else
				_delay_ms(100);

	free(msg);
	return (err);
}

/*! \brief initialize the serial port connected to the phone.
 *
 * and allocate the RXTX struct buffer.
 */
void phone_init(void)
{
	uartPtr = uart_init(MODEM_USART_PORT);
}

/*! \brief power up the phone.
 * \note turning on the modem will take from 11sec to 16 seconds
 */
uint8_t phone_on(void)
{
	int i;

	_delay_ms(1000);
	PHONE_PORT |= _BV(PHONE_ON);
	_delay_ms(500);
	PHONE_PORT &= ~(_BV(PHONE_ON));
	_delay_ms(10000);
	phone_send("AT&FE0&C0&D0\r");
	i = phone_waitfor("OK", FALSE);
#ifdef CELL_FIXED_OPERATOR
	phone_send(CELL_FIXED_OPERATOR);
	i = phone_waitfor("OK", FALSE);
#endif
	phone_send("AT^SNFS=5\r");
	return (i | phone_waitfor("OK", FALSE));
}

/*! \brief answer the call */
void phone_answer(void)
{
	phone_send("ATA\r");
	phone_waitfor("OK", 0);
}

/*! \brief drop the call */
void phone_hangup(void)
{
	phone_send("ATH0\r");
	phone_waitfor("OK", 0);
}
