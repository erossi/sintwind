#ifndef _SHT11_H_
#define _SHT11_H_

#define SHT11_DDR	DDRC
#define SHT11_PORT	PORTC
#define SHT11_PIN	PINC
#define SHT11_SCK	PC0
#define SHT11_DATA	PC1

#define SHT11_CMD_STATUS_REG_W 6
#define SHT11_CMD_STATUS_REG_R 7
#define SHT11_CMD_MEASURE_TEMP 3
#define SHT11_CMD_MEASURE_HUMI 5
#define SHT11_CMD_RESET        15

#define SHT11_C1 -4.0        /* for 12 Bit */
#define SHT11_C2  0.0405     /* for 12 Bit */
#define SHT11_C3 -0.0000028  /* for 12 Bit */
#define SHT11_T1  0.01       /* for 14 Bit @ 5V */
#define SHT11_T2  0.00008    /* for 14 Bit @ 5V */

/* clock delay in ms */
#define SHT11_SCK_DELAY 1

struct sht11_t
{
  uint16_t raw_temperature;
  uint16_t raw_humidity;
  float temperature;
  float humidity_linear;
  float humidity_compensated;
  float dewpoint;
};

void
sht11_init (void);

uint16_t
sht11_send_command (uint8_t command);

uint8_t
sht11_read_status_reg (void);

void
sht11_read_all (struct sht11_t *dataset);

#endif
