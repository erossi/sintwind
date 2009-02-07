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
#include <stdlib.h>
#include <util/delay.h>
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
#include "uart.h"
#include "led.h"

/* Globals */
struct wind_array *wind;
volatile int loop;
struct uartStruct *uartPtr;

int main(void)
{
	struct sht11_t *temperature;
	char *message;
	int i;

	loop = 0;
	wind = malloc(sizeof(struct wind_array));
	temperature = malloc(sizeof(struct sht11_t));
	message = malloc(UART_RXBUF_SIZE);

	/*
	 * initializing parts
	 */

	port_init();
	led_blink(1);
	wait_for_click();

	array_init(wind);
	led_blink(2);
	wait_for_click();

	anemometer_init();
	led_blink(3);
	wait_for_click();

	sht11_init();
	led_blink(4);
	wait_for_click();

	phone_init();
	led_blink(5);
	wait_for_click();

	/* Enable interrupt */
	sei();

	if (phone_setup())
		led_blink(5);
	else
		led_blink(1);

	wait_for_click();

	for (;;) {
		if (wind->flag) {
			led_blink(1);
			do_media(wind);
			wind->flag = 0;
			sei();
		}

		if (phone_msg(message)) {
			led_blink(2);

			if (phone_valid_msg(message, "RING")) {
				led_blink(3);
				wait_for_click();

				phone_answer();
				led_blink(3);
				wait_for_click();
				
				/*
				sht11_read_all(temperature);
				*/
				led_blink(3);
				wait_for_click();
				
				synth_play_message(wind, temperature);
				led_blink(3);
				wait_for_click();
				
				phone_hangup();
			}
		}
	}

	free(wind);
	free(temperature);
	free(message);
}
