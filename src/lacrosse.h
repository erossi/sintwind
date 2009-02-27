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

#ifndef LACROSSE_H_
#define LACROSSE_H_

/* delay in usec from rx bit */
/* should be 1250 us, but considering some delay due to 
 programming delay */
#define LACROSSE_RX_DELAY 1200

void lacrosse_init(void);
void lacrosse_start(void);
void lacrosse_stop(void);
uint8_t lacrosse_adjust(void);
uint8_t lacrosse_is_connected(void);

#endif
