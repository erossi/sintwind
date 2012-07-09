/* This file is part of OpenSint
 * Copyright (C) 2005-2012 Enrico Rossi
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

/*! \file lacrosse.c
 * \brief larosse tx20 interface.
 */

#include <inttypes.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lacrosse.h"

/*!
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

/*! \brief check if header is 00100 */
uint8_t header_ok(void) {
	if (wind->lacrosse_head == 4)
		return (TRUE);
	else
		return (FALSE);
}

/*! Check the checksum of the message.
 * \return true or false.
 */
uint8_t lacrosse_checksum(void) {
	wind->lacrosse_chkok = wind->lacrosse_bearing;
	wind->lacrosse_chkok ^= (wind->lacrosse_speed & 0x0f);
	wind->lacrosse_chkok ^= ((wind->lacrosse_speed >> 4) & 0x0f);
	wind->lacrosse_chkok ^= ((wind->lacrosse_speed >> 8) & 0x0f);

	if (wind->lacrosse_chkok == wind->lacrosse_chksum)
		return(TRUE);
	else
		return(FALSE);
}

/*! The interrupt function to get the lacrosse string.
 *
 * \bug if the message is non-valid and there is no more
 * reading left to be used, then the message is considered valid
 * anyway.
 */
ISR(INT0_vect) {
	uint8_t i;

	/*! \note Maybe neccessary ~500us delay to read bits
	 * not near up/down front.
	 */
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

	/* if the header is ok, */
	if (header_ok()) {
		/* if the checksum is ok or we can not read more
		 * messages.
		 */
		if (lacrosse_checksum() || (!wind->lacrosse_loop--)) {
			lacrosse_stop();
			wind->flag = 1;
		}
	}
}

/*! \brief stop the lacrosse reading.
 */
void lacrosse_stop(void) {
	/* Clear \CE */
	LACROSSE_CE_PORT &= ~(_BV(LACROSSE_CE));
}

/* \brief start the lacrosse reading.
 *
 * Restart the lacrosse loop counter and reset any reading
 * present in the struct.
 *
 * \note don't trust any present value in the wind structure.
 */
void lacrosse_start(void) {
	wind->flag = 0;
	wind->lacrosse_loop = LACROSSE_LOOP; /* reset the counter */

	/* Set \CE */
	LACROSSE_CE_PORT |= _BV(LACROSSE_CE);
}

/* \brief initialize the lacrosse anemometer.
 *
 * Setup the interrupt register to activate the ISR function
 * when the INT0 pin is low.
 *
 * \note the interrupt keep being generated until the pin is
 * released, which should happen when the data is received.
 */
void lacrosse_init(void)
{
	/* INT0 ISR on low pin. This should be the default. */
	EICRA &= ~(_BV(ISC01) | _BV(ISC00));

	/* enable INT0 */
	EIMSK |= _BV(INT0);

	lacrosse_start();
}

/*! \brief shutdown lacrosse operations.
 * \note the default EICRA settings no need to change.
 */
void lacrosse_shutdown(void)
{
	lacrosse_stop();

	/* disable INT0 */
	EIMSK &= ~_BV(INT0);
}

/*! \brief check if the lacrosse is connected.
 *
 * This function should be used to try to autosetup which
 * anemometer is present.
 *
 * \todo Remove this functions, when happens the lacrosse isn't
 * detected correctly at the startup, the sint remains without
 * anemometer until someone reset it.
 * \important never use this routine with interrupt enable.
 * \bug _delay_ms not working properly, this function should be
 * wait 15 sec, if wind->flag is present then a lacrosse is
 * connected.
 */
uint8_t lacrosse_is_connected(void)
{
	uint8_t i;

	lacrosse_init();
	sei();

	for (i=0; i<50; i++)
		if (wind->flag)
			i=99;
		else
			_delay_ms(300);

	cli();
	lacrosse_shutdown();

	if (i == 100)
		return(TRUE);
	else
		return(FALSE);
}

/*! \brief correct the lacrosse readings.
 * \bug return WHAT?!?
 */
uint8_t lacrosse_adjust(void) {
	wind->angle_rt = wind->lacrosse_bearing * 22.5;
	wind->speed_rt = wind->lacrosse_speed * LACROSSE_RATIO;
	return(0);
}
