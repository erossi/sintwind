/* This file is part of OpenSint
 * Copyright (C) 2005-2007 Enrico Rossi
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

#include <avr/io.h>
#include "default.h"
#include "synth.h"
#include "cell.h"
#include "bus.h"
/* put this after default because we have to set F_CPU */
#include <util/delay.h>

extern enum address_bus addr_bus;

void switch_bus (enum address_bus bus, uint8_t out)
{
  switch (bus)
    {
    case WIND_DIR:
      /*
	we must set address bus to wind dir encoders (pin 0 to 4)
	and data bus to input (pin 0 to 7).
      */
      ADDRBUS_PORT = 3; /* NULL */
      _delay_ms (1);
      DATABUS_DDR = 0; /* all input */
      ADDRBUS_PORT = 1; /* msb 00000001 lsb */
      _delay_ms (1);
      break;

    case READ_PHONE:
      /*
	ab = to phone (p0 - 4) and db to input (pin 0, 1).
      */
      ADDRBUS_PORT = 3; /* NULL */
      _delay_ms (1);
      DATABUS_DDR = 0;
      ADDRBUS_PORT = 2; /* 00000010 */
      _delay_ms (1);
      break;

    case WRITE_PHONE:
      /*
	ab = to phone (different to phone read)
	and db to output (pin 0 - 3).
	since pin 4 to 7 are used for synth we must clear them.
	Then we have to keep synth PD and CE to 1 (synth reset condition).
      */
      ADDRBUS_PORT = 3; /* NULL */
      _delay_ms (1);
      DATABUS_DDR = 0xff;
      out &= 15; /* synth part [0000] cell part [1111] */
      DATABUS_PORT = (out | _BV (PD) | _BV (CE));
      _delay_ms (1);
      ADDRBUS_PORT = 7; /* 00000111 */
      break;

    case SYNTH_CTRL:
      /*
	This control synth PD /CE A8
	ab pc4 -> 00111 <- pc0
	db out pd 4(PD), 5(/CE), 6(A8)
      */
      ADDRBUS_PORT = 3; /* NULL */
      _delay_ms (1);
      DATABUS_DDR = 0xff;
      out &= 240; /* synth part [1111] cell part [0000] */
      DATABUS_PORT = out;
      _delay_ms (1);
      ADDRBUS_PORT = 7;
      break;

    case SYNTH_ADDR:
      /*
	This control synth A0 - A7
	ab pc4 -> 01011 <- pc0
	db out pd 0-7
      */
      ADDRBUS_PORT = 3; /* NULL */
      _delay_ms (1);
      DATABUS_DDR = 0xff;
      DATABUS_PORT = out;
      _delay_ms (1);
      ADDRBUS_PORT = 11;
/*       DATABUS_PORT = 0; */
      break;

    case SYNTH_EOM:
      /*
	This control synth EOM pin
	ERRATO! ab pc4 -> 10010 <- pc0
	ab pc4 -> 00010 <- pc0
	db in pd2
	ab = to synth (equal to phone) and db to input (pin 2).
      */
      ADDRBUS_PORT = 3; /* NULL */
      _delay_ms (1);
      DATABUS_DDR = 0; /* READ */
/*       DATABUS_PORT = out; */
      ADDRBUS_PORT = 2; /* 00000010 */
/*       DATABUS_DDR = 251; /\* 11111011 *\/ */
      _delay_ms (1);
      break;

    case NULL_BUS:
      /* NULL address */
      /* ADDRBUS_PORT = 3 pc0 -> 11000 <- pc4 */
      ADDRBUS_PORT = 3; /* NULL */
      _delay_ms (1);
	break;
    }
}
