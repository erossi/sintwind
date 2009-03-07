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
#include <avr/interrupt.h>
#include "default.h"
#include "init.h"
/* sht11.h defined before synth.h to avoid
 * invalid pointer type in the struct sht11_t
 * used in synth_play_message.
 */
#include "sht11.h"
#include "synth.h"
#include "anemometer.h"
#include "media.h"
#include "cell.h"

/* Globals */
struct wind_array *wind;
volatile int loop;

int main(void)
{
	/* Global VARS */
	struct wind_array why_not_use_malloc;
	struct sht11_t why_not_use_malloc2;
	struct sht11_t *temperature;

	/*
	 * allocating variables
	 */

	wind = &why_not_use_malloc;
	loop = 0;
	temperature = &why_not_use_malloc2;

	/*
	 * initializing parts
	 */

	port_init();
	array_init(wind);
	anemometer_init();
	phone_init();
	sht11_init();

	synth_pause();

	/* Enable interrupt */
	sei();

	for (;;) {
		if (wind->flag) {
			do_media(wind);
			wind->flag = 0;
			sei();
		}

		if (ring()) {
			answer_phone();
			sht11_read_all(temperature);
			synth_play_message(wind, temperature);
			hangup_phone();
		}
	}
}
