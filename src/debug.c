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

#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>
#include "default.h"
#include "uart.h" /* should we use somethink from cell ? */
#include "cell.h"
#include "utils.h"
#include "debug.h"

extern struct uartStruct *uartPtr;

void debug_write(const char *s)
{
	phone_send(s);
}

void debug_hello(void)
{
	debug_write("\n");
	debug_write("***********************************************");
	debug_write("*                                             *");
	debug_write("* Open Sint Project - Sint Wind vX.Y          *");
	debug_write("*               By                            *");
	debug_write("*                                             *");
	debug_write("* Andrea Marabini (info@marabo.it)            *");
	debug_write("* Enrico Rossi    (e.rossi@tecnobrain.com)    *");
	debug_write("*                                             *");
	debug_write("* http://tecnobrain.com/OpenSint/             *");
	debug_write("*            GNU GPL v3                       *");
	debug_write("*                                             *");
	debug_write("***********************************************");
	debug_write("\n");
}

int debug_phone_on(char *msg)
{
	int i;

	debug_write("Do you want to clean checkpoint EEPROM (yes/N)? ");

	while (!(phone_msg(msg)))
		led_blink(1);

	debug_write(msg);
	led_blink(2);

	if (phone_valid_msg(msg, "yes")) {
		i = 1;
		debug_write("Clearing EEPROM checkpoint\n");
	} else {
		debug_write("Leaving EEPROM boot error set!\n");
		i = 0;
	}

	return(i);
}

void debug_wind_status(struct wind_array *wind)
{
	debug_write("This is the wind status (not yet!)\r\n");
}

void debug_temperature(struct sht11_t *temp)
{
	char *string;

	string = malloc (20);

	string = ultoa (temp->raw_temperature, string, 10);
	debug_write (string);
	debug_write (" ");

	string = ultoa (temp->raw_humidity, string, 10);
	debug_write (string);
	debug_write (" ");

	string = dtostrf ((double)temp->temperature, 6, 3, string);
	debug_write (string);
	debug_write (" ");

	string = dtostrf ((double)temp->humidity_linear, 6, 3, string);
	debug_write (string);
	debug_write (" ");

	string = dtostrf ((double)temp->humidity_compensated, 6, 3, string);
	debug_write (string);
	debug_write (" ");

	string = dtostrf ((double)temp->dewpoint, 6, 3, string);
	debug_write (string);
	debug_write ("\r\n");

	_delay_ms (100);

	free (string);
}

void debug_synth(struct wind_array *wind, struct sht11_t *temp)
{
	debug_write("This is the Synth part unimplemented yet!\r\n");
}

