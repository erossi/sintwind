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
#include <avr/pgmspace.h>
#include "default.h"
#include "synth.h"
#include "cell.h"
#include "utils.h"
#include "debug.h"

extern struct uartStruct *uartPtr;

void debug_write(const char *s)
{
	phone_send(s);
}

void debug_write_P(PGM_P s)
{
	char *c;

	c = malloc (80);
	phone_send (strcpy_P (c, s));
	free (c);
}

void debug_hello(void)
{
	debug_write_P (PSTR("\n"));
	debug_write_P (PSTR("\n"));
	debug_write_P (PSTR("\n"));
	debug_write_P (PSTR("***********************************************\n"));
	debug_write_P (PSTR("*                                             *\n"));
	debug_write_P (PSTR("* Open Sint Project - Sint Wind v3.1          *\n"));
	debug_write_P (PSTR("*                                             *\n"));
	debug_write_P (PSTR("*               By                            *\n"));
	debug_write_P (PSTR("*                                             *\n"));
	debug_write_P (PSTR("* Andrea Marabini (info@marabo.it)            *\n"));
	debug_write_P (PSTR("* Enrico Rossi    (e.rossi@tecnobrain.com)    *\n"));
	debug_write_P (PSTR("*                                             *\n"));
	debug_write_P (PSTR("* http://tecnobrain.com/OpenSint/             *\n"));
	debug_write_P (PSTR("*                                             *\n"));
	debug_write_P (PSTR("*            GNU GPL v3                       *\n"));
	debug_write_P (PSTR("***********************************************\n"));
	debug_write_P (PSTR("\n"));
}

int debug_phone_on(char *msg)
{
	int i;

	debug_write_P (PSTR("Do you want to clean checkpoint EEPROM (yes/NO)? "));

	while (!(phone_msg(msg)))
		led_blink(1);

	debug_write(msg);
	debug_write("\n");
	led_blink(2);

	if (phone_valid_msg(msg, "yes")) {
		i = 1;
		debug_write_P (PSTR("Clearing EEPROM checkpoint\n"));
	} else {
		debug_write_P (PSTR("OK\n"));
		i = 0;
	}

	return (i);
}

void debug_lacrosse(struct wind_array *wind, char *string)
{
	debug_write_P (PSTR("hd [ "));
	string = itoa(wind->lacrosse_head, string, 2);
	debug_write(string);
	debug_write_P (PSTR(" ] br [ "));
	string = itoa(wind->lacrosse_bearing, string, 10);
	debug_write(string);
	debug_write_P (PSTR(" ] spd [ "));
	string = itoa(wind->lacrosse_speed, string, 10);
	debug_write(string);
	debug_write_P (PSTR(" ] nbr [ "));
	string = itoa(wind->lacrosse_nbearing, string, 2);
	debug_write(string);
	debug_write_P (PSTR(" ] nspd [ "));
	string = itoa(wind->lacrosse_nspeed, string, 10);
	debug_write(string);
	debug_write_P (PSTR(" ]\n"));

	debug_write_P (PSTR("chksum [ "));
	string = itoa(wind->lacrosse_chksum, string, 2);
	debug_write(string);
	debug_write_P (PSTR(" ] chkOK [ "));
	string = itoa(wind->lacrosse_chkok, string, 2);
	debug_write(string);
	debug_write_P (PSTR(" ]\n"));
}

void debug_wind_status(struct wind_array *wind, char *string)
{
	debug_write_P (PSTR("S.rt [ "));
	string = itoa(wind->speed_rt, string, 10);
	debug_write(string);
	debug_write_P (PSTR(" ] A.rt [ "));
	string = itoa(wind->angle_rt, string, 10);
	debug_write(string);
	debug_write_P (PSTR(" ] V.min [ "));
	string = itoa(wind->vmin, string, 10);
	debug_write(string);
	debug_write_P (PSTR(" ] V.max [ "));
	string = itoa(wind->vmax, string, 10);
	debug_write(string);
	debug_write_P (PSTR(" ] V [ "));
	string = itoa(wind->speed, string, 10);
	debug_write(string);
	debug_write_P (PSTR(" ] Deg. [ "));
	string = itoa(wind->angle, string, 10);
	debug_write(string);
	debug_write_P (PSTR(" ]\n"));

	if (!wind->sensor)
		debug_lacrosse(wind, string);
}

/* passing an already malloc string spare RAM */
void debug_temperature(struct sht11_t *temp, char *string)
{
	string = ultoa(temp->raw_temperature, string, 10);
	debug_write_P (PSTR("Raw temperature value: ----------> "));
	debug_write(string);
	debug_write("\n");

	string = ultoa(temp->raw_humidity, string, 10);
	debug_write_P (PSTR("Raw humidity value: -------------> "));
	debug_write(string);
	debug_write("\n");

	string = dtostrf((double)temp->temperature, 6, 3, string);
	debug_write_P (PSTR("Temperature (C): ----------------> "));
	debug_write(string);
	debug_write("\n");

	string = dtostrf((double)temp->humidity_linear, 6, 3, string);
	debug_write_P (PSTR("Linear Humidity (%): ------------> "));
	debug_write(string);
	debug_write("\n");

	string = dtostrf((double)temp->humidity_compensated, 6, 3, string);
	debug_write_P (PSTR("Temp-compensated Humidity (%): --> "));
	debug_write(string);
	debug_write("\n");

	string = dtostrf((double)temp->dewpoint, 6, 3, string);
	debug_write_P (PSTR("Temperature dewpoint (C): -------> "));
	debug_write(string);
	debug_write("\n");

	_delay_ms(1000);
}

void debug_synth(struct wind_array *wind, struct sht11_t *temp, char *msg)
{
	debug_write_P (PSTR("Do you want to synth data? (yes/NO) "));

	while (!(phone_msg(msg)))
		led_blink(1);

	debug_write(msg);
	debug_write("\n");

	if (phone_valid_msg(msg, "yes")) {
		debug_write_P (PSTR("Ok. Playing data...\n"));
		synth_play_message(wind, temp);
		debug_write_P (PSTR("Done!\n"));
	}

	debug_write_P (PSTR("\n"));
	debug_write_P (PSTR("Do you want to play all synth memory? (yes/NO) "));

	while (!(phone_msg(msg)))
		led_blink(1);

	debug_write(msg);
	debug_write("\n");

	if (phone_valid_msg(msg, "yes")) {
		debug_write_P (PSTR("Ok. Playing all synth memory...\n"));
		synth_play_memory();
		debug_write_P (PSTR("Done!\n"));
	}

	debug_write_P (PSTR("HangUp...\n"));
}

void debug_which_sensor(struct wind_array *wind)
{
	/* Which sensor are we using */
	if (!wind->sensor)
		debug_write_P(PSTR("Sensor set to: LaCrosse.\n"));
	else
		debug_write_P(PSTR("Sensor set to: Davis.\n"));
}

void debug_sensor(struct wind_array *wind, char *msg)
{
	debug_which_sensor(wind);
	debug_write_P (PSTR("Do you want to change it? (yes/[no]) "));

	while (!(phone_msg(msg)))
		led_blink(1);

	debug_write(msg);
	debug_write("\n");

	if (phone_valid_msg(msg, "yes")) {
		debug_write_P (PSTR("YOU MUST RESET!!!\n"));

		if (!wind->sensor)
			wind->sensor = 1;
		else
			wind->sensor = 0;
	}

	debug_which_sensor(wind);
}

void debug_help(void)
{
	debug_write_P (PSTR("\n"));
	debug_write_P (PSTR("Help:\n"));
	debug_write_P (PSTR(" RING   Start phone answer and playback\n"));
	debug_write_P (PSTR(" sensor Change sensor(MUST RESET)\n"));
	debug_write_P (PSTR("\n"));
	debug_write_P (PSTR("S.rt = Speed in real time\n"));
	debug_write_P (PSTR("A.rt = Angle in real time\n"));
	debug_write_P (PSTR("V.min = Minimum speed in the last cycle\n"));
	debug_write_P (PSTR("V.max = Maximum speed in the last cycle\n"));
	debug_write_P (PSTR("V = Medium speed in the last cycle\n"));
	debug_write_P (PSTR("Deg. = Medium angle in degrees in the last cycle\n"));
	debug_write_P (PSTR("\n"));
}
