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

#include <avr/io.h>
#include "default.h"
#include "init.h"
#include "led.h"
#include "adc.h"

/*
   this should be specified in default.
   but if synth addr is split over more ports this software
   can not be run.
*/

#ifndef SYNTH_ADDR
#define SYNTH_ADDR PORTC
#endif

int main(void)
{
	port_init();
	led_blink(1);
	wait_for_click();

	adc_init();
	led_blink(2);
	wait_for_click();

	for (;;) {
		SYNTH_ADDR = adc_wind_position();
	}
}
