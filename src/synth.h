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

#ifndef SYNTH_H_
#define SYNTH_H_

/* club di volo */
#define SYNTH_S_CLUB 112

/* vento prov. da */
#define SYNTH_S_WIND 96

/* XXX */
#define SYNTH_S_NORTH 82
#define SYNTH_S_EAST 76
#define SYNTH_S_SOUTH 78
#define SYNTH_S_WEST 80

/* con una intensita' da */
#define SYNTH_S_INTENSITY 108

/* YYY */
#define SYNTH_S_ABSENT 74

/* a */
#define SYNTH_S_TO 86

/* XXX */

/* km/h */
#define SYNTH_S_KMH 84

/* PAUSA */

/* temperatura */
#define SYNTH_S_TEMP 88

/* [meno] XXX */
#define SYNTH_S_MINUS 90

/* virgola */
#define SYNTH_S_COMMA 92

/* YYY */

/* gradi centigradi */
#define SYNTH_S_CELSIUS 99

/* PAUSA */

/* umidita' relativa */
#define SYNTH_S_UMIDITY 102

/* XXX */

/* percento */
#define SYNTH_S_PERC 94

/* data not avaiable */
#define SYNTH_S_DNA 105

void synth_init(void);
void synth_play_message(struct wind_array *wind, struct sht11_t *temp);

/* Use under test only */
void say_it(uint8_t position);
void say_int(int value);
void synth_say_wind_direction(enum wind_dir direction);

#endif
