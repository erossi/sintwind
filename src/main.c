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
#include <avr/eeprom.h>
#include "default.h"
#include "init.h"
#include "synth.h"
#include "sht11.h"
#include "anemometer.h"
#include "media.h"
#include "cell.h"
#include "uart.h"
#include "utils.h"

/* EEPROM Area have to be global? */
uint8_t EEMEM EE_chkpoint;

/* Globals */
struct wind_array *wind;
volatile int loop;
struct uartStruct *uartPtr;

void delay1h(void)
{
	int i;

	for (i=0; i<360; i++) {
		led_blink(5);
		_delay_ms(10000);

		if (check_for_click())
			i = 360;
	}
}

int main(void)
{
	uint8_t chkpoint;
	struct sht11_t *temperature;
	char *message;

	loop = 0;
	wind = malloc(sizeof(struct wind_array));
	temperature = malloc(sizeof(struct sht11_t));
	message = malloc(UART_RXBUF_SIZE);

	/* initializing parts */
	port_init();
	array_init(wind);
	anemometer_init();
	sht11_init();
	phone_init(); /* activate uart comm only */
	sei(); /* Enable interrupt */

	/* Read eeprom checkpoint status */
	chkpoint = eeprom_read_byte(&EE_chkpoint);

	if (chkpoint)
		delay1h();
	else {
		chkpoint = 1;
		eeprom_write_byte(&EE_chkpoint, chkpoint);
	}

	/* if there is not enought juice we may crash here */
	while (chkpoint) {
		if (phone_on())
			delay1h(); /* error */
		else {
			chkpoint = 0;
			eeprom_write_byte(&EE_chkpoint, chkpoint);
		}
	}

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
				phone_answer();
				sht11_read_all(temperature);
				synth_play_message(wind, temperature);
				phone_hangup();
			}
		}

		if (check_for_click()) {
			led_blink(3);
			sht11_read_all(temperature);
			synth_play_message(wind, temperature);
		}
	}

	free(wind);
	free(temperature);
	free(message);
}
