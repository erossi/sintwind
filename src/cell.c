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
}

uint8_t ring (void)
{
  uint8_t i;

  i = _PHONE_IN & _BV(_PHONE_RING);
  return (i);
}

void answer_phone (void)
{
  uint8_t i;

/*
 * Press the green button for 1 sec
 */

  _PHONE_PORT = _BV (_PHONE_ON);
  for (i=0; i<20; i++)
    _delay_ms (50);

  /*   Wait 1sec */
  switch_bus (WRITE_PHONE, 0);
  for (i=0; i<100; i++)
    _delay_ms (10);
}

void hangup_phone (void)
{
  uint8_t i;

  switch_bus (WRITE_PHONE, _BV(CELL_HANGUP));
/*   Delay 1sec */
  for (i=0; i<20; i++)
    _delay_ms (50);

  switch_bus (WRITE_PHONE, 0);
}
