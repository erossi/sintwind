/* This file is part of OpenSint
 * Copyright (C) 2005-2009 Enrico Rossi
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

#ifndef ANEMOMETER_H_
#define ANEMOMETER_H_

uint8_t anemometer_adjust(struct wind_array *wind);
void anemometer_init(struct wind_array *wind);
enum wind_dir get_wind_direction(int direction);
void anemometer_start(struct wind_array *wind);
void anemometer_stop(struct wind_array *wind);

#endif
