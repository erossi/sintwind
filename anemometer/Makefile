MCU = atmega8
INC = -I/usr/avr/include/
LIBS = m
OPTLEV = 2
CFLAGS = $(INC) -g -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV)
LFLAGS = -l$(LIBS)
CC = avr-gcc
OBJCOPY = avr-objcopy -j .text -j .data -O ihex
OBJDUMP = avr-objdump
SIZE = avr-size
DUDE = avrdude -c stk500v1 -p m8515 -P /dev/ttyUSB0 -e -U flash:w:sint2.hex -F
REMOVE = rm -f
objects = anemometer.o adc.o isr.o

.PHONY: clean indent
.SILENT: help
.SUFFIXES: .c, .o

all: $(objects)

test_anemometer: $(objects)
	$(CC) $(CFLAGS) -o test.elf test_anemometer.c $(objects) $(LFLAGS)
	$(OBJCOPY) test.elf test.hex

test_adc: adc.o
	$(CC) $(CFLAGS) -o test.elf test_adc.c adc.o
	$(OBJCOPY) test.elf test.hex

program:
	avrdude -c stk500v1 -p $(MCU) -P /dev/ttyUSB0 -e -U flash:w:test.hex

clean :
	rm $(objects) *.elf *.hex

indent :
	indent *.c *.h
