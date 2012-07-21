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

/*! \file cell.h
 * \brief cellular phone functions
 *
 * \important Command sent to the modem MUST be terminated with
 * '\r' char and not '\n'.
 */

#ifndef _CELL_H_
#define _CELL_H_

#include "uart.h"

/*! \brief which port has the power ON switch.
 */
#define PHONE_PORT PORTB

/*! \brief which pin is the power ON.
 */
#define PHONE_ON PB0

/*! \brief on which USART port is the modem connected.
 */
#define MODEM_USART_PORT 0

/*! if a fixed cellular operator must be used then
 * uncomment below and change the operator's code
 * example with I_WIND code: 22288
 * "at+cops=1,2,22288\r"
 */
/*
#define CELL_FIXED_OPERATOR "at+cops=1,2,<op-code>\r"
*/

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

void phone_send(const char *s);
void phone_init(void);
uint8_t phone_on(void);
uint8_t phone_msg(char *s);
void phone_answer(void);
void phone_hangup(void);
uint8_t phone_waitfor(const char *s, const uint8_t locked);

#endif
