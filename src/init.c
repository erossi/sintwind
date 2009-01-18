/* This file is part of OpenSint
 * Copyright (C) 2005-2009 Enrico Rossi
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
#include "init.h"

void port_b_init (void)
{
  /*
   * pb0: IN - Davis wind speed pulse
   * pb1: IN - /EOM from synth
   * pb2: OUT - PD to synth
   * pb3: OUT - /CE to synth
   * pb4: OUT - A2 addr bus synth
   * pb5: OUT - A3 addr bus synth
   * pb6-7: UnUsed
   */

  /*
   * Synth Pre init.
   * Setting these before enable port to avoid
   * playing message at startup.
   * PD = 0, /CE = 1
   */
  PORTB = _BV (3);
  DDRB = _BV (2) | _BV (3) | _BV (4) | _BV (5);
}

void port_c_init (void)
{
  /*
  * pc0: IN - ?
  * pc1: IN - ADC1 Wind direction
  * pc2: OUT - mc35i IGT_IN
  * pc3: OUT - Led LD1
  * pc4: xx - sht11 SDA used in/out on needed
  * pc5: xx - sht11 SCL used in/out on needed
  * pc6-7: UnUsed
  */

  PORTC = 0;
  DDRC = _BV (2) | _BV (3);
}

void port_d_init (void)
{
  /*
   * pd0-1: xx RS232 rx/tx
   * ps2-7: OUT - sht11 A4-A9
   */
  DDRD = 0xFC;
}

void port_init (void)
{
  port_b_init ();
  port_c_init ();
  port_d_init ();
}

void array_init (struct wind_array *wind)
{
  wind->flag = 0; /* 0=ok take value 1=value taken */
  wind->speed = 0;
  wind->vmin = 255;
  wind->vmax = 0;
  wind->angle = 0;
  wind->direction = NORTH;
  wind->tendency = STABLE;
  wind->media_rt.x = 0;
  wind->media_rt.y = 0;
  wind->counter_rt = 0;
}
