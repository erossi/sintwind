/* This file is part of OpenSint
 * Copyright (C) 2005-2011 Enrico Rossi
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

/*! \file adc.c
 * \brief Analog to digital converter for davids wind direction.
 */

#include <stdint.h>
#include <avr/io.h>
#include "adc.h"

/*! Enable ADC. */
void adc_init(void)
{
	/*! select ADC clock = F_CPU / 128, manual sample. */
	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);

	/*! Set internal VREF, left adjusted result, adc0 */
	ADMUX = _BV(REFS0) | _BV(ADLAR);
}

/*! \brief Read the ADC value.
 * \return 8 bit value.
 */
uint8_t adc_read(void)
{
	/* Start the conversion */
	ADCSRA |= _BV(ADSC);

	/* Wait until the conversion is ended */
	loop_until_bit_is_clear(ADCSRA, ADSC);

	/* Return 8-bit conversion's value */
	return (ADCH);
}

/*! \brief Convert between 0-255 to 0-360 degrees */
int adc_wind_position(void)
{
	int i;

	i = adc_read() * ADC_RATIO;
	if (i > 359)
		i = 359;

	return (i);
}
