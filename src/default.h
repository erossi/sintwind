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

#ifndef _DEFAULT_WIND_H_
#define _DEFAULT_WIND_H_

/*
 * Phone
 * On portB pin 4 and 5 must be out in DDR, there control
 * on and off key on the phone.
 * On portC pin 0 must be IN in DDRC, there check the RINGing.
 */

#define _PHONE_PORT PORTB
#define _PHONE_IN PINC
#define _PHONE_ON 4
#define _PHONE_OFF 5
#define _PHONE_RING 0

/*
 * Synth PORT
 */

#define _SYNTH_ADDR PORTD
#define _SYNTH_CTRL_OUT PORTB
#define _SYNTH_CTRL_IN  PINB
#define _SYNTH_EOM 1
#define _SYNTH_PD 2
#define _SYNTH_CE 3
/*
 * Hardcoded into init.c procedure
 *
#define _SYNTH_ADDR_DDR DDRD
#define _SYNTH_CTRL_DDR DDRB
*/

/*
 * Delay.h CPU speed definition
 */

/* CPU Speed 4 MHz */
#define F_CPU 4E6
/*
 #define F_CPU 4000000UL 4Mhz
 #define F_CPU 14.7456E6

 maximum delay possible is: (see util/delay.h)
 The maximal possible delay is 262.14 ms / F_CPU in MHz.
 so we have 4Mhz CPU, maximum is 65ms. (65.535 actually).
*/

/*
 *  Set the prescaler used for the timer.
 */

/* #define TIMER_PRESCALER_1024 */
#define TIMER_PRESCALER_256
/* #define TIMER_PRESCALER_64 */

/*
  If We want to use the media value calculated on 5 min. as
  the first element into the next cycle.
 */
#define MEDIA_NEXT_CYCLE 1

/* sample size 2^10 bit */
#define ADC_CUTOFF 9

enum wind_dir
{
  NORTH,
  NORTH_EAST,
  EAST,
  SOUTH_EAST,
  SOUTH,
  SOUTH_WEST,
  WEST,
  NORTH_WEST
};

enum wind_tendency
{
  INCREASE,
  DECREASE,
  STABLE
};

struct complex
{
  float x;
  float y;
};

struct wind_array
{
  /*
    Volatile stuff, used into ISR
    flag: boolean used by ISR to tell to main there is a new
          data (speed and direction) to be elaborated.
    speed_rt: number of pulse x round detected.
    angle_rt: 0-359 degrees detected
  */
  volatile uint8_t flag;
  volatile uint8_t speed_rt;
  volatile int angle_rt;


  /*
    wind elements
    speed
    0-359 degrees
  */
  uint8_t speed, vmin, vmax;
  int angle;
  enum wind_dir direction;
  enum wind_tendency tendency;

  /*
    real time elements
  */
  uint8_t vmin_rt, vmax_rt;
  struct complex vector_rt, media_rt;
  uint8_t counter_rt;
};

#endif
