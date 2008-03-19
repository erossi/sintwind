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
#include <avr/io.h>
#include "default.h"
/* put this after default because we have to set F_CPU */
#include <util/delay.h>
#include "synth.h"

void synth_pause (void)
{
  uint8_t i;

  /* 200ms */
  for (i=0; i<4; i++)
    _delay_ms (50);
}

void synth_reset (void)
{
  /* RESET SYNTH */
  /* errore devono essere up tutte e 2 alti */
  _SYNTH_CTRL_OUT = _BV (_SYNTH_PD) | _BV (_SYNTH_CE);
  _delay_ms (50);
}

void wait_for_eom (void)
{
/*   int i; */

/*   for (i=0;i<20000;i++) */
/*     if (!(_SYNTH_CTRL_IN & _BV(_SYNTH_EOM))) */
/*       i=21000; */
/*     else */
/*       _delay_ms (1); */

  loop_until_bit_is_clear (_SYNTH_CTRL_IN, _SYNTH_EOM);
  _delay_ms (25);
}

void say_it (uint8_t position)
{
  /* set PD to 0 */
  _SYNTH_CTRL_OUT = _BV (_SYNTH_CE); /* PD = 0, /CE = 1 */
  _delay_ms (50);

  /* set a0-a8 to address the text */
  _SYNTH_ADDR = position;
  _delay_ms (50); /* Only for PD -> CE > 100msec */

  /* set /CE to 0 */
  _SYNTH_CTRL_OUT = 0; /* PD = 0, /CE = 0 */
  _delay_ms (5);

  /* set /CE to 1 */
  _SYNTH_CTRL_OUT = _BV(_SYNTH_CE); /* PD = 0, /CE = 1 */
  _delay_ms (1);

  wait_for_eom ();
  /*   reset_synth (); */
}

/* This is capable of say number from 0 to 21 */
void say_int_0th (uint8_t value)
{
  say_it (value*2);
}

/* This is capable of say numbers from 20 to 99 */
void say_int_10th (uint8_t value)
{
  uint8_t dec,rest;
  uint8_t base_addr = 40;

  dec=value/10;
  rest=value%10;

  if (rest == 1)
    base_addr += 2;

  switch (dec)
    {
    case 2:
      say_it (base_addr);
      break;
    case 3:
      say_it (base_addr + 4);
      break;
    case 4:
      say_it (base_addr + 8);
      break;
    case 5:
      say_it (base_addr + 12);
      break;
    case 6:
      say_it (base_addr + 16);
      break;
    case 7:
      say_it (base_addr + 20);
      break;
    case 8:
      say_it (base_addr + 24);
      break;
    case 9:
      say_it (base_addr + 28);
      break;
    }

  if (rest > 1)
    say_int_0th (rest);
}

/* This is capable of say numbers from 100 to 199 */
void say_int_100th (uint8_t value)
{
  /* Say 100 in any case*/
  say_it (72);

  /* Say the rest */
  if (value>120)
    say_int_10th (value%100);
  else
    if (value>100)
      say_int_0th (value%100);
}

void say_int (int value)
{
  /*
    if value is negative
    say minus, make the number positive and go on.
  */

  if (value < 0)
    {
      say_it (_SYNTH_S_MINUS);
      value *= -1;
    }

  if (value < 20)
    say_int_0th (value);
  else
    if (value < 100)
      say_int_10th (value);
    else
      say_int_100th (value);
}

void synth_say_wind_direction (enum wind_dir direction)
{
  switch (direction)
    {
    case NORTH:
      say_it (_SYNTH_S_NORTH);
      break;

    case NORTH_EAST:
      say_it (_SYNTH_S_NORTH);
      say_it (_SYNTH_S_EAST);
      break;

    case EAST:
      say_it (_SYNTH_S_EAST);
      break;

    case SOUTH_EAST:
      say_it (_SYNTH_S_SOUTH);
      say_it (_SYNTH_S_EAST);
      break;

    case SOUTH:
      say_it (_SYNTH_S_SOUTH);
      break;

    case SOUTH_WEST:
      say_it (_SYNTH_S_SOUTH);
      say_it (_SYNTH_S_WEST);
      break;

    case WEST:
      say_it (_SYNTH_S_WEST);
      break;

    case NORTH_WEST:
      say_it (_SYNTH_S_NORTH);
      say_it (_SYNTH_S_WEST);
      break;
    }
}

void synth_play_message (struct wind_array *wind)
{

/*       say_it (_SYNTH_S_ABSENT); */
/*       say_it (_SYNTH_S_EAST); */
/*       say_it (_SYNTH_S_SOUTH); */
/*       say_it (_SYNTH_S_WEST); */
/*       say_it (_SYNTH_S_NORTH); */
/*       say_it (_SYNTH_S_KMH); */
/*       say_it (_SYNTH_S_TO); */
/*       say_it (_SYNTH_S_TEMP); */
/*       say_it (_SYNTH_S_MINUS); */
/*       say_it (_SYNTH_S_COMMA); */
/*       say_it (_SYNTH_S_PERC); */
/*       say_it (_SYNTH_S_WIND); */
/*       say_it (_SYNTH_S_CELSIUS); */
/*       say_it (_SYNTH_S_UMIDITY); */
/*       say_it (_SYNTH_S_DNA); */
/*       say_it (_SYNTH_S_INTENSITY); */
/*       say_it (_SYNTH_S_CLUB); */

  say_it (_SYNTH_S_CLUB);
  say_it (_SYNTH_S_WIND);
  if (wind->vmax)
  {
    synth_say_wind_direction (wind->direction);
    say_it (_SYNTH_S_INTENSITY);
    say_int (wind->vmin);
    say_it (_SYNTH_S_TO);
    say_int (wind->vmax);
    say_it (_SYNTH_S_KMH); /* Km/h */
  }
  else
    say_it (_SYNTH_S_DNA);

/*   synth_reset (); */
}
