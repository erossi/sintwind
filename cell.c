/* This file is part of OpenSint
 * Copyright (C) 2005-2007 Enrico Rossi
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
#include "bus.h"
#include "cell.h"
/* put this after default because we have to set F_CPU */
#include <util/delay.h>

uint8_t ring (void)
{
  uint8_t i;

  switch_bus (READ_PHONE, 0);
  i=DATABUS_PIN & _BV(CELL_RX);
  switch_bus (NULL_BUS, 0);
  return (i);
}

void answer_phone (void)
{
  uint8_t i;

  switch_bus (WRITE_PHONE, _BV(CELL_TX));
/*   Delay 1sec */
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
