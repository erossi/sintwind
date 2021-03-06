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

#include <inttypes.h>
#include <avr/io.h>
#include "init.h"

void port_a_init(void)
{
	/*
	 * pa0: IN - ADC0 Wind direction
	 * pa1: OUT - LaCrosse \CE
	 * pa2-7: Unused
	 */

	PORTA = 0;
	DDRA = _BV(LACROSSE_CE);
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
	 * pb7: IN - Manual click - force PULL-UP
	 */

	/*
	 * Synth Pre init.
	 * Setting these before enable port to avoid
	 * playing message at startup.
	 * PD = 0, /CE = 1
	 */
	PORTB = _BV(PB5) | _BV(PB7);
	DDRB = _BV(PB0) | _BV(PB4) | _BV(PB5) | _BV(PB6);
}

void port_c_init(void)
{
	/*
	 * pc0-7: OUT - synth A2-A9
	 */

	DDRC = 0xff;
	PORTC = 0;
}

void port_d_init(void)
{
	/*
	 * pd0-1: xx RS232 rx/tx
	 * pd2: INT0 rx lacrosse PULL-UP software and hardware
	 * pd6: IN - ICP1 Davis wind speed pulse
	 */

	PORTD = _BV(LACROSSE_RX);
	DDRD = 0;
}

void port_init(void)
{
	port_a_init();
	port_b_init();
	port_c_init();
	port_d_init();
}
