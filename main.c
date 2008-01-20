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

/* #include <avr/interrupt.h> */
#include <avr/io.h>
#include "default.h"
#include "sensor.h"
#include "uart.h"

/* #include "adc.h" */
#include <util/delay.h>

uint8_t *adc_area;
uint16_t adc_index;
uint8_t adc_runover;

int
main(void)
{
/*   uint16_t i = 0; */
/*   uint16_t adc_size = 1 << ADC_CUTOFF; */
  int result;
  char *string;

  DDRB = 255;
  PORTB = 0xff;
  DDRA = 0;
  string = malloc (10);

  sensor_init ();
  uart_init();

/*   adc_init (); */

/*   adc_area = malloc (adc_size); */
/*   adc_index = 0; */
/*   adc_runover = 0; */
/*   sei (); */

/*   adc_start (); */

  for (;;)
    {
      loop_until_bit_is_clear (PINA, 0);
      PORTB = 0;
      _delay_ms (10);
      PORTB = 0xff;

      result = (int)sensor_read_temperature ();
      if (result != -999)
	{
	  itoa (result, string, 10);
	  uart_printstr (string);
	  uart_putchar ('\n');
	}

      _delay_ms (10);

      result = (int)sensor_read_humidity ();
      if (result != -999)
	{
	  itoa (result, string, 10);
	  uart_printstr (string);
	  uart_putchar ('\n');
	}

      _delay_ms (10);
    }

  free (adc_area);
}
