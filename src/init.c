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

void port_a_init(void)
{
	/*
	 * pa0: IN - ADC0 Wind direction
	 * pa1-7: Unused
	 */

	PORTA = 0;
	DDRA = 0;
}

void port_b_init(void)
{
	/*
	 * pb0: OUT - IGT_IN mc35i on/off pulse
	 * pb1: xx - sht11 SDA used in/out on needed
	 * pb2: xx - sht11 SCL used in/out on needed
	 * pb3: IN - /EOM from synth
	 * pb4: OUT - PD to synth
	 * pb5: OUT - /CE to synth
	 * pb6: OUT - LD1 led
	 * pb7: unused
	 */

	/*
	 * Synth Pre init.
	 * Setting these before enable port to avoid
	 * playing message at startup.
	 * PD = 0, /CE = 1
	 */
	PORTB = _BV(5);
	DDRB = _BV(0) | _BV(4) | _BV(5) | _BV(6);
}

void port_c_init(void)
{
	/*
	 * pc0-7: OUT - sht11 A2-A9
	 */

	PORTC = 0;
	DDRC = 0xff;
}

void port_d_init(void)
{
	/*
	 * pd0-1: xx RS232 rx/tx
	 * pd6: IN - ICP1 Davis wind speed pulse
	 */

	PORTD = 0;
	DDRD = 0;
}

void port_init(void)
{
	port_a_init();
	port_b_init();
	port_c_init();
	port_d_init();
}

void array_init(struct wind_array *wind)
{
	wind->flag = 0;		/* 0=ok take value 1=value taken */
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
