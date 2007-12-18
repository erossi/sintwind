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
#include <avr/io.h>
#include "../default.h"
/* put this after default because we have to set F_CPU */
#include <util/delay.h>
#include "synth.h"

extern volatile wind_array wind; /* has to be cleared */

void init_synth (void)
{
/* Address bus only output */
  SYNTH_ADDR_DDR = 0xFF;

/* PD and CE out, EOM in */
  SYNTH_CTRL_DDR = _BV (PD) | _BV (CE);
}

void pause_sentence (void)
{
  uint8_t i;

/* 200ms */
  for (i=0; i<4; i++)
    _delay_ms (50);
}

void reset_synth (void)
{
  /* RESET SYNTH */
/* errore devono essere up tutte e 2 alti */
  SYNTH_CTRL_OUT = _BV (PD) | _BV (CE);
  _delay_ms (50);
}

void wait_for_eom (void)
{
  int i;

  for (i=0;i<20000;i++)
    if (!(SYNTH_CTRL_IN & _BV(EOM)))
      i=21000;
    else
      _delay_ms (1);

  _delay_ms (25);
}

void say_it (uint8_t position)
{
  /* set PD to 0 */
  SYNTH_CTRL_OUT = _BV (CE); /* PD = 0, /CE = 1 */
  _delay_ms (50);

  /* set a0-a8 to address the text */
  SYNTH_ADDR = position;
  _delay_ms (50); /* Only for PD -> CE > 100msec */

  /* set /CE to 0 */
  SYNTH_CTRL_OUT = 0; /* PD = 0, /CE = 0 */
  _delay_ms (5);

  /* set /CE to 1 */
  SYNTH_CTRL_OUT = _BV(CE); /* PD = 0, /CE = 1 */
  _delay_ms (1);

  wait_for_eom ();
/*   reset_synth (); */
  pause_sentence ();
}

void synth_play_message (void)
{
  uint8_t i;

  say_it (56); /* Time to fly... intensita' vento da */

/*   vmin */
  i=wind.vmin/2;
  if (i > 40)
    i=51;

  say_it (i);

  say_it (41); /* a */

/*   vmax */
  i=wind.vmax/2;
  if (i > 40)
    i=51;

  say_it (i);

  say_it (47); /* Km/h */

  say_it (53); /* Provenienza */

  switch (wind.direction)
    {
    case NORTH:
      say_it (50);
      break;

    case NORTH_EAST:
      say_it (50);
      say_it (43);
      break;

    case EAST:
      say_it (43);
      break;

    case SOUTH_EAST:
      say_it (54);
      say_it (43);
      break;

    case SOUTH:
      say_it (54);
      break;

    case SOUTH_WEST:
      say_it (54);
      say_it (52);
      break;

    case WEST:
      say_it (52);
      break;

    case NORTH_WEST:
      say_it (50);
      say_it (52);
      break;
    }

/*   say_it (55); /\* Tendenza *\/ */

  reset_synth ();
}
