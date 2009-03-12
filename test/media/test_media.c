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

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "default.h"
#include "media.h"

int main(void)
{
	struct wind_array *wind;
	int i;

	wind = malloc(sizeof(struct wind_array));
	wind->angle_rt = 0;
	printf("In (Hz) | SpeesRT | AngleRT | vminRT | VmaxRT | ");
	printf("vmin | Vmax | Speed | Angle");
	printf("\n");

	for (i=0; i<300; i++) {
		wind->speed_rt = i;
		media(wind);
		printf("%d | ", i);
		printf("%d | %d | %d | ", wind->speed_rt, wind->angle_rt, wind->vmin_rt);
		printf("%d | %d | %d | ", wind->vmax_rt, wind->vmin, wind->vmax);
		printf("%d | %d", wind->speed, wind->angle);
		printf("\n");

	}

	free(wind);
	return(0);
}