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

#ifndef DAVIS_H_
#define DAVIS_H_

/*! \file davis.h
 * \brief davis specific functions */

#include "default.h"
#include "adc.h"
#include "anemometer.h"

/*! Filter cutoff to eliminate spike */
#define DAVIS_CUTOFF 5

void davis_init(void);
void davis_start(void);
void davis_stop(void);
uint8_t davis_adjust(void);

#endif
