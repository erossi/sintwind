#include <stdint.h>
#include <stdlib.h>

/* #include <avr/interrupt.h> */
#include <avr/io.h>
#include "default.h"
#include "sensor.h"
#include "uart.h"

/* #include "adc.h" */
#include <util/delay.h>

uint8_t *adc_area;
uint16_t adc_index;
uint8_t adc_runover;

int
main(void)
{
/*   uint16_t i = 0; */
/*   uint16_t adc_size = 1 << ADC_CUTOFF; */
  int result;
  char *string;

  DDRB = 255;
  PORTB = 0xff;
  DDRA = 0;
  string = malloc (10);

  sensor_init ();
  uart_init();

/*   adc_init (); */

/*   adc_area = malloc (adc_size); */
/*   adc_index = 0; */
/*   adc_runover = 0; */
/*   sei (); */

/*   adc_start (); */

  for (;;)
    {
      loop_until_bit_is_clear (PINA, 0);
      PORTB = 0;
      _delay_ms (10);
      PORTB = 0xff;

      result = (int)sensor_read_temperature ();
      if (result != -999)
	{
	  itoa (result, string, 10);
	  uart_printstr (string);
	  uart_putchar ('\n');
	}

      _delay_ms (10);

      result = (int)sensor_read_humidity ();
      if (result != -999)
	{
	  itoa (result, string, 10);
	  uart_printstr (string);
	  uart_putchar ('\n');
	}

      _delay_ms (10);
    }

  free (adc_area);
}
