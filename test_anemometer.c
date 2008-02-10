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
#include <avr/interrupt.h>
#include "default.h"
#include "init.h"
#include "synth/synth.h"
#include "anemometer/anemometer.h"

/* This must not be used */
#include "anemometer/adc.h"

/* Globals */
struct wind_array *wind;
volatile int loop;

int
main (void)
{
  /* Global VARS */
  struct wind_array why_not_use_malloc;

  wind = &why_not_use_malloc;
  loop = 0;
  array_init (wind);
  anemometer_init ();
  synth_init ();
  synth_pause ();

/* Enable interrupt */
  sei ();

  for (;;)
    if (wind->flag)
	{

/* This must be used into general media stuff */
	  wind->direction = get_wind_direction (wind->angle);

	  synth_say_wind_direction (wind->direction);
	  say_int (wind->speed_rt);
	  synth_pause ();
	  wind->flag = 0;
	  sei ();
	}
}
