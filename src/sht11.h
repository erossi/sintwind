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

#ifndef SHT11_H_
#define SHT11_H_

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

struct sht11_t {
	uint16_t raw_temperature;
        uint8_t raw_temperature_crc8; /* read */
        uint8_t raw_temperature_crc8c; /* calculated */
	uint16_t raw_humidity;
        uint8_t raw_humidity_crc8;
        uint8_t raw_humidity_crc8c;
	double temperature;
	double humidity_linear;
	double humidity_compensated;
	double dewpoint;
	uint8_t cmd; /* command to send */
	uint16_t result; /* result of the command */
	uint8_t crc8; /* crc8 returned */
	uint8_t crc8c; /* crc8 calculated */
};

void sht11_init(void);
void sht11_read_all(struct sht11_t *sht11);

#endif
