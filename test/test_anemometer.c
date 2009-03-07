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

#include <inttypes.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "default.h"
#include "init.h"
#include "lacrosse.h"

/* Globals */
struct wind_array *wind;
volatile int loop;

int main(void)
{
	/* Global VARS */
	struct wind_array why_not_use_malloc;

	wind = &why_not_use_malloc;
	loop = 0;

	port_init();
	array_init(wind);
	lacrosse_init();

	/* Enable interrupt */
	sei();

	for (;;)
		if (wind->flag) {
			PORTC = 85;
			_delay_ms(500);
			PORTC = (uint8_t)(wind->lacrosse & 0xff); 	
			wind->flag = 0;
		}
}
