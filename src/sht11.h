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

struct sht11_t {
	uint16_t raw_temperature;
        uint8_t raw_temperature_crc8; /* read */
        uint8_t raw_temperature_crc8c; /* calculated */
	uint16_t raw_humidity;
        uint8_t raw_humidity_crc8;
        uint8_t raw_humidity_crc8c;
	uint8_t status_reg;
	uint8_t status_reg_crc8;
	uint8_t status_reg_crc8c;
	double temperature;
	double humidity_linear;
	double humidity_compensated;
	double dewpoint;
	uint8_t cmd; /* command to send */
	uint16_t result; /* result of the command */
	uint8_t crc8; /* crc8 returned */
	uint8_t crc8c; /* crc8 calculated */
};

void sht11_init(struct sht11_t *sht11);
void sht11_end(void);
void sht11_read_all(struct sht11_t *sht11);

#endif
