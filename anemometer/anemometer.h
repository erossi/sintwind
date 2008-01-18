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

#ifndef _ANEMOMETER_H_
#define _ANEMOMETER_H_

/*
  Minutes to use to calculate media
  It depends on cpu speed and prescaled timer
  4.19 sec/cycle, 1 min= 15 cycle, 5 min= 75 cycle
*/
#define MEDIA_MINUTES 75

enum wind_dir
{
  NORTH,
  NORTH_EAST,
  EAST,
  SOUTH_EAST,
  SOUTH,
  SOUTH_WEST,
  WEST,
  NORTH_WEST
};

enum wind_tendency
{
  INCREASE,
  DECREASE,
  STABLE
};

struct complex
{
  float x;
  float y;
};

struct wind_array
{
  /* spare */
  uint8_t flag;

  /* wind elements */

  /* speed */
  uint8_t speed, vmin, vmax;

  /* 0-359 degrees */
  int angle;

  enum wind_dir direction;
  enum wind_tendency tendency;

  /* real time elements */

  /* speed */
  uint8_t speed_rt, vmin_rt, vmax_rt;

  /* 0-359 degrees */
  int angle_rt;

  struct complex vector_rt, media_rt;
  uint8_t counter_rt;
};

enum wind_dir get_wind_direction (int direction);
void do_media (struct wind_array *wind);

#endif
