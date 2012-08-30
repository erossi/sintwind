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
#include <util/crc16.h>
#include <util/delay.h>

#ifdef HAVE_DEFAULT
#include "default.h"
#endif

#include "sht11_io.h"

/* clock delay in ms */
#define SHT11_SCK_DELAY 1

/* stuff that can be configured on default.h */
#ifndef SHT11_HAVE_DEFAULT
#define SHT11_DDR	DDRB
#define SHT11_PORT	PORTB
#define SHT11_PIN	PINB
#define SHT11_DATA	PB1
#define SHT11_SCK	PB2
#endif

void set_sck_out(void)
{
	SHT11_DDR |= (1<<SHT11_SCK);
}

void set_sck_in(void)
{
	SHT11_DDR &= ~(1<<SHT11_SCK);
}

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

/* remember this return 0 or n, not 0 or 1 */
uint8_t read_data_pin(void)
{
	return(SHT11_PIN & (1<<SHT11_DATA));
}

void sck_delay(void)
{
	_delay_ms(SHT11_SCK_DELAY);
}

uint8_t wait_until_data_is_ready(void)
{
	/* And if nothing came back this code hangs here */
	loop_until_bit_is_set(SHT11_PIN, SHT11_DATA);
	loop_until_bit_is_clear(SHT11_PIN, SHT11_DATA);
	return(0);
}

/*
   sensirion has implemented the CRC the wrong way round. We
   need to swap everything.
   bit-swap a byte (bit7->bit0, bit6->bit1 ...)
   code provided by Guido Socher http://www.tuxgraphics.org/
 */
uint8_t bitswapbyte(uint8_t byte)
{
	uint8_t i=8;
	uint8_t result=0;
	while(i) {
		result=(result<<1);

		if (1 & byte) {
			result=result | 1;
		}

		i--;
		byte=(byte>>1);
	}

	return(result);
}

uint8_t sht11_crc8(uint8_t crc, uint8_t data)
{
	return(_crc_ibutton_update(crc, bitswapbyte(data)));
}

int sht11_io_init(void)
{
	/* sht11 clk pin to output and set high */
	set_sck_out();
	set_sck_high();
	return(0); /* for compatibility reason */
}

/* only for compatibile reason */
void sht11_io_end(void)
{
	set_sck_in();
	set_data_in();
}

