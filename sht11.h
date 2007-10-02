#ifndef _SHT11_H_
#define _SHT11_H_

#define SHT11_DDR DDRC
#define SHT11_PORT PORTC
#define SHT11_PIN PINC

#define SHT11_SCK PC0
#define SHT11_DATA PC1

/* clock delay in ms */
#define SHT11_SCK_DELAY 1

void
sht11_init (void);

uint16_t
sht11_send_command (uint8_t command);

uint8_t
sht11_read_status_reg (void);

#endif
