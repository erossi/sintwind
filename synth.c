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
#include <avr/pgmspace.h>
#include "default.h"
#include "bus.h"
#include "synth.h"
/* put this after default because we have to set F_CPU */
#include <util/delay.h>

extern volatile wind_array wind; /* has to be cleared */

/* La frase stazione anemom. e' = 0 */

static const prog_uint8_t synth[] = {
  32, /* 0 */
  36, /* 2 */
  40, /* 4 */
  45, /* 6 */
  49, /* 8 */
  54, /* 10 */
  59, /* 12 */
  64, /* 14 */
  71, /* 16 */
  76, /* 18 */
  82, /* 20 */
  87, /* 22 */
  93, /* 24 */
  100, /* 26 */
  106,
  112,
  116,
  122,
  130,
  136,
  142,
  147,
  153,
  161,
  168,
  177,
  183,
  190,
  199,
  207,
  215,
  221,
  228,
  237,
  244,
  251, /* 70 */
  /* and for now on A8 = 1 */
  1, /* 72 */
  8, /* 74 */
  16, /* 76 */
  23, /* 78 */
  30, /* 80 */
  36, /* 41 a */
  39, /* 42 Dati non disponibili */
  49, /* 43 Est */
  54, /* 44 In aumento */
  61, /* 45 In calo */
  66, /* 46 Invariata */
  73, /* 47 Km/h */
  80, /* 48 Massima */
  85, /* 49 Minima */
  90, /* 50 Nord */
  95, /* 51 Oltre gli 80 */
  106, /* 52 Ovest */
  110, /* 53 Provenienza */
  117, /* 54 Sud */
  121, /* 55 Tendenza intensità vento */
  0 /* 56 Stazione anemomentrica del club... */
};

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
  switch_bus (SYNTH_CTRL, (_BV (PD) | _BV (CE)));
  _delay_ms (50);
}

void wait_for_eom (void)
{
  int i;

  switch_bus (SYNTH_EOM, 0);

  for (i=0;i<20000;i++)
    if (!(DATABUS_PIN & _BV(EOM)))
      i=21000;
    else
      _delay_ms (1);

  _delay_ms (25);
}

void say_it (uint8_t position)
{
  uint8_t out;

  /* set PD to 0 */
  if ((position > 35) && (position < 56)) /* set a8 to 1 */
    out = _BV (CE) | _BV (A8);
  else
    out = _BV (CE); /* PD = 0, /CE = 1, A8 = 0 */
  switch_bus (SYNTH_CTRL, out);
  _delay_ms (50);

  /* set a0-a8 to address the text */
  out = pgm_read_byte (&(synth[position]));
  switch_bus (SYNTH_ADDR, out);
  _delay_ms (50); /* Only for PD -> CE > 100msec */

  /* set /CE to 0 */
  if ((position > 35) && (position < 56))
    out = _BV (A8); /* PD = 0, /CE = 0, A8 = 1 */
  else
    out = 0; /* PD = 0, /CE = 0, A8 = 0 */
  switch_bus (SYNTH_CTRL, out);
  _delay_ms (5);

  /* set /CE to 1 */
  if ((position > 35) && (position < 56))
    DATABUS_PORT = _BV(CE) | _BV (A8); /* PD = 0, /CE = 1, A8 = 1 */
  else
    DATABUS_PORT = _BV(CE); /* PD = 0, /CE = 1, A8 = 0 */
  /* DATABUS_PORT |= _BV(CE); */
  _delay_ms (1);

  wait_for_eom ();
/*   reset_synth (); */
  pause_sentence ();
}

void play_message (void)
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
