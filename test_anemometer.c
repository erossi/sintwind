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

#include <inttypes.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include "default.h"
#include "anemometer.h"
#include <avr/delay.h>
#include "usart.h"

volatile wind_array wind;

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
  uint8_t value;

  init_port_here ();
  clear_wind_array ();

  /* Set the baudrate to 9,600 bps using a 4MHz crystal */
  USART_Init (25);

  fdevopen (uart_putchar, NULL, 0);
  
  /* printf ("Hello world! \n"); */

  /* enable interrupts */
  sei ();

  for (;;)
    {
      if (wind.flag)
	{
	do_media ();
	wind.flag = 0;

        printf ("RT: s[%3u] m[%3u] M[%3u] ",wind.speed, wind.vmin, wind.vmax);
        printf ("a[%3u] c[%2u] * ", wind.angle, wind.counter);
        printf ("MT: s[%3u] m[%3u] M[%3u] ",wind.speed1, wind.vmin1, wind.vmax1);
        printf ("a[%3u] c[%2u]\n", wind.angle1, wind.counter1);

	}

      /*
      PORTB = ~(wind.speed1);
      _delay_ms (5000);
      PORTB = ~(wind.vmin1 | 32);
      _delay_ms (5000);
      PORTB = ~(wind.vmax1 | 64);
      _delay_ms (5000);

      switch (wind.direction1)
	{
	case NORTH:
	  value = _BV (0);
	  break;
	case NORTH_EAST:
	  value = _BV (0) | _BV (1);
	  break;
	case EAST:
	  value = _BV (1);
	  break;
	case SOUTH_EAST:
	  value = _BV (1) | _BV (2);
	  break;
	case SOUTH:
	  value = _BV (2);
	  break;
	case SOUTH_WEST:
	  value = _BV (2) | _BV (3);
	  break;
	case WEST:
	  value = _BV (3);
	  break;
	case NORTH_WEST:
	  value = _BV (3) | _BV (0);
	  break;
	default:
	  value = 15;
	}

      switch (wind.direction1)
	{
	case NORTH:
	  value = _BV (4);
	  break;
	case NORTH_EAST:
	  value = _BV (4) | _BV (5);
	  break;
	case EAST:
	  value = _BV (5);
	  break;
	case SOUTH_EAST:
	  value = _BV (5) | _BV (6);
	  break;
	case SOUTH:
	  value = _BV (6);
	  break;
	case SOUTH_WEST:
	  value = _BV (6) | _BV (7);
	  break;
	case WEST:
	  value = _BV (7);
	  break;
	case NORTH_WEST:
	  value = _BV (7) | _BV (4);
	  break;
	default:
	  value = 240;
	}

      //      PORTB = ~(value | wind.speed1);
      PORTB = ~((wind.vmin1 << 4) | wind.speed);
      */

    };

  return (0);
}
