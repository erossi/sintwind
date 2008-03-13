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

/* These routines are used to read the wind speed and direction,
 * do media etc.
 */

#include <inttypes.h>
#include <avr/interrupt.h>
#include "../default.h"
#include "adc.h"
#include "anemometer.h"
#include "isr.h"

/* Global variable and pointer to be used */
/* inside the ISR routine */

extern volatile int loop;
extern struct wind_array *wind;

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
    is less than _ANEMOMETER_CUTOFF count prescaled, we have a spike,
    ignoring it.
  */
  if (diff > _ANEMOMETER_CUTOFF)
    {
      ++loop;
      timer = ICR1;
    }
}

ISR (SIG_OVERFLOW1)
{
  /*
    Any time the system trigger an overflow interrupt.
    The wind pointer must be present as global variables.
    wind flag is a boolean flag we enable to signal to the main that there
    is a time to be elaborated. Once the main take care of the new value, it
    switch off the flag, so if we find the flag on, it means that
    the main didn't take care of the last value (maybe it was too busy)
    so we simply do nothing.
    Wind->direction is supposed to be a media, not a single shot, this is
    why it will not be converted in here.
  */

  /* Stop the ISR */
  cli ();

  /* for now 1 to 1 conversion */
  wind->speed_rt = loop;

  /* 0-359 degrees */
  wind->angle_rt = get_wind_position ();
  wind->flag = 1;
  loop = 0;
}

void
isr_init (void)
{
  TCCR1A = 0;
  /* ICES1 = 0 trigger edge on negative */

  /*
    Time ellapsed every cycle is 4.19424 sec.
    4Mhz / 256 = 15625 Hz (clock pre-scaled of 256)
    1/15625 Hz = 0.000064 sec (every counter's step)
    Number of step per cycle is 65535 (16bit)
    0.000064 sec * 65535 = 4.19424 sec every cycle
  */

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
