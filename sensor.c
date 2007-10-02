#include <stdint.h>

#include "default.h"
#include "sht11.h"
#include "sensor.h"

void
sensor_init (void)
{
  sht11_init ();
/*       result = sht11_read_status_reg (); */

/*       if (result) */
/* 	{ */
/* 	  itoa (result, string, 10); */
/* 	  uart_printstr (string); */
/* 	  uart_putchar (' '); */
/* 	} */
/*       _delay_ms (10); */


}

float
sensor_read_temperature (void)
{
  uint16_t result;
  float temp=-999;

  result = sht11_send_command (3);

  if (result)
    temp = -40 + 0.01 * result;

  return (temp);
}

float
sensor_read_humidity (void)
{
  uint16_t result;
  float hum=-999;

  result = sht11_send_command (5);

  if (result)
    hum = -4 + (0.0405 * result) + (-0.0000028 * result * result);

  return (hum);
}
