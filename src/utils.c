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
#include "utils.h"

void led_blink(int num)
{
	while (num) {
		UTILS_LED_PORT |= _BV(UTILS_LED_PIN);
		_delay_ms(300);
		UTILS_LED_PORT &= ~_BV(UTILS_LED_PIN);
		_delay_ms(300);
		num--;
	}
}

void wait_for_click(void)
{
	loop_until_bit_is_clear(UTILS_SWITCH_PORT, UTILS_SWITCH_PIN);
	led_blink(1);
	_delay_ms(1000);
}

int check_for_click(void)
{
	if (bit_is_clear(UTILS_SWITCH_PORT, UTILS_SWITCH_PIN)) {
		led_blink(1);
		_delay_ms(1000);
		return (1);
	} else
		return (0);
}

void delay1h(void)
{
	int i;

	for (i = 0; i < 360; i++) {
		led_blink(5);
		_delay_ms(10000);

		if (check_for_click())
			i = 360;
	}
}
