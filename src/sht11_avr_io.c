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

#include <avr/io.h>
#include <util/delay.h>
#include "sht11_avr_io.h"

void set_sck_high(void)
{
	SHT11_PORT |= (1<<SHT11_SCK);
}

void set_sck_low(void)
{
	SHT11_PORT &= ~(1<<SHT11_SCK);
}

void set_data_out(void)
{
	SHT11_DDR |= (1<<SHT11_SCK) | (1<<SHT11_DATA);
}

void set_data_in(void)
{
	SHT11_DDR |= (1<<SHT11_SCK);
	SHT11_DDR &= ~(1<<SHT11_DATA);
}

void set_data_high(void)
{
	/* release the data pin, pullup do the rest */
	/*   SHT11_PORT |= (1<<SHT11_DATA); */
	set_data_in();
}

void set_data_low(void)
{
	SHT11_PORT &= ~(1<<SHT11_DATA);
	set_data_out();
}

void sck_delay(void)
{
	_delay_ms(SHT11_SCK_DELAY);
}
