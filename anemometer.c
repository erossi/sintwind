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

/* These routines are used to read the wind speed and direction,
 * do media etc.
*/

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include "default.h"
#include "ace128.h"
#include "anemometer.h"

int loop = 0;
extern volatile wind_array wind; /* has to be cleared */

/*
Test version:
I want to test if use the media value find out for a cycle as
the first element of the next cycle is better than starting
from a clean situation.

 - get vmin and vmax
 - do media for all v.x and v.y
 - get wind speed
 - convet v.x and v.y in angle
 - set direction from angle

 a) start from a clean situation every cycle:
    - clear all real time things
      reset vmin (255) and vmax(0)

 b) start using the media value into the next cycle as 1st. element
    - keep vmedia.x and y.
    - set the counter to 1.
    - set vmin and vmax to vmedia.

 to switch between a and b, use the MEDIA_NEXT_CYCLE set into
 default.h
 */

void store_media (void)
{
  float radiant;

  /* set min and max */
  wind.vmin = wind.vmin_rt;
  wind.vmax = wind.vmax_rt;

  /* media = sum / number of element */
  wind.media_rt.x /= wind.counter_rt;
  wind.media_rt.y /= wind.counter_rt;

  /*
    from complex back to p(r,theta)
    r = sqrt(x*x + y*y)
    theta = arc tangent (y/x)
  */
  wind.speed = sqrt ((wind.media_rt.x * wind.media_rt.x) + (wind.media_rt.y * wind.media_rt.y));

  /* securing from division by zero error in the atan div! */
  if (wind.media_rt.x == 0)
    wind.media_rt.x = 0.01;

  /*  theta in radiant */
  radiant = atan (wind.media_rt.y / wind.media_rt.x);

  /*
    Convert theta (radiant) to 360 degrees
    atan (-PI to PI)
    check your forgotten math school book
  */
  wind.angle = radiant * 180 / M_PI;

  if (wind.media_rt.x >= 0) /* 1th or 4th quadrant */
    wind.angle = 90 - wind.angle;
  else /* 2th or 3th quadrant */
    wind.angle = 270 - wind.angle;

  /* Get the direction from the angle */
  wind.direction = get_wind_direction (wind.angle);

  /* clear the real time media
  if you want to start from 0,0 you have to clean the media
  and reset mix and max. If you want the first element of the next
  cycle is the media of this cycle then keep the value.
  */
#ifndef MEDIA_NEXT_CYCLE
  /* case a) */
  wind.media_rt.x = 0;
  wind.media_rt.y = 0;
  wind.counter_rt = 0; /* clear real time counter */
  wind.vmin_rt = 255;
  wind.vmax_rt = 0;

#else
  /* case b) */
  wind.counter_rt = 1; /* set real time counter to 1 */
  wind.vmin_rt = wind.speed;
  wind.vmax_rt = wind.speed;
#endif
}

void
do_media (void)
{
  float radiant;

  /*
    Any time the main find it has a wind speed flag activate, it means
    we have a tick per minutes value to elaborate.
    This routine which is also hardware dependant is used to trasform
    ticks per minuter in (?? m/s) (?? km/h).
    REMEMBER: clear media_rt the first time (init)
    speed_rt and angle_rt are set by interrupt routine
  */

  /*
    Adjust wind speed, these value are hardware dependant.
    See spreadsheet.
  */
  if (wind.speed_rt < 140)
    wind.speed_rt /= 2.7;

  if ((wind.speed_rt >= 140) && (wind.speed_rt < 170))
    wind.speed_rt /= 3;

  if ((wind.speed_rt >= 170) && (wind.speed_rt < 210))
    wind.speed_rt /= 3.5;

  if (wind.speed_rt >= 210)
    wind.speed_rt /= 6;

  /*
    convert wind direction from degrees to radiant
    sin, cos etc. work with radiant.
    radiant = degrees * 2PI /360
  */
  radiant = wind.angle_rt * M_PI / 180;

  /*
    convert from polar p(r,theta) to complex p(x,y)
    theta 0 degrees is p(0,1)
    x = r sin(theta)
    y = r cos(theta)
  */
  wind.vector_rt.x = wind.speed_rt * sin (radiant);
  wind.vector_rt.y = wind.speed_rt * cos (radiant);

  /*
    collecting the sum of all x and y
    to calculate the media at the end
  */
  wind.media_rt.x += wind.vector_rt.x;
  wind.media_rt.y += wind.vector_rt.y;

  /* RT min e max */
  if (wind.speed_rt < wind.vmin_rt)
    wind.vmin_rt = wind.speed_rt;

  if (wind.speed_rt > wind.vmax_rt)
    wind.vmax_rt = wind.speed_rt;

  /* Read the default.h to know how many minutes we use */

#ifndef MEDIA_NEXT_CYCLE
  /* case a) see top comment we start counter from 0 */
  if (++wind.counter_rt == MEDIA_MINUTES)
    store_media ();

#else
  /* case b) see top comment we start counter from 1 */
  if (++wind.counter_rt > MEDIA_MINUTES)
    store_media ();

#endif
}

ISR (SIG_INPUT_CAPTURE1)
{
  static uint16_t timer = 0;
  /* only to implement low pass filter */
  uint16_t diff;

  /*
    Be care: Anemometer specific function.
    Different kind of anemometer may have different way of reading data.
    Any half turn we have a signal from the anemometer, so we trigger an
    interrupt to increment a counter. Note: this has to be a very quick
    routine because in a strong wind we may have impulses many times per
    second, keep also in
    mind that there are spikes also, which means that we may have a real uge
    qty of interrupt per second (this may be considered a bug).
    Any trigger simply increment by 1 a counter any half-turn of the anenom.
    in the meantime any minute we count how many half-turn we did so
    we learn the wind speed.
    A low-pass filter is implemented to filter spikes, we do this by looking
    to the timers increment since the last time this routine has been
    triggered.
    If this difference is to low (less than 10 ticks) I consider this time
    the routine has been triggere by a spike, not by a pulse form
    the anemometer.

    loop is the external common variable used to count the pulse per second.
  */

  /* How much time is passed? */
  diff = ICR1 - timer;

  /*
    if the difference between this front and the previous
    is less than 10 count prescaled, we have a spike,
    ignoring it.
  */
  if (diff > 10)
    {
      ++loop;
      timer = ICR1;
    }
}

ISR (SIG_OVERFLOW1)
{
  /*
    Any time the system trigger an overflow interrupt.
    wind flag is a boolean flag we enable to signal to the main that there
    is a time to be elaborated. Once the main take care of the new value, it
    switch off the flag, so if we find the flag on, it means that
    the main didn't take care of the last value (maybe it was too busy)
    so we simply do nothing.
  */

  if (!wind.flag)
    {
      wind.speed_rt = loop; /* for now 1 to 1 conversion */
      wind.angle_rt = get_wind_position ();
      wind.flag = 1;
    }

  loop = 0;
}
