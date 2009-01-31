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

/* Globals */
struct wind_array *wind;
volatile int loop;
struct uartStruct *uartPtr;

void led_blink(int num)
{
	while (num) {
		LED_PORT |= _BV(LED_P);
		_delay_ms(500);
		LED_PORT &= ~_BV(LED_P);
		_delay_ms(500);
		num--;
	}
}

void wait_for_click(void)
{
	loop_until_bit_is_set(MANUAL_PORT, MANUAL_P);
	led_blink(1);
	_delay_ms(1000);
}

int main(void)
{
	struct sht11_t *temperature;
	char *message;

	loop = 0;
	wind = malloc(sizeof(struct wind_array));
	temperature = malloc(sizeof(struct sht11_t));
	message = malloc(UART_RXBUF_SIZE);

	/*
	 * initializing parts
	 */

	port_init();
	led_blink(7);
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

	for (;;) {
		if (wind->flag) {
			led_blink(2);
			wind->flag = 0;
			sei();
		}

		if (phone_msg(message)) {
			led_blink(3);

			if (phone_valid_msg(message, "RING")) {
				phone_answer();
				sht11_read_all(temperature);
				synth_play_message(wind, temperature);
				phone_hangup();
			}
		}
	}

	free(wind);
	free(temperature);
	free(message);
}
