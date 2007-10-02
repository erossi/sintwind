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
#include <stdio.h>
#include <avr/io.h>
#include "default.h"
#include "ace128.h"

/* Simple test program to output the direction read by */
/* the encoder. */

int
main (void)
{
/*   enum wind_dir direction; */
/*   int angle; // 0-359 degrees */
  volatile wind_array wind;
  uint8_t value;

  /* PORTD output */
  DDRD = 0xff;

  /* PORTC input */
  DDRC = 0;

  for (;;)
    {
      wind.angle = get_wind_position ();
      wind.direction = get_wind_direction (wind.angle);

      switch (wind.direction)
	{
	case NORTH:
	  value = _BV (4);
	  break;
	case NORTH_EAST:
	  value = _BV (4) | _BV (5);
	  break;
	case EAST:
	  value = _BV (5);
	  break;
	case SOUTH_EAST:
	  value = _BV (5) | _BV (6);
	  break;
	case SOUTH:
	  value = _BV (6);
	  break;
	case SOUTH_WEST:
	  value = _BV (6) | _BV (7);
	  break;
	case WEST:
	  value = _BV (7);
	  break;
	case NORTH_WEST:
	  value = _BV (7) | _BV (4);
	  break;
	default:
	  value = 240;
	}

      PORTD = ~value;
/*       PORTD = ~(uint8_t)wind.angle; */
    };

  return (0);
}
