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
#include "../default.h"
#include <util/delay.h>
#include "synth.h"

/* This maybe change in the future to non-global */
volatile wind_array wind;

void p (void)
{
  uint8_t i;

  /* pause 200ms */
  for (i=0; i<4; i++)
    _delay_ms (50);
}

int main (void)
{
  uint8_t i;
  
  synth_init ();
  p ();

  for (;;)
    {
      say_it (_SYNTH_S_CLUB);
      p();
      say_int (-1);
      p();

      for (i=0; i<20; i++)
	{
	say_int (i);
	p ();
	}

      for (i=20; i<110; i+= 10)
	{
	  say_int (i);
	  p ();
	  say_int (i+1);
	  p ();
	}

      for (i=0; i<200; i += 11)
	{
	  say_int (i);
	  p ();
	}

      say_it (_SYNTH_S_ABSENT);
      say_it (_SYNTH_S_EAST);
      say_it (_SYNTH_S_SOUTH);
      say_it (_SYNTH_S_WEST);
      say_it (_SYNTH_S_NORTH);
      say_it (_SYNTH_S_KMH);
      say_it (_SYNTH_S_TO);
      say_it (_SYNTH_S_TEMP);
      say_it (_SYNTH_S_MINUS);
      say_it (_SYNTH_S_COMMA);
      say_it (_SYNTH_S_PERC);
      say_it (_SYNTH_S_WIND);
      say_it (_SYNTH_S_CELSIUS);
      say_it (_SYNTH_S_UMIDITY);
      say_it (_SYNTH_S_DNA);
      say_it (_SYNTH_S_INTENSITY);
      say_it (_SYNTH_S_CLUB);

      p();
      p();
      p();
    }

/* Play the whole synth memory */
  for (;;)
    {
      for (i=0; i<97; i+=2)
	say_it (i);

      say_it (99);
      say_it (102);
      say_it (105);
      say_it (108);
      say_it (112);
    }
}
