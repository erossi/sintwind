/* This file is part of OpenSint
 * Copyright (C) 2005-2008 Enrico Rossi
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
#include "cell.h"
/* put this after default because we have to set F_CPU */
#include <util/delay.h>

void phone_init (void)
{
  uint8_t i;

/*
 * Press the red button for 1 sec
 * then release it and wait another second.
 */

  _PHONE_PORT |= _BV (_PHONE_OFF);
  for (i=0; i<100; i++)
    _delay_ms (10);

  _PHONE_PORT &= ~(_BV (_PHONE_OFF));
  for (i=0; i<100; i++)
    _delay_ms (10);
}

uint8_t ring (void)
{
  return (bit_is_set (_PHONE_IN, _PHONE_RING));
}

void answer_phone (void)
{
  uint8_t i;

/*
 * Press the green button for 1/2 sec
 * then release it and wait another second.
 */

  _PHONE_PORT |= _BV (_PHONE_ON);
  for (i=0; i<50; i++)
    _delay_ms (10);

  _PHONE_PORT &= ~(_BV (_PHONE_ON));
  for (i=0; i<100; i++)
    _delay_ms (10);
}

/*
 * Keep in mind do not press red button
 * more than 1/2 sec. to avoid to turn off the phone in
 * case the communication is hangup before.
 */

void hangup_phone (void)
{
  uint8_t i;

  _PHONE_PORT |= _BV (_PHONE_OFF);
  for (i=0; i<50; i++)
    _delay_ms (10);

  _PHONE_PORT &= ~(_BV (_PHONE_OFF));
  for (i=0; i<100; i++)
    _delay_ms (10);
}

