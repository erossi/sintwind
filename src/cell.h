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

#ifndef _CELL_H_
#define _CELL_H_

/*
 * Phone
 * On portB pin 4 and 5 must be out in DDR, there control
 * on and off key on the phone.
 * On portC pin 0 must be IN in DDRC, there check the RINGing.
 */

#define _PHONE_PORT PORTB
#define _PHONE_IN PINC
#define _PHONE_ON 4
#define _PHONE_OFF 5
#define _PHONE_RING 0

void phone_init (void);
uint8_t ring (void);
void answer_phone (void);
void hangup_phone (void);

#endif
