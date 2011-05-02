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

/*! \file default.h
 * \brief default definition for all functions.
 */
#ifndef DEFAULT_WIND_H_
#define DEFAULT_WIND_H_

/* ---- Delay.h CPU speed definition ---- */
/*
 * MUST BE IN MAKEFILE OR IN GCC -D F_CPU=4000000UL

 #define F_CPU 1000000UL 1Mhz
 #define F_CPU 4000000UL 4Mhz
 #define F_CPU 14.7456E6

 maximum delay possible is: (see util/delay.h)
 The maximal possible delay is 262.14 ms / F_CPU in MHz.
 so we have 4Mhz CPU, maximum is 65ms. (65.535 actually).
 Over this range you loose precision.
*/

#define PHONE_HAVE_DEFAULT
#define PHONE_PORT PORTB
#define PHONE_ON PB0

#endif
