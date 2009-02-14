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
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "default.h"
#include "uart.h"
#include "cell.h"
#include "debug.h"

extern struct uartStruct *uartPtr;

void debug_write(const char *s)
{
	uart_printstr(s);
}

void debug_hello(void)
{
	debug_write("\r\n");
	debug_write("***********************************************");
	debug_write("*                                             *");
	debug_write("* Open Sint Project - Sint Wind vX.Y          *");
	debug_write("*               By                            *");
	debug_write("*                                             *");
	debug_write("* Andrea Marabini (info@marabo.it)            *");
	debug_write("* Enrico Rossi    (e.rossi@tecnobrain.com)    *");
	debug_write("*                                             *");
	debug_write("* http://tecnobrain.com/OpenSint/             *");
	debug_write("*            GNU GPL v3                       *");
	debug_write("*                                             *");
	debug_write("***********************************************");
}

int debug_phone_on(void)
{
	return(1);
}

