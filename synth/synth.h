/* This file is part of OpenSint
 * Copyright (C) 2005-2007 Enrico Rossi
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

#define SYNTH_ADDR PORTD
#define SYNTH_ADDR_DDR DDRD

#define SYNTH_CTRL_OUT PORTB
#define SYNTH_CTRL_IN  PINB
#define SYNTH_CTRL_DDR DDRB

#define EOM 1
#define PD 2
#define CE 3

void synth_init (void);
void synth_say_it (uint8_t position);
void synth_play_message (void);

#endif
