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

#ifndef _ADC_H_
#define _ADC_H_

/*
 * Ratio to convert 0-255 to 0-360 degrees (mostly)
 * From tha Davis sensor we have a range 0 - 4V, then
 * we read 0 - (4/5)*255 value from the adc.
 * This means 0 - 204 rappresent 0-360 degrees.
 * 1.75 seems a good approximation
 */
#define _ADC_RATIO 1.75

void adc_init(void);
int get_wind_position(void);

#endif
