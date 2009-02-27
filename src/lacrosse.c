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

#include <inttypes.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "default.h"
#include "lacrosse.h"

/*
   The serial protocol is as follows-
   a.. Approx 1.2mS bit time >> 833 bps
   b.. inverted data - 0V = logic 1, +3.3V = logic 0
   c.. one long (20 bit) stream with start frame and end checksum
   d.. issued every 2 seconds while /CE is true (held at 0V)
   e.. 00100bbbbvvvvvvvvvvvvcccc
   f.. 00100 start frame
   g.. hhhh bearing, lsb first, 0-15 cardinal points from North, clockwise
   rotation, 0=N, 1=NNE, etc
   h.. vvvvvvvvvvvv velocity, lsb first, 0-4096, turbine counts over previous 2
   second period, non accumulating
   i.. velocity count appears somewhat nonlinear- the turbine is quite sensitive
   to small gusts below 5 knots that would not move a conventional cup anemometer,
   but less so at higher speeds
   j.. knots = velocity/8 seems to be a reasonable approximation across typical
   wind speed ranges
   k.. cccc checksum, appers to be simple XOR of the 4 bbbb, vvvv,vvvv,vvvv
   nibbles
 */

/* Global variable and pointer to be used */
/* inside the ISR routine */

extern struct wind_array *wind;

ISR(INT0_vect)
{
	uint8_t i;

	/* Maybe neccessary ~500us delay to read bits not near up/down front */
	_delay_us(500);

	/* header */
	wind->lacrosse_head = 0;

	for (i=0; i<5; i++) {
		if (LACROSSE_RX_PORT & _BV(LACROSSE_RX))
				wind->lacrosse_head |= _BV(i);

		_delay_us(LACROSSE_RX_DELAY);
	}
	
	/* bearing */
	wind->lacrosse_bearing = 0;

	for (i=0; i<4; i++) {
		if (LACROSSE_RX_PORT & _BV(LACROSSE_RX))
				wind->lacrosse_bearing |= _BV(i);

		_delay_us(LACROSSE_RX_DELAY);
	}

	/* speed */
	wind->lacrosse_speed = 0;
	
	for (i=0; i<12; i++) {
		if (LACROSSE_RX_PORT & _BV(LACROSSE_RX))
				wind->lacrosse_speed |= _BV(i);

		_delay_us(LACROSSE_RX_DELAY);
	}

	/* checksum */
	wind->lacrosse_chksum = 0;
	
	for (i=0; i<4; i++) {
		if (LACROSSE_RX_PORT & _BV(LACROSSE_RX))
				wind->lacrosse_chksum |= _BV(i);

		_delay_us(LACROSSE_RX_DELAY);
	}

	/* NOT bearing */
	wind->lacrosse_nbearing = 0;

	for (i=0; i<4; i++) {
		if (LACROSSE_RX_PORT & _BV(LACROSSE_RX))
				wind->lacrosse_nbearing |= _BV(i);

		_delay_us(LACROSSE_RX_DELAY);
	}

	/* NOT speed */
	wind->lacrosse_nspeed = 0;
	
	for (i=0; i<12; i++) {
		if (LACROSSE_RX_PORT & _BV(LACROSSE_RX))
				wind->lacrosse_nspeed |= _BV(i);

		_delay_us(LACROSSE_RX_DELAY);
	}

	wind->flag = 1;
}

void lacrosse_stop(void)
{
	/* Clear \CE */
	LACROSSE_CE_PORT &= ~(_BV(LACROSSE_CE));
}

void lacrosse_start(void)
{
	/* Set \CE */
	LACROSSE_CE_PORT |= _BV(LACROSSE_CE);
}

void lacrosse_init(void)
{
	/* datasheet pag67 on INT0 register setup */
	/* INT0 low level only generates int. */
	MCUCR &= ~(_BV(ISC01) | _BV(ISC00));

	/* enable INT0 */
	GICR |= _BV(INT0);

	lacrosse_start();
}

void lacrosse_shutdown(void)
{
	lacrosse_stop();

	/* disable INT0 */
	GICR &= ~(_BV(INT0));
}

/* check if header is 00100 */
uint8_t header_ok(void)
{
	if (wind->lacrosse_head == 4)
		return (1);
	else
		return (0);
}

uint8_t lacrosse_is_connected(void)
{
	uint8_t i, j;

	j = 0;
	lacrosse_init();
	sei();

	for (i=0; i<50; i++)
		if (wind->flag) {
			j = header_ok();
			i = 100;
		} else {
			_delay_ms(200);
		}

	cli();
	lacrosse_shutdown();
	return(j);
}

/* Extract and store correct value from lacrosse stream */
uint8_t lacrosse_adjust(void)
{
	if (header_ok()) {
		wind->angle_rt = wind->lacrosse_bearing * 22.5;
		wind->speed_rt = wind->lacrosse_speed * LACROSSE_RATIO;
		wind->lacrosse_chkok = wind->lacrosse_bearing;
		wind->lacrosse_chkok ^= (wind->lacrosse_speed & 0x0f);
		wind->lacrosse_chkok ^= ((wind->lacrosse_speed >> 4) & 0x0f);
		wind->lacrosse_chkok ^= ((wind->lacrosse_speed >> 8) & 0x0f);

		if (wind->lacrosse_chkok == wind->lacrosse_chksum)
			return(1);
	}

	return(0);
}
