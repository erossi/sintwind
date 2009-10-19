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
#include <avr/pgmspace.h>
#include "default.h"
#include "init.h"
#include "synth.h"
#include "sht11.h"
#include "anemometer.h"
#include "media.h"
#include "cell.h"
#include "uart.h"
#include "utils.h"
#include "debug.h"

/* EEPROM Area really have to be global? */
uint8_t EEMEM EE_chkpoint;
uint8_t EEMEM EE_sensor; /* Lacrosse = 1, default Davis */

/* Globals */
struct wind_array *wind;
volatile int loop;
struct uartStruct *uartPtr;

/* Better 2 main with and without debug since only one will run */
void run_with_debug(struct sht11_t *temperature, char *message)
{
	uint8_t chkpoint;

	debug_hello();

	/* Read eeprom checkpoint status (last boot) */
	chkpoint = eeprom_read_byte(&EE_chkpoint);

	/* which sensor are we using? */
	debug_which_sensor(wind);

	/* If checkpoint then last boot went wrong */
	if (chkpoint)
		debug_write_P(PSTR("Last power up went wrong!\n"));
	else {
		debug_write_P(PSTR ("Setting up EEPROM checkpoint\n"));
		chkpoint = 1;
		eeprom_write_byte(&EE_chkpoint, chkpoint);
	}

	/* Keep the checkpoint set or clear in the eeprom */
	if (debug_phone_on(message)) {
		chkpoint = 0;
		eeprom_write_byte(&EE_chkpoint, chkpoint);
	}

	for (;;) {
		if (wind->flag) {
			led_blink(1);
			/* passing message to avoid malloc */
			debug_wind_status(wind, message);
			media(wind);
		}

		if (phone_msg(message)) {
			led_blink(2);

			if (phone_valid_msg(message, "RING")) {
				phone_answer();
				sht11_read_all(temperature);
				/* passing message to avoid malloc */
				debug_temperature(temperature, message);
				debug_synth(wind, temperature, message);
				phone_hangup();
			}

			if (phone_valid_msg(message, "sensor")) {
				/* passing message to avoid malloc */
				debug_sensor(wind, message);
				eeprom_write_byte(&EE_sensor, wind->sensor);
			}

			if (phone_valid_msg(message, "help")) {
				/* passing message to avoid malloc */
				debug_help();
			}
		}
	}
}

void run_free(struct sht11_t *temperature, char *message)
{
	uint8_t chkpoint;

	/* Read eeprom checkpoint status (last boot) */
	chkpoint = eeprom_read_byte(&EE_chkpoint);

	/* If checkpoint then last boot went wrong */
	if (chkpoint)
		delay1h();
	else {
		chkpoint = 1;
		eeprom_write_byte(&EE_chkpoint, chkpoint);
	}

	/* without enought power we may die here */
	while (chkpoint) {
		if (phone_on())
			delay1h();	/* error */
		else {
			chkpoint = 0;
			eeprom_write_byte(&EE_chkpoint, chkpoint);
		}
	}

	for (;;) {
		if (wind->flag) {
			led_blink(1);
			media(wind);
		}

		if (phone_msg(message)) {
			led_blink(2);

			if (phone_valid_msg(message, "RING")) {
				phone_answer();
				cli(); /* disable interrupt */
				sht11_read_all(temperature);
				synth_play_message(wind, temperature);
				sei(); /* re-enable interrupt */
				phone_hangup();
			}
		}

		if (check_for_click()) {
			led_blink(3);
			sht11_read_all(temperature);
			synth_play_message(wind, temperature);
		}
	}
}

int main(void)
{
	int debug;
	struct sht11_t *temperature;
	char *message;

	/* Init globals */
	loop = 0;
	wind = malloc(sizeof(struct wind_array));

	/* Init locals */
	temperature = malloc(sizeof(struct sht11_t));
	message = malloc(UART_RXBUF_SIZE);

	/* reading sensor to be used */
	wind->sensor = eeprom_read_byte(&EE_sensor);

	/* initializing parts */
	port_init();
	/* 1 sec if click, else no delay */
	debug = check_for_click();
	array_init(wind);
	/* if lacrosse is not present, the check will take 10 sec */
	anemometer_init(wind);
	sht11_init();
	phone_init();		/* activate uart comm only, no delay */
	sei();			/* Enable interrupt */

	if (debug)
		run_with_debug(temperature, message);
	else
		run_free(temperature, message);

	free(wind);
	free(temperature);
	free(message);
	return (0);
}
