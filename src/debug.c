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
#include "uart.h"		/* should we use somethink from cell ? */
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
	debug_write("***********************************************\n");
	debug_write("*                                             *\n");
	debug_write("* Open Sint Project - Sint Wind vX.Y          *\n");
	debug_write("*               By                            *\n");
	debug_write("*                                             *\n");
	debug_write("* Andrea Marabini (info@marabo.it)            *\n");
	debug_write("* Enrico Rossi    (e.rossi@tecnobrain.com)    *\n");
	debug_write("*                                             *\n");
	debug_write("* http://tecnobrain.com/OpenSint/             *\n");
	debug_write("*            GNU GPL v3                       *\n");
	debug_write("*                                             *\n");
	debug_write("***********************************************\n");
	debug_write("\n");
}

int debug_phone_on(char *msg)
{
	int i;

	debug_write("Do you want to clean checkpoint EEPROM (yes/N)? ");

	while (!(phone_msg(msg)))
		led_blink(1);

	debug_write(msg);
	debug_write("\n");
	led_blink(2);

	if (phone_valid_msg(msg, "yes")) {
		i = 1;
		debug_write("Clearing EEPROM checkpoint\n");
	} else {
		debug_write("Leaving EEPROM boot error set!\n");
		i = 0;
	}

	return (i);
}

void debug_wind_status(struct wind_array *wind)
{
	debug_write("This is the wind status (not yet!)\n");
}

/* passing an already malloc string spare RAM */
void debug_temperature(struct sht11_t *temp, char *string)
{
	string = ultoa(temp->raw_temperature, string, 10);
	debug_write("Raw temperature value: ----------> ");
	debug_write(string);
	debug_write("\n");

	string = ultoa(temp->raw_humidity, string, 10);
	debug_write("Raw humidity value: -------------> ");
	debug_write(string);
	debug_write("\n");

	string = dtostrf((double)temp->temperature, 6, 3, string);
	debug_write("Temperature (°C): ---------------> ");
	debug_write(string);
	debug_write("\n");

	string = dtostrf((double)temp->humidity_linear, 6, 3, string);
	debug_write("Linear Humidity (%): ------------> ");
	debug_write(string);
	debug_write("\n");

	string = dtostrf((double)temp->humidity_compensated, 6, 3, string);
	debug_write("Temp-compensated Humidity (%): --> ");
	debug_write(string);
	debug_write("\n");

	string = dtostrf((double)temp->dewpoint, 6, 3, string);
	debug_write("Temperature dewpoint (°C): ------> ");
	debug_write(string);
	debug_write("\n");

	_delay_ms(1000);
}

void debug_synth(struct wind_array *wind, struct sht11_t *temp)
{
	debug_write("This is the Synth part unimplemented yet!\n");
}
