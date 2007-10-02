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

#include <avr/io.h>
#include <avr/interrupt.h>
#include "default.h"
#include "init.h"

extern volatile wind_array wind;

void
init_port (void)
{
  /*
    Address bus bit 0..4 output
    This is the address bus.
  */
  ADDRBUS_DDR = 31; /* msb 00011111 lsb */

  /*
    Data bus can be input/output
    Set it to input for now
  */
  DATABUS_DDR = 0;
}


void
init_counter (void)
{
  TCCR1A = 0;
  /* ICES1 = 0 trigger edge on negative */

  /* counter prescaler/1024 */
#ifdef TIMER_PRESCALER_1024
   TCCR1B = _BV (ICNC1) | _BV (CS12) | _BV (CS10);
#endif

  /* counter prescaler/256 */
#ifdef TIMER_PRESCALER_256
  TCCR1B = _BV (ICNC1) | _BV (CS12);
#endif

  /* counter prescaler/64 */
#ifdef TIMER_PRESCALER_64
  TCCR1B = _BV (ICNC1) | _BV (CS11) | _BV (CS10);
#endif
  
  /* enable interrupt on timer 0 interrupt
   use include deprecated.h to use the function */
  TIMSK = _BV (TICIE1) | _BV (TOIE1);
}

void
clear_wind_array (void)
{
  wind.flag = 0; /* 0=ok take value 1=value taken */
  wind.speed = 0;
  wind.vmin = 255;
  wind.vmax = 0;
  wind.angle = 0;
  wind.direction = NORTH;
  wind.tendency = STABLE;
  wind.media_rt.x = 0;
  wind.media_rt.y = 0;
  wind.counter_rt = 0;
}
