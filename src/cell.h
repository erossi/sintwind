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

/*! \file cell.h
 * \brief cellular phone functions
 */

#ifndef CELL_H_
#define CELL_H_

#include "uart.h"

#define PHONE_PORT PORTB
#define PHONE_ON PB0

/*! if a fixed cellular operator must be used then
 * uncomment below and change the operator's code
 * example with I_WIND code: 22288
 * "at+cops=1,2,22288\n"
 */
/*
#define CELL_FIXED_OPERATOR "at+cops=1,2,<op-code>\n"
*/

void phone_send(const char *s);
void phone_init(void);
int phone_on(void);
int phone_msg(char *s);
void phone_answer(void);
void phone_hangup(void);
int phone_waitfor(const char *s, const int locked);

#endif
