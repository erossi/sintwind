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

/*
 * Delay.h CPU speed definition
 */

#ifndef F_CPU

/* CPU Speed 1 MHz */
#define F_CPU 1000000UL
/*
 #define F_CPU 4000000UL 4Mhz
 #define F_CPU 14.7456E6

 maximum delay possible is: (see util/delay.h)
 The maximal possible delay is 262.14 ms / F_CPU in MHz.
 so we have 4Mhz CPU, maximum is 65ms. (65.535 actually).
*/
#endif

/*
 *  Set the prescaler used for the timer.
 */

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

enum wind_dir
{
  NORTH,
  NORTH_EAST,
  EAST,
  SOUTH_EAST,
  SOUTH,
  SOUTH_WEST,
  WEST,
  NORTH_WEST
};

enum wind_tendency
{
  INCREASE,
  DECREASE,
  STABLE
};

struct complex
{
  float x;
  float y;
};

struct wind_array
{
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

  /*
    wind elements
    speed
    0-359 degrees
  */
  uint8_t speed, vmin, vmax;
  int angle;
  enum wind_dir direction;
  enum wind_tendency tendency;

  /*
    real time elements
  */
  uint8_t vmin_rt, vmax_rt;
  struct complex vector_rt, media_rt;
  uint8_t counter_rt;
};

#define UART_BAUD 9600
#define UART_RXBUF_SIZE 128
#define UART_TXBUF_SIZE 128
#define UART_RXBUF_MASK ( UART_RXBUF_SIZE - 1 )
#define UART_TXBUF_MASK ( UART_TXBUF_SIZE - 1 )
#if ( UART_RXBUF_SIZE & UART_RXBUF_MASK )
#error RX buffer size is not a power of 2
#endif
#if ( UART_TXBUF_SIZE & UART_TXBUF_MASK )
#error TX buffer size is not a power of 2
#endif

struct uartStruct
{
  char *rx_buffer;
  char *tx_buffer;
  volatile uint8_t rx_flag, tx_flag, rxIdx, txIdx;
};

#endif
