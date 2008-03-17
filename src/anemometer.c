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

/* These routines are used to read the wind speed and direction,
 * do media etc.
*/

#include <inttypes.h>
#include "default.h"
#include "isr.h"
#include "adc.h"
#include "anemometer.h"

void anemometer_init (void)
{
  adc_init ();
  isr_init ();

/* Do We need a port Setup for the Pulse counter??? Maybe */
}

/*
  return standard direction
  from encoder 0-359
*/
enum wind_dir
get_wind_direction (int direction)
{
  enum wind_dir wind;

  wind = NORTH;

  /* Default case */
/*   if ((direction < 23) || (direction >= 338)) */
/*     wind = NORTH; */

  if ((direction >= 23) && (direction < 68))
    wind = NORTH_EAST;
  if ((direction >= 68) && (direction < 113))
    wind = EAST;
  if ((direction >= 113) && (direction < 158))
    wind = SOUTH_EAST;
  if ((direction >= 158) && (direction < 203))
    wind = SOUTH;
  if ((direction >= 203) && (direction < 248))
    wind = SOUTH_WEST;
  if ((direction >= 248) && (direction < 293))
    wind = WEST;
  if ((direction >= 293) && (direction < 338))
    wind = NORTH_WEST;

  return (wind);
}
