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

#ifndef DEFAULT_WIND_H_
#define DEFAULT_WIND_H_

/* ---- Delay.h CPU speed definition ---- */
/*
 * MUST BE IN MAKEFILE OR IN GCC -D F_CPU=4000000UL

 #define F_CPU 1000000UL 1Mhz
 #define F_CPU 4000000UL 4Mhz
 #define F_CPU 14.7456E6

 maximum delay possible is: (see util/delay.h)
 The maximal possible delay is 262.14 ms / F_CPU in MHz.
 so we have 4Mhz CPU, maximum is 65ms. (65.535 actually).
 Over this range you loose precision.
*/

/* ---- Anemometer ---- */

#define LACROSSE_CE_PORT PORTD
#define LACROSSE_CE PD3
#define LACROSSE_RX_PORT PIND
#define LACROSSE_RX PIND2

/* Set the prescaler used for the timer. */

/* #define TIMER_PRESCALER_1024 */
#define TIMER_PRESCALER_256
/* #define TIMER_PRESCALER_64 */

/*
  If We want to use the media value at run (x) as 
  the first element of run (x+1).
  This lead to a wrong value into the 2 first run from the 
  power up.

#define MEDIA_NEXT_CYCLE 1
 */

enum wind_dir {
	NORTH,
	NORTH_EAST,
	EAST,
	SOUTH_EAST,
	SOUTH,
	SOUTH_WEST,
	WEST,
	NORTH_WEST
};

enum wind_tendency {
	INCREASE,
	DECREASE,
	STABLE
};

struct complex {
	float x;
	float y;
};

struct wind_array {
	/*
	   Volatile stuff, used into ISR
	   flag: boolean used by ISR to tell to main there is a new
	   data (speed and direction) to be elaborated.
	   speed_rt: number of pulse x round detected.
	   angle_rt: 0-359 degrees detected
	 */
	volatile uint8_t flag;
	volatile uint8_t speed_rt;
	volatile int angle_rt;

	/* wind elements speed 0-359 degrees */
	uint8_t speed, vmin, vmax;
	int angle;
	enum wind_dir direction;
	enum wind_tendency tendency;

	/* real time elements */
	uint8_t vmin_rt, vmax_rt;
	struct complex vector_rt, media_rt;
	uint8_t counter_rt;

	/* Type of anemometer: 0 for lacrosse */
	uint8_t sensor;

	/* LaCrosse data
 	   [00000100][0000bbbb][000vvvvvvvvvvvv][0000cccc]
	 */
	uint8_t lacrosse_head, lacrosse_bearing, lacrosse_chksum;
	uint16_t lacrosse_speed;
};


/* ---- Cell and modem ---- */

#define UART_HAVE_DEFAULT
#define UART_BAUD 9600
/* Keep in mind that you don't have enought RAM */
#define UART_RXBUF_SIZE 64
#define UART_TXBUF_SIZE 64
#define UART_RXBUF_MASK ( UART_RXBUF_SIZE - 1 )
#define UART_TXBUF_MASK ( UART_TXBUF_SIZE - 1 )
#if ( UART_RXBUF_SIZE & UART_RXBUF_MASK )
#error RX buffer size is not a power of 2
#endif
#if ( UART_TXBUF_SIZE & UART_TXBUF_MASK )
#error TX buffer size is not a power of 2
#endif

struct uartStruct {
	char *rx_buffer;
	char *tx_buffer;
	volatile uint8_t rx_flag, tx_flag, rxIdx, txIdx;
};

#define PHONE_HAVE_DEFAULT
#define PHONE_PORT PORTB
#define PHONE_ON PB0


/* ---- Utils setup ----- */

#define UTILS_HAVE_DEFAULT
#define UTILS_LED_PORT PORTB
#define UTILS_LED_PIN PB6
#define UTILS_SWITCH_PORT PINB
#define UTILS_SWITCH_PIN PINB7


/* ---- Synth ---- */

#define SYNTH_HAVE_DEFAULT
#define SYNTH_ADDR PORTC
#define SYNTH_CTRL_OUT PORTB
#define SYNTH_CTRL_IN  PINB
#define SYNTH_EOM PINB3
#define SYNTH_PD PB4
#define SYNTH_CE PB5


/* ---- sht11 ----- */

#define SHT11_HAVE_DEFAULT
#define SHT11_DDR	DDRB
#define SHT11_PORT	PORTB
#define SHT11_PIN	PINB
#define SHT11_DATA	PB1
#define SHT11_SCK	PB2

struct sht11_t {
	uint16_t raw_temperature;
	uint16_t raw_humidity;
	float temperature;
	float humidity_linear;
	float humidity_compensated;
	float dewpoint;
};

#endif
