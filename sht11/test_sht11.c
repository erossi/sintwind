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

#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include "default.h"
#include "sht11.h"
#include "uart.h"
#include <util/delay.h>

int
main(void)
{
  char *string;
  struct sht11_t dataset;

  DDRB = 255;
  PORTB = 0xff;
  DDRA = 0;
  string = malloc (20);

  sht11_init ();
  uart_init();

  for (;;)
    {
/*       wait until a button is pressed */
      loop_until_bit_is_clear (PINA, 0);

/*       flash led connected to PORTB */
      PORTB = 0;
      _delay_ms (10);
      PORTB = 0xff;

      sht11_read_all (&dataset);

      string = ultoa (dataset.raw_temperature, string, 10);
      uart_printstr (string);
      uart_putchar (' ');

      string = ultoa (dataset.raw_humidity, string, 10);
      uart_printstr (string);
      uart_putchar (' ');

      string = dtostrf ((double)dataset.temperature, 6, 3, string);
      uart_printstr (string);
      uart_putchar (' ');

      string = dtostrf ((double)dataset.humidity_linear, 6, 3, string);
      uart_printstr (string);
      uart_putchar (' ');

      string = dtostrf ((double)dataset.humidity_compensated, 6, 3, string);
      uart_printstr (string);
      uart_putchar (' ');

      string = dtostrf ((double)dataset.dewpoint, 6, 3, string);
      uart_printstr (string);
      uart_putchar ('\n');

      _delay_ms (10);
    }
}
