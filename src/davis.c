/* This file is part of OpenSint
 * Copyright (C) 2005-2012 Enrico Rossi
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

/*! \file davis.c
 * \brief These routines are used to read the wind speed and direction,
 * do media etc.
 *
 * \note the wind struct is global.
 */

#include <inttypes.h>
#include <avr/interrupt.h>
#include "davis.h"

/*! Global variable and pointer to be used inside the ISR routine */
volatile int loop;

/*! \brief wind speed counter ISR
   Different kind of anemometer may have different way of reading data.
   Any half turn we have a signal from the anemometer, so we trigger an
   interrupt to increment a counter.
   Any trigger simply increment by 1 a counter any
   half-turn of the anenom.
   In the meantime any minute we count how many half-turn we did so
   we learn the wind speed.
   A low-pass filter is implemented to filter spikes,
   we do this by looking
   to the timers increment since the last time this routine has been
   triggered.
   If this difference is to low (less than 10 ticks) I consider
   this time
   the routine has been triggere by a spike, not by a pulse form
   the anemometer.
   \var loop is the external common variable used to count the
   pulse per second.
   \note Anemometer specific function.
   \note this has to be a very quick routine because
   in a strong wind we may have impulses many times per
   second, keep also in
   mind that there are spikes also, which means that we may
   have a real huge
   qty of interrupt per second (this may be considered a bug).
   */
ISR(TIMER1_CAPT_vect)
{
	static uint16_t timer = 0;
	/*! only to implement low pass filter */
	uint16_t diff;

	/* How much time is passed? */
	diff = ICR1 - timer;

	/*
	   if the difference between this front and the previous
	   is more than DAVIS_CUTOFF, we have a valid pulse, else
	   it is a spike and ignore it.
	 */
	if (diff > DAVIS_CUTOFF) {
		++loop;
		timer = ICR1;
	}
}

/*! \brief calulate wind speed with collected pulse value.
   Any time the system trigger an overflow interrupt.
   The wind pointer must be present as global variables.
   wind flag is a boolean flag we enable to signal to
   main that there is a time to be elaborated.
   Once the main take care of the new value, it
   switch off the flag, so if we find the flag on, it means that
   the main didn't take care of the last value (maybe it was too busy)
   so we simply do nothing.
   Wind->direction is supposed to be a media, not a single shot, this is
   why it will not be converted in here.

   \note the wind struct is global.
   */
ISR(TIMER1_OVF_vect)
{
	/* for now 1 to 1 conversion */
	wind->speed_rt = loop;

	/* 0-359 degrees */
	wind->angle_rt = adc_wind_position();
	wind->flag = 1;
	loop = 0;
}

/*! \brief setup counters and timers.
 * Time ellapsed every cycle is 4.19424 sec.
 * 4Mhz / 256 = 15625 Hz (clock pre-scaled of 256)
 * or
 * 1Mhz / 64 = 15625 Hz (clock pre-scaled of 64)
 * 1/15625 Hz = 0.000064 sec (every counter's step)
 * Number of step per cycle is 65535 (16bit)
 * 0.000064 sec * 65535 = 4.19424 sec every cycle.
 *
 * \note this example is based on 4Mhz cpu clock. Adjust
 * the prescaler to your settings.
 */
void davis_timer_setup(void)
{
	TCCR1A = 0;

	/* ICES1 = 0 trigger edge on negative */

	/*! The counter is:
	 * prescaler/256 (4Mhz) CS12
	 * prescaler/64 (1Mhz) CS11 and CS10
	 * noise canceller is enable and the trigger edge is on
	 * positive.
	 */
#if F_CPU == 1000000UL
	TCCR1B = _BV(ICNC1) | _BV(CS11) | _BV(CS10);
#elif F_CPU == 4000000UL
	TCCR1B = _BV(ICNC1) | _BV(CS12);
#else
#error "Unsupported CPU speed in davis.c"
#endif

	/*! enable interrupt on timer 0 event and overflow.
	 */
	TIMSK1 = _BV(ICIE1) | _BV(TOIE1);
}

/*! \brief initialize the davis anemometer. */
void davis_init(void)
{
	loop = 0;
	adc_init();
	davis_timer_setup();
}

/*! \brief anemometer value fine tuning and error correction.
 * Any time the main find it has a wind speed flag active, it means
 * we have a tick per cycle to elaborate.
 * This routine is used to trasform
 * ticks per cycle in (?? m/s) (?? km/h).
 * \note clear media_rt the first time (init).
 * \note speed_rt and angle_rt are set by interrupt routine.
 * \todo This routine should also check the validity of these value someway.
 * \return TRUE - ok the value has been corrected, FALSE - error.
 * \bug return is always TRUE, please fix.
 */
uint8_t davis_adjust(void)
{
	/*! Adjust wind speed, these value are hardware dependant.
	   See spreadsheet.
	 */
	wind->speed_rt /= 1.35;
	return(1);
}
