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

#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include "adc.h"

int
main (void)
{
/* Out */
  DDRD = 0xff;

/* In */
  DDRB = 0;

  adc_init ();

  for (;;)
    {
      PORTD = 0xff;
      loop_until_bit_is_clear (PINB, 0);

      PORTD = 0;
      loop_until_bit_is_clear (PINB, 1);

      PORTD = adc_read ();
      loop_until_bit_is_clear (PINB, 0);

    }
}
