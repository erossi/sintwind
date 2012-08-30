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

#include <stdint.h>
#include <math.h>
#include <avr/io.h>
#include <util/delay.h>

#ifdef HAVE_DEFAULT
#include "default.h"
#endif

#include "sht11_io.h"
#include "sht11.h"

#define SHT11_CMD_STATUS_REG_W 6
#define SHT11_CMD_STATUS_REG_R 7
#define SHT11_CMD_MEASURE_TEMP 3
#define SHT11_CMD_MEASURE_HUMI 5
#define SHT11_CMD_RESET        15

#define SHT11_C1 -4.0		/* for 12 Bit */
#define SHT11_C2  0.0405	/* for 12 Bit */
#define SHT11_C3 -0.0000028	/* for 12 Bit */
#define SHT11_T1  0.01		/* for 14 Bit @ 5V */
#define SHT11_T2  0.00008	/* for 14 Bit @ 5V */

void send_byte(uint8_t byte)
{
	uint8_t i;

	i = 8;

	while (i) {
		--i;

		if (byte & (1<<i))
			set_data_high();
		else
			set_data_low();

		sck_delay();
		set_sck_high();
		sck_delay();
		set_sck_low();
	}
}

uint8_t read_byte(void)
{
	uint8_t i, result;

	result = 0;
	i = 8;

	while (i) {
		--i;
		sck_delay();
		set_sck_high();

		if (read_data_pin())
			result |= (1<<i);

		sck_delay();
		set_sck_low();

	}

	return (result);
}

void send_ack(void)
{
	/* Send ack */
	set_data_low();
	sck_delay();
	set_sck_high();
	sck_delay();
	set_sck_low();
	set_data_in();
}

uint8_t read_ack(void)
{
	uint8_t ack;

	/* read ack after command */
	set_data_in();
	sck_delay();
	set_sck_high();
	ack = read_data_pin();
	sck_delay();
	set_sck_low();

	return (ack);
}

/* terminate a session without sending an ack */
void terminate_no_ack(void)
{
	set_data_high();
	sck_delay();
	set_sck_high();
	sck_delay();
	set_sck_low();
	set_data_in();
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
	sck_delay();
	set_sck_high();
	sck_delay();
	set_data_low();
	sck_delay();
	set_sck_low();
	sck_delay();
	set_sck_high();
	sck_delay();
	set_data_high();
	sck_delay();
	set_sck_low();
	sck_delay();
}

void sht11_read_status_reg(struct sht11_t *sht11)
{
	uint8_t ack;

	sht11->cmd = 7; /* read status reg cmd */
	sht11->status_reg_crc8 = 0;
	sht11->status_reg_crc8c = 0;
	send_start_command();
	send_byte(sht11->cmd);
	ack = read_ack();
	sht11->status_reg_crc8c = sht11_crc8(sht11->status_reg_crc8c, sht11->cmd);

	if (!ack) {
		sht11->status_reg = read_byte();
		sht11->status_reg_crc8c = sht11_crc8(sht11->status_reg_crc8c, sht11->status_reg);
		send_ack();
		sht11->status_reg_crc8 = read_byte();
		terminate_no_ack();
	}
}

/* Disable Interrupt to avoid possible clk problem. */
void send_cmd(struct sht11_t *sht11)
{
	uint8_t ack, byte;

	/* safety 000xxxxx */
	sht11->cmd &= 31;
	sht11->result = 0;
	sht11->crc8 = 0;
	sht11->crc8c = 0;

	send_start_command();
	send_byte(sht11->cmd);
	ack = read_ack();
	sht11->crc8c = sht11_crc8(sht11->crc8c, sht11->cmd);

	if (!ack) {
		/* And if nothing came back this code hangs here */
		wait_until_data_is_ready();

		/* inizio la lettura dal MSB del primo byte */
		byte = read_byte();
		sht11->result = byte << 8;
		sht11->crc8c = sht11_crc8(sht11->crc8c, byte);

		/* Send ack */
		send_ack();

		/* inizio la lettura dal MSB del secondo byte */
		byte = read_byte();
		sht11->result |= byte;
		sht11->crc8c = sht11_crc8(sht11->crc8c, byte);

		send_ack();

		/* inizio la lettura del CRC-8 */
		sht11->crc8 = read_byte();
		terminate_no_ack();
	}
}

void sht11_dewpoint(struct sht11_t *sht11)
{
	double k;
	k = (log10(sht11->humidity_compensated) - 2) / 0.4343 +
	    (17.62 * sht11->temperature) / (243.12 + sht11->temperature);
	sht11->dewpoint = 243.12 * k / (17.62 - k);
}

void sht11_read_temperature(struct sht11_t *sht11)
{
	sht11->cmd = SHT11_CMD_MEASURE_TEMP;
	send_cmd(sht11);
	sht11->raw_temperature = sht11->result;
	sht11->raw_temperature_crc8 = sht11->crc8;
	sht11->raw_temperature_crc8c = sht11->crc8c;
	sht11->temperature = SHT11_T1 * sht11->raw_temperature - 40;
}

void sht11_read_humidity(struct sht11_t *sht11)
{
	sht11->cmd = SHT11_CMD_MEASURE_HUMI;
	send_cmd(sht11);
	sht11->raw_humidity = sht11->result;
	sht11->raw_humidity_crc8 = sht11->crc8;
	sht11->raw_humidity_crc8c = sht11->crc8c;
	sht11->humidity_linear = SHT11_C1 +
	    (SHT11_C2 * sht11->raw_humidity) +
	    (SHT11_C3 * sht11->raw_humidity * sht11->raw_humidity);

	/* Compensate humidity result with temperature */
	sht11->humidity_compensated = (sht11->temperature - 25) *
	    (SHT11_T1 + SHT11_T2 * sht11->raw_humidity) +
	    sht11->humidity_linear;

	/* Range adjustment */
	if (sht11->humidity_compensated > 100)
		sht11->humidity_compensated = 100;
	if (sht11->humidity_compensated < 0.1)
		sht11->humidity_compensated = 0.1;
}

void sht11_init(struct sht11_t *sht11)
{
	sht11_io_init();
	sht11_read_status_reg(sht11);
}

void sht11_end(void)
{
	sht11_io_end();
}

void sht11_read_all(struct sht11_t *sht11)
{
	sht11_read_temperature(sht11);
	sht11_read_humidity(sht11);
	sht11_dewpoint(sht11);
}
