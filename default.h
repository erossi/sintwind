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

#ifndef _DEFAULT_WIND_H_
#define _DEFAULT_WIND_H_

#define DATABUS_PIN PIND /* input data */
#define DATABUS_PORT PORTD /* output data */
#define DATABUS_DDR DDRD /* Data direction */
#define ADDRBUS_PORT PORTC /* address bus only set, no read */
#define ADDRBUS_DDR DDRC /* address bus setting */
#define ACE128_RATIO 2.82

/* CPU Speed 4 MHz */
#define F_CPU 4E6
/*
 #define F_CPU 4000000UL 4Mhz
 #define F_CPU 14.7456E6

 maximum delay possible is: (see util/delay.h)
 The maximal possible delay is 262.14 ms / F_CPU in MHz.
 so we have 4Mhz CPU, maximum is 65ms. (65.535 actually).
*/

/*
  Set the prescaler used for the timer.
 */

/* #define TIMER_PRESCALER_1024 */
#define TIMER_PRESCALER_256
/* #define TIMER_PRESCALER_64 */

/*
  define the next variable if we want to use the media value
  into the next cycle as the first element.
 */
#define MEDIA_NEXT_CYCLE 1

/* sample size 2^10 bit */
#define ADC_CUTOFF 9

#endif
