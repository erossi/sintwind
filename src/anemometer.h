/* This file is part of OpenSint
 * Copyright (C) 2005-2011 Enrico Rossi
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

/*! \file anemometer.h */

#ifndef ANEMOMETER_H_
#define ANEMOMETER_H_

#include "davis.h"
#include "lacrosse.h"

/*! type of anemometer used */
#define ANE_DAVIS 1
#define ANE_LACROSSE 2
#define ANE_AUTO 99

/*
  If We want to use the media value at run (x) as
  the first element of run (x+1).
  This lead to a wrong value into the 2 first run from the
  power up.
 */

#define MEDIA_NEXT_CYCLE

enum wind_dir {
	NORTH,
	NORTH_EAST,
	EAST,
	SOUTH_EAST,
	SOUTH,
	SOUTH_WEST,
	WEST,
	NORTH_WEST
};

enum wind_tendency {
	INCREASE,
	DECREASE,
	STABLE
};

struct complex {
	double x;
	double y;
};

struct wind_array {
	/*
	   Volatile stuff, used into ISR
	   flag: boolean used by ISR to tell to main there is a new
	   data (speed and direction) to be elaborated.
	   speed_rt: number of pulse x round detected.
	   angle_rt: 0-359 degrees detected
	 */
	volatile uint8_t flag;
	volatile int speed_rt;
	volatile int angle_rt;

	/* wind elements speed 0-359 degrees */
	int speed, vmin, vmax, angle;
	enum wind_dir direction;
	enum wind_tendency tendency;

	/* real time elements */
	int vmin_rt, vmax_rt;
	struct complex vector_rt, media_rt;
	uint8_t counter_rt;

	/* Type of anemometer: 0 for lacrosse */
	uint8_t sensor;

	/* LaCrosse data
	   [00000100][0000bbbb][000vvvvvvvvvvvv][0000cccc]
	 */
	uint8_t lacrosse_head, lacrosse_chksum;
	uint8_t lacrosse_bearing, lacrosse_nbearing;
	uint16_t lacrosse_speed, lacrosse_nspeed;
	uint8_t lacrosse_chkok; /* calculated checksum*/
	uint8_t lacrosse_loop; /* max number of failed reading */
};

/*! global wind array struct used into ISR. */
struct wind_array *wind;

uint8_t anemometer_adjust(struct wind_array *wind);
void anemometer_init(struct wind_array *wind);
enum wind_dir get_wind_direction(int direction);
void anemometer_start(struct wind_array *wind);
void anemometer_stop(struct wind_array *wind);
uint8_t anemometer_eeread(void);
void anemometer_eesave(uint8_t sensor);

#endif
