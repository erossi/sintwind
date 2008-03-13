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
#include "synth/synth.h"
#include "anemometer/anemometer.h"
#include "anemometer/adc.h"

int
main (void)
{
  struct wind_array wind;

  adc_init ();
  synth_init ();
  synth_pause ();

  for (;;)
    {
      wind.angle = get_wind_position ();
      wind.direction = get_wind_direction (wind.angle);
      synth_say_wind_direction (wind.direction);
      synth_pause ();
    }
}
