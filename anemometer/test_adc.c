#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include "adc.h"

int
main (void)
{
/* Out */
  DDRD = 0xff;

/* In */
  DDRB = 0;

  adc_init ();

  for (;;)
    {
      PORTD = 0xff;
      loop_until_bit_is_clear (PINB, 0);

      PORTD = 0;
      loop_until_bit_is_clear (PINB, 1);

      PORTD = adc_read ();
      loop_until_bit_is_clear (PINB, 0);

    }
}
