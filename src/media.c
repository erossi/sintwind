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
#include <math.h>
#include "default.h"
#include "anemometer.h"
#include "media.h"

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

/* Standard deviation sd = sqrt(sum(Xi^2)/n - Xmed^2) */

double sqr(double x)
{
	return(x*x);
}

double module(double x, double y)
{
	double m;

	m = sqrt(sqr(x) + sqr(y));
	return (m);
}

void store_media(struct wind_array *wind)
{
	double radiant;

	/* set min and max */
	wind->vmin = wind->vmin_rt;
	wind->vmax = wind->vmax_rt;

	/* if we started with the previous cycle's media,
	   than we have one more element.
	   */
#ifdef MEDIA_NEXT_CYCLE
	wind->counter_rt++;
#endif

	/* media = sum / number of element */
	wind->media_rt.x /= wind->counter_rt;
	wind->media_rt.y /= wind->counter_rt;

	/* from complex back to p(r,theta) r=sqrt(x*x + y*y) theta=atan(y/x) */
	wind->speed = sqrt(sqr(wind->media_rt.x) + sqr(wind->media_rt.y));

	/* securing from division by zero error in the atan div! */
	if (wind->media_rt.x == 0)
		radiant = atan(wind->media_rt.y / 0.01);
	else
		radiant = atan(wind->media_rt.y / wind->media_rt.x);

	/* Convert theta (radiant) to 360 degrees atan (-PI to PI) */
	wind->angle = radiant * 180 / M_PI;

	if (wind->media_rt.x >= 0)	/* 1th or 4th quadrant */
		wind->angle = 90 - wind->angle;
	else			/* 2th or 3th quadrant */
		wind->angle = 270 - wind->angle;

	/* Get the direction from the angle */
	wind->direction = get_wind_direction(wind->angle);

	/* clear the real time media
	   if you want to start from 0,0 you have to clean the media
	   and reset min and max. If you want the first element of the next
	   cycle is the media of this cycle then keep the value.
	 */
	wind->counter_rt = 0;	/* clear real time counter */

#ifdef MEDIA_NEXT_CYCLE
	wind->vmin_rt = wind->speed;
	wind->vmax_rt = wind->speed;
#else
	wind->media_rt.x = 0;
	wind->media_rt.y = 0;
	wind->vmin_rt = 255;
	wind->vmax_rt = 0;
#endif
}

void media(struct wind_array *wind)
{
	double radiant;

	/* 
	   Adjust value based on the sensor type and specs.
	   after this point wind->speed_rt and wind->angle_rt
	   must contain real km/h and degrees.
	 */
	/* Ignoring wrong data (lacrosse checksum TOBEFIXED) */
	anemometer_adjust(wind);

	/*
	   convert wind direction from degrees to radiant
	   sin, cos etc. work with radiant.
	   radiant = degrees * 2PI /360
	 */
	radiant = wind->angle_rt * M_PI / 180;

	/*
	   convert from polar p(r,theta) to complex p(x,y)
	   theta 0 degrees is p(0,1)
	   x = r sin(theta)
	   y = r cos(theta)
	 */
	wind->vector_rt.x = wind->speed_rt * sin(radiant);
	wind->vector_rt.y = wind->speed_rt * cos(radiant);

	/* collecting the sum of all x and y to calculate the media. */
	wind->media_rt.x += wind->vector_rt.x;
	wind->media_rt.y += wind->vector_rt.y;

	/* RT min e max */
	if (wind->speed_rt < wind->vmin_rt)
		wind->vmin_rt = wind->speed_rt;

	if (wind->speed_rt > wind->vmax_rt)
		wind->vmax_rt = wind->speed_rt;

	/* Read the media.h to know how many ticks we use. */
	if (++wind->counter_rt == MEDIA_MINUTES)
		store_media(wind);

	wind->flag = 0;
}
