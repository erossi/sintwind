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
#include <avr/interrupt.h>
#include "default.h"
#include "anemometer.h"
#include "init.h"
#include "synth.h"
#include "cell.h"

/* address bus MUST per global, since we never read it 
to discover where it is set. */
volatile wind_array wind;
enum address_bus addr_bus;

int
main (void)
{
  reset_synth ();
  init_port ();
  clear_wind_array ();

  /* enable interrupts */
  sei ();

  for (;;)
    {
      if (wind.flag)
	{
	do_media ();
	wind.flag = 0;
	}

      if (ring())
	{
	  cli (); /* disable interrupts */
	  play_message ();
	  sei ();
	}

      _delay_1s (1);
    };

  return (0);
}
