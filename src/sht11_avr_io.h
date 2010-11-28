/* This file is part of sht11
 * Copyright (C) 2005-2010 Enrico Rossi
 * 
 * Sht11 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sht11 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SHT11_AVRIO_H
#define SHT11_AVRIO_H

/* clock delay in ms */
#define SHT11_SCK_DELAY 1

/* stuff that can be configured on default.h */
#ifndef SHT11_HAVE_DEFAULT
#define SHT11_DDR	DDRC
#define SHT11_PORT	PORTC
#define SHT11_PIN	PINC
#define SHT11_DATA	PC1
#define SHT11_SCK	PC0
#endif

void set_sck_high(void);
void set_sck_low(void);
void set_data_out(void);
void set_data_in(void);
void set_data_high(void);
void set_data_low(void);
void sck_delay(void);

#endif
