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

#ifndef DEBUG_H_
#define DEBUG_H_

void debug_write(const char *s);
void debug_write_P(PGM_P s);
void debug_hello(void);
int debug_phone_on(char *msg);
void debug_wind_status(struct wind_array *wind, char *string);
void debug_temperature(struct sht11_t *temp, char *string);
void debug_synth(struct wind_array *wind, struct sht11_t *temp, char *msg);
void debug_which_sensor(struct wind_array *wind);
void debug_sensor(struct wind_array *wind, char *msg);
void debug_help(void);

#endif
