#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "default.h"

extern uint8_t *adc_area;
extern uint16_t adc_index;
extern uint8_t adc_runover;

/*
 * ADC conversion complete.  Fetch the 8-bit value
 */
ISR(ADC_vect)
{
  *(adc_area + adc_index) = ADCH;
  ++adc_index;

  if (adc_index >> ADC_CUTOFF)
    {
      ADCSRA &= ~_BV(ADIE);		/* disable ADC interrupt */
      adc_runover++;
      adc_index = 0;
    }
}

void
adc_init (void)
{
  /*
   enable ADC, select ADC clock = F_CPU / 8 (i.e. 125 kHz)
   interrupt enabled
   */
  ADCSRA = _BV(ADEN) | _BV(ADATE) | _BV(ADIE) | _BV(ADPS1) | _BV(ADPS0);

/* F_CPU / 16 */
/*   ADCSRA = _BV(ADEN) | _BV(ADATE) | _BV(ADIE) | _BV(ADPS2); */

  /* Set left adjusted result */
  ADMUX = _BV(ADLAR);
}

void
adc_start (void)
{
  ADCSRA |= _BV(ADIE);
  ADCSRA |= _BV(ADSC);
}
