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

void port_init(void)
{
	/*
	 * Synth Pre-port init.
	 * Setting these before enable port to avoid
	 * playing message at startup.
	 * PD = 0, /CE = 1
	 */
	_SYNTH_CTRL_OUT = _BV(_SYNTH_CE);

	/*
	 * DDRB
	 * Synth PD and CE to out,
	 * phone ON and OFF to out.
	 */
	DDRB = _BV(_SYNTH_PD) | _BV(_SYNTH_CE) |
	    _BV(_PHONE_ON) | _BV(_PHONE_OFF);

	/*
	 * DDRD Synth address bus to OUT
	 */
	DDRD = 0xFF;

	/*
	 * Phone RING in
	 * should be so by default!
	 * DDRC = .... keep the default
	 */
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
}

