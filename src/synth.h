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

#ifndef _SYNTH_H_
#define _SYNTH_H_

/*
 * I/O ports are shared with others, definition and
 * initialization are in main program
 *
#define _SYNTH_ADDR PORTD
#define _SYNTH_ADDR_DDR DDRD

#define _SYNTH_CTRL_OUT PORTB
#define _SYNTH_CTRL_IN  PINB

#define _SYNTH_CTRL_DDR DDRB
*/

#define _SYNTH_EOM 1
#define _SYNTH_PD 2
#define _SYNTH_CE 3

/* club di volo */
#define _SYNTH_S_CLUB 112

/* vento prov. da */
#define _SYNTH_S_WIND 96

/* XXX */
#define _SYNTH_S_NORTH 82
#define _SYNTH_S_EAST 76
#define _SYNTH_S_SOUTH 78
#define _SYNTH_S_WEST 80

/* con una intensita' da */
#define _SYNTH_S_INTENSITY 108

/* YYY */
#define _SYNTH_S_ABSENT 74

/* a */
#define _SYNTH_S_TO 86
 
/* XXX */

/* km/h */
#define _SYNTH_S_KMH 84

/* PAUSA */

/* temperatura */
#define _SYNTH_S_TEMP 88

/* [meno] XXX */
#define _SYNTH_S_MINUS 90

/* virgola */
#define _SYNTH_S_COMMA 92

/* YYY */

/* gradi centigradi */
#define _SYNTH_S_CELSIUS 99

/* PAUSA */

/* umidita' relativa */
#define _SYNTH_S_UMIDITY 102

/* XXX */

/* percento */
#define _SYNTH_S_PERC 94

/* data not avaiable */
#define _SYNTH_S_DNA 105

void synth_init (void);
void synth_play_message (struct wind_array *wind);

/* Use under test only */
void say_it (uint8_t position);
void say_int (int value);
void synth_pause (void);
void synth_say_wind_direction (enum wind_dir direction);

#endif
