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

#include <stdint.h>
#include <math.h>
#include <avr/io.h>
#include "default.h"
#include "sht11.h"
#include <util/delay.h>

void set_sck_high(void)
{
	SHT11_PORT |= _BV(SHT11_SCK);
}

void set_sck_low(void)
{
	SHT11_PORT &= ~_BV(SHT11_SCK);
}

void set_data_out(void)
{
	SHT11_DDR |= _BV(SHT11_SCK) | _BV(SHT11_DATA);
}

void set_data_in(void)
{
	SHT11_DDR |= _BV(SHT11_SCK);
	SHT11_DDR &= ~_BV(SHT11_DATA);
}

void set_data_high(void)
{
	/* release the data pin, pullup do the rest */
	/*   SHT11_PORT |= _BV(SHT11_DATA); */
	set_data_in();
}

void set_data_low(void)
{
	SHT11_PORT &= ~_BV(SHT11_DATA);
	set_data_out();
}

void send_byte(uint8_t byte)
{
	uint8_t i;

	i = 8;

	while (i) {
		--i;

		if (byte & _BV(i))
			set_data_high();
		else
			set_data_low();
		_delay_ms(SHT11_SCK_DELAY);

		set_sck_high();
		_delay_ms(SHT11_SCK_DELAY);

		set_sck_low();
	}
}

uint8_t read_byte(void)
{
	uint8_t i, bit, result;

	result = 0;
	i = 8;

	while (i) {
		--i;
		_delay_ms(SHT11_SCK_DELAY);
		set_sck_high();
		bit = SHT11_PIN & _BV(SHT11_DATA);
		_delay_ms(SHT11_SCK_DELAY);
		set_sck_low();

		if (bit)
			result |= _BV(i);
	}

	return (result);
}

void send_ack(void)
{
	/* Send ack */
	set_data_low();
	_delay_ms(SHT11_SCK_DELAY);
	set_sck_high();
	_delay_ms(SHT11_SCK_DELAY);
	set_sck_low();
	set_data_in();
}

uint8_t read_ack(void)
{
	uint8_t ack;

	/* read ack after command */
	set_data_in();
	_delay_ms(SHT11_SCK_DELAY);
	set_sck_high();
	ack = SHT11_PIN & _BV(SHT11_DATA);
	_delay_ms(SHT11_SCK_DELAY);
	set_sck_low();

	return (ack);
}

void send_start_command(void)
{
	/* DATA:   _____           ________
	   DATA:         |_______|
	   SCK :       ___       ___
	   SCK :  ___|     |___|     |______
	 */

	set_data_high();
	/*   set_data_out (); */
	_delay_ms(SHT11_SCK_DELAY);
	set_sck_high();
	_delay_ms(SHT11_SCK_DELAY);
	set_data_low();
	_delay_ms(SHT11_SCK_DELAY);
	set_sck_low();
	_delay_ms(SHT11_SCK_DELAY);
	set_sck_high();
	_delay_ms(SHT11_SCK_DELAY);
	set_data_high();
	_delay_ms(SHT11_SCK_DELAY);
	set_sck_low();
	_delay_ms(SHT11_SCK_DELAY);
}

void sht11_init(void)
{
	/* sht11 clk pin to output and set high */
	SHT11_DDR |= _BV(SHT11_SCK);
	set_sck_high();
}

uint8_t sht11_read_status_reg(void)
{
	uint8_t result, crc8, ack;

	result = 0;
	crc8 = 0;
	send_start_command();

	/* send read status register command */
	send_byte(7);
	ack = read_ack();

	if (ack)
		return (0);

	result = read_byte();

	/* Send ack */
	send_ack();

	/* inizio la lettura del CRC-8 */
	crc8 = read_byte();

	send_ack();
	return (result);
}

/* Disable Interrupt to avoid possible clk problem. */
uint16_t sht11_send_command(uint8_t command)
{
	uint16_t result;
	uint8_t crc8, ack;

	/* safety 000xxxxx */
	command &= 31;
	result = 0;
	crc8 = 0;

	send_start_command();
	send_byte(command);
	ack = read_ack();

	if (!ack) {
		/* And if nothing came back this code hangs here */
		loop_until_bit_is_set(SHT11_PIN, SHT11_DATA);
		loop_until_bit_is_clear(SHT11_PIN, SHT11_DATA);

		/* inizio la lettura dal MSB del primo byte */
		result = read_byte() << 8;

		/* Send ack */
		send_ack();

		/* inizio la lettura dal MSB del secondo byte */
		result |= read_byte();

		send_ack();

		/* inizio la lettura del CRC-8 */
		crc8 = read_byte();

		/* do not Send ack */
		set_data_high();
		_delay_ms(SHT11_SCK_DELAY);
		set_sck_high();
		_delay_ms(SHT11_SCK_DELAY);
		set_sck_low();
		set_data_in();
	}

	return (result);
}

void sht11_dewpoint(struct sht11_t *dataset)
{
	double k;
	k = (log10(dataset->humidity_compensated) - 2) / 0.4343 +
	    (17.62 * dataset->temperature) / (243.12 + dataset->temperature);
	dataset->dewpoint = 243.12 * k / (17.62 - k);
}

void sht11_read_temperature(struct sht11_t *dataset)
{
	dataset->raw_temperature = sht11_send_command(SHT11_CMD_MEASURE_TEMP);
	dataset->temperature = SHT11_T1 * dataset->raw_temperature - 40;
}

void sht11_read_humidity(struct sht11_t *dataset)
{
	dataset->raw_humidity = sht11_send_command(SHT11_CMD_MEASURE_HUMI);
	dataset->humidity_linear = SHT11_C1 +
	    (SHT11_C2 * dataset->raw_humidity) +
	    (SHT11_C3 * dataset->raw_humidity * dataset->raw_humidity);

	/* Compensate humidity result with temperature */
	dataset->humidity_compensated = (dataset->temperature - 25) *
	    (SHT11_T1 + SHT11_T2 * dataset->raw_humidity) +
	    dataset->humidity_linear;

	/* Range adjustment */
	if (dataset->humidity_compensated > 100)
		dataset->humidity_compensated = 100;
	if (dataset->humidity_compensated < 0.1)
		dataset->humidity_compensated = 0.1;
}

void sht11_read_all(struct sht11_t *dataset)
{
	sht11_read_temperature(dataset);
	sht11_read_humidity(dataset);
	sht11_dewpoint(dataset);
}
