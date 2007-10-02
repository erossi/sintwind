#include <stdint.h>

#include <avr/io.h>
#include "default.h"
#include "sht11.h"
#include <util/delay.h>

void
set_sck_high (void)
{
  SHT11_PORT |= _BV(SHT11_SCK);
}

void
set_sck_low (void)
{
  SHT11_PORT &= ~_BV(SHT11_SCK);
}

void
set_data_out (void)
{
  SHT11_DDR |= _BV(SHT11_SCK) | _BV(SHT11_DATA);
}

void
set_data_in (void)
{
  SHT11_DDR &= _BV(SHT11_SCK) | ~_BV(SHT11_DATA);
}

void
set_data_high (void)
{
  /*   SHT11_PORT |= _BV(SHT11_DATA); */
  set_data_in ();
}

void
set_data_low (void)
{
  SHT11_PORT &= ~_BV(SHT11_DATA);
  set_data_out ();
}

void send_byte (uint8_t byte)
{
  uint8_t i;

  i=8;

  while (i)
    {
      --i;

      if (byte & _BV(i))
	set_data_high ();
      else
	set_data_low ();
      _delay_ms (SHT11_SCK_DELAY);

      set_sck_high ();
      _delay_ms (SHT11_SCK_DELAY);

      set_sck_low ();
    }
}

uint8_t
read_byte (void)
{
  uint8_t i, bit, result ;

  result = 0;
  i=8;

  while (i)
    {
      --i;

      _delay_ms (SHT11_SCK_DELAY);
      set_sck_high ();
      bit = SHT11_PIN & _BV(SHT11_DATA);
      _delay_ms (SHT11_SCK_DELAY);
      set_sck_low ();

      if (bit)
	result |= _BV(i);
    }

  return (result);
}

void
send_ack (void)
{
  /* Send ack */
  set_data_low ();
  _delay_ms (SHT11_SCK_DELAY);
  set_sck_high ();
  _delay_ms (SHT11_SCK_DELAY);
  set_sck_low ();
  set_data_in ();
}

uint8_t
read_ack (void)
{
  uint8_t ack;

  /* read ack after command */
  set_data_in ();
  _delay_ms (SHT11_SCK_DELAY);
  set_sck_high ();
  ack = SHT11_PIN & _BV(SHT11_DATA);
  _delay_ms (SHT11_SCK_DELAY);
  set_sck_low ();

  return (ack);
}

void
send_start_command (void)
{
  set_data_high ();
  /*   set_data_out (); */
  _delay_ms(SHT11_SCK_DELAY);
  set_sck_high ();
  _delay_ms(SHT11_SCK_DELAY);
  set_data_low ();
  _delay_ms(SHT11_SCK_DELAY);
  set_sck_low ();
  _delay_ms (SHT11_SCK_DELAY);
  set_sck_high ();
  _delay_ms (SHT11_SCK_DELAY);
  set_data_high ();
  _delay_ms(SHT11_SCK_DELAY);
  set_sck_low ();
  _delay_ms (SHT11_SCK_DELAY);
}

void
sht11_init (void)
{
  /* sht11 clk pin to output and set high */
  SHT11_DDR |= _BV(SHT11_SCK);
  set_sck_high();
}

uint8_t
sht11_read_status_reg (void)
{
  uint8_t result, crc8, ack;

  result = 0;
  crc8 = 0;

  send_start_command ();

  /* send read status register command */
  send_byte (7);
  ack = read_ack ();

  if (ack)
    return (0);

  result = read_byte ();

  /* Send ack */
  send_ack ();

  /* inizio la lettura del CRC-8 */
  crc8 = read_byte ();

  send_ack();

  return (result);
}

uint16_t
sht11_send_command (uint8_t command)
{
  uint16_t result;
  uint8_t crc8, ack;

  /* safety 000xxxxx */
  command &= 31;
  result = 0;
  crc8 = 0;

  send_start_command ();
  send_byte (command);
  ack = read_ack ();

  if (ack)
    return (0);

  loop_until_bit_is_set (SHT11_PIN, SHT11_DATA);
  loop_until_bit_is_clear (SHT11_PIN, SHT11_DATA);

  /* inizio la lettura dal MSB del primo byte */
  result = read_byte() << 8;

  /* Send ack */
  send_ack ();

  /* inizio la lettura dal MSB del secondo byte */
  result |= read_byte ();

  /* Send ack */
  send_ack ();

  /* inizio la lettura del CRC-8 */
  crc8 = read_byte ();

  /* do not Send ack */
  set_data_high ();
  _delay_ms (SHT11_SCK_DELAY);
  set_sck_high ();
  _delay_ms (SHT11_SCK_DELAY);
  set_sck_low ();
  set_data_in ();

  return (result);
}
