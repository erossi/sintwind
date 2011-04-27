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

/* These routines are used to read the wind speed and direction,
 * do media etc.
*/

#include <stdlib.h>
#include <inttypes.h>
#include <avr/eeprom.h>
#include "anemometer.h"

/*! type of wind sensor (GLOBAL) */
uint8_t EEMEM EE_sensor;

/*
   TRUE - data is valid
   FALSE - wrong data
 */
uint8_t anemometer_adjust(struct wind_array *wind)
{
	if (wind->sensor == ANE_DAVIS)
		return(davis_adjust());
	else
		return(lacrosse_adjust());
}

void anemometer_start(struct wind_array *wind)
{
	if (wind->sensor == ANE_DAVIS)
		davis_start();
	else
		lacrosse_start();
}

void anemometer_stop(struct wind_array *wind)
{
	if (wind->sensor == ANE_DAVIS)
		davis_stop();
	else
		lacrosse_stop();
}

void array_init(struct wind_array *wind)
{
	wind->flag = 0;		/* 0=ok take value 1=value taken */
	wind->speed_rt = 0;
	wind->angle_rt = 0;
	wind->speed = 0;
	wind->vmin = 255;
	wind->vmax = 0;
	wind->angle = 0;
	wind->direction = NORTH;
	wind->tendency = STABLE;
	wind->vmin_rt = 255;
	wind->vmax_rt = 0;
	wind->vector_rt.x = 0;
	wind->vector_rt.y = 0;
	wind->media_rt.x = 0;
	wind->media_rt.y = 0;
	wind->counter_rt = 0;
	wind->lacrosse_head=0;
	wind->lacrosse_bearing=0;
	wind->lacrosse_speed=0;
	wind->lacrosse_chksum=0;
}

/*! Read which wind sensor is stored into the EEPROM */
uint8_t anemometer_eeread(void)
{
	return(eeprom_read_byte(&EE_sensor));
}

void anemometer_eesave(uint8_t sensor)
{
	eeprom_write_byte(&EE_sensor, sensor);
}

struct wind_array *anemometer_init(struct wind_array *wind)
{
	uint8_t eesensor;

	eesensor = anemometer_eeread();
	wind = malloc(sizeof(struct wind_array));
	array_init(wind);

	switch (eesensor) {
		case ANE_LACROSSE:
			lacrosse_init();
			wind->sensor = ANE_LACROSSE;
			break;
		case ANE_DAVIS:
			davis_init();
			wind->sensor = ANE_DAVIS;
			break;
		default:
			if (lacrosse_is_connected()) {
				lacrosse_init();
				wind->sensor = ANE_LACROSSE;
			} else {
				davis_init();
				wind->sensor = ANE_DAVIS;
			}
	}

	return(wind);
}

/*
  return standard direction
  from encoder 0-359
*/
enum wind_dir get_wind_direction(int direction)
{
	enum wind_dir wind;

	wind = NORTH;

	/* Default case */
/*   if ((direction < 23) || (direction >= 338)) */
/*     wind = NORTH; */

	if ((direction >= 23) && (direction < 68))
		wind = NORTH_EAST;
	if ((direction >= 68) && (direction < 113))
		wind = EAST;
	if ((direction >= 113) && (direction < 158))
		wind = SOUTH_EAST;
	if ((direction >= 158) && (direction < 203))
		wind = SOUTH;
	if ((direction >= 203) && (direction < 248))
		wind = SOUTH_WEST;
	if ((direction >= 248) && (direction < 293))
		wind = WEST;
	if ((direction >= 293) && (direction < 338))
		wind = NORTH_WEST;

	return (wind);
}
