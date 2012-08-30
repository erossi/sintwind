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

#ifndef SHT11_IO_H
#define SHT11_IO_H

void set_sck_out(void);
void set_sck_in(void);
void set_sck_high(void);
void set_sck_low(void);
void set_data_out(void);
void set_data_in(void);
void set_data_high(void);
void set_data_low(void);
uint8_t read_data_pin(void);
void sck_delay(void);
uint8_t wait_until_data_is_ready(void);
uint8_t sht11_crc8(uint8_t crc, uint8_t data);
int sht11_io_init(void);
void sht11_io_end(void);

#endif
