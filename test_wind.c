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
/* put this after default because we have to set F_CPU */
#include <util/delay.h>

/* address bus MUST per global, since we never read it 
to discover where it is set. */
volatile wind_array wind;
enum address_bus addr_bus;

void _delay_1s (uint8_t delay)
{
  uint8_t i;

  while (delay--)
    for (i=0; i<20; i++)
      _delay_ms (50);
}

int main (void)
{
  init_port ();
  reset_synth ();
  clear_wind_array ();
  init_counter ();

  /* enable interrupts */
  sei ();

  for (;;)
    {
      if (wind.flag)
	{
	do_media ();
	wind.flag = 0;
	cli (); /* disable interrupts */
	play_message ();
	sei ();
	}
    };

  return (0);
}
