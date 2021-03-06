/* This file is part of OpenSint
 * Copyright (C) 2005-2011 Enrico Rossi
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

#ifndef MEDIA_H_
#define MEDIA_H_

#include "anemometer.h"

/*
  Minutes to use to calculate media
  It depends on cpu speed and prescaled timer
  4.19 sec/cycle, 1 min= 15 cycle, 5 min= 75 cycle
*/

#define MEDIA_MINUTES 15

/*
   MEDIA_NEXT_CYCLE defines the first element (0) of
   real time value. If defined we use medium value,
   if undefined we reset the value.
 */

void media(struct wind_array *wind);

#endif
