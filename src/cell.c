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
#include <util/delay.h>
#include "default.h"
#include "cell.h"

void phone_init(void)
{
}

int phone_on(void)
{
/*
 * Press the red button for 1 sec
 * then release it and wait another second.
 */

	_delay_ms(1000);
	_PHONE_PORT |= _BV(_PHONE_OFF);
	_delay_ms(1000);
	_PHONE_PORT &= ~(_BV(_PHONE_OFF));
	_delay_ms(1000);
	return(1);
}

int phone_ring(void)
{
	return (bit_is_set(_PHONE_IN, _PHONE_RING));
}

void phone_answer(void)
{
/*
 * Press the green button for 1/2 sec
 * then release it and wait another second.
 */

	_PHONE_PORT |= _BV(_PHONE_ON);
	_delay_ms(500);
	_PHONE_PORT &= ~(_BV(_PHONE_ON));
	_delay_ms(1000);
}

/*
 * Keep in mind do not press red button
 * more than 1/2 sec. to avoid to turn off the phone in
 * case the communication is hangup before.
 */

void phone_hangup(void)
{
	_PHONE_PORT |= _BV(_PHONE_OFF);
	_delay_ms(500);
	_PHONE_PORT &= ~(_BV(_PHONE_OFF));
	_delay_ms(1000);
}
