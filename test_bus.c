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

/*
This test shoud enable all trigger and read in a loop
from the phone (2 pin)
from the syth (1 pin)
from the wind dir (8 pin)
and output to phone (4 pin)
 */

#include <inttypes.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "default.h"
#include "anemometer.h"
#include "init.h"
#include "bus.h"
/* put this after default because we have to set F_CPU */
#include <util/delay.h>

/* address bus MUST per global, since we never read it 
to discover where it is set. */
volatile wind_array wind;
enum address_bus addr_bus;

int
main (void)
{
  uint8_t value;

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

      switch_bus (WIND_DIR);
      _delay_ms (5000);

      switch_bus (READ_PHONE);
      _delay_ms (5000);

      addr_bus = WRITE_PHONE;
      switch_bus (addr_bus);
      _delay_ms (5000);

      addr_bus = READ_SYNTH;
      switch_bus (addr_bus);
      _delay_ms (5000);
    };

  return (0);
}
