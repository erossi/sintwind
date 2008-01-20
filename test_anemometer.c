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

#include <inttypes.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include "default.h"
#include "anemometer/anemometer.h"
#include <avr/delay.h>

void
init_port_here (void)
{
  TCCR1A = 0;
  /* ICES1 = 0 trigger edge on negative */

  /* counter prescaler/1024 */
  /*  TCCR1B = _BV (ICNC1) | _BV (CS12) | _BV (CS10); */

  /* counter prescaler/256 */
  TCCR1B = _BV (ICNC1) | _BV (CS12);

  /*
     counter prescaler/64
     TCCR1B = _BV (ICNC1) | _BV (CS11) | _BV (CS10);

     PORTB output
     DDRB = 0xFF;

     PORTD input
   */
  ACE128_DDR = 0;

  /* enable interrupt on timer 0 interrupt */
  timer_enable_int (_BV (TICIE1) | _BV (TOIE1));
}

int
main (void)
{
  struct wind_array wind;

  init_port_here ();
  clear_wind_array ();

  sei ();

  for (;;)
    {
      if (wind.flag)
	{
	  do_media ();
	  wind.flag = 0;

	  printf ("RT: s[%3u] m[%3u] M[%3u] ", wind.speed, wind.vmin,
		  wind.vmax);
	  printf ("a[%3u] c[%2u] * ", wind.angle, wind.counter);
	  printf ("MT: s[%3u] m[%3u] M[%3u] ", wind.speed1, wind.vmin1,
		  wind.vmax1);
	  printf ("a[%3u] c[%2u]\n", wind.angle1, wind.counter1);

	}


         //      PORTB = ~(value | wind.speed1);
         PORTB = ~((wind.vmin1 << 4) | wind.speed);
       */

    };

  return (0);
}
