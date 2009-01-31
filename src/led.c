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

#include <avr/io.h>
#include <util/delay.h>
#include "default.h"
#include "led.h"

void led_blink(int num)
{
	while (num) {
		LED_PORT |= _BV(LED_P);
		_delay_ms(500);
		LED_PORT &= ~_BV(LED_P);
		_delay_ms(500);
		num--;
	}
}

void wait_for_click(void)
{
	loop_until_bit_is_clear(LED_MANUAL_PORT, LED_MANUAL_P);
	led_blink(1);
	_delay_ms(1000);
}
