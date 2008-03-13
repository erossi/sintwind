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

#ifndef _CELL_H_
#define _CELL_H_

#define CELL_RX 1
#define CELL_ACTIVE 0

#define CELL_POWER 0
#define CELL_TX 1
#define CELL_HANGUP 2
#define CELL_KEY1 3

uint8_t ring (void);
void answer_phone (void);
void hangup_phone (void);

#endif
