#MCU = atmega8515
MCU = atmega16
INC = -I/usr/avr/include/
LIBS = m
OPTLEV = 0
CFLAGS = $(INC) -g -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV)
LFLAGS = -l$(LIBS)
CC = avr-gcc
OBJCOPY = avr-objcopy -j .text -j .data -O ihex
OBJDUMP = avr-objdump
SIZE = avr-size
DUDE = avrdude -c stk500v1 -p m8515 -P /dev/ttyUSB0 -e -U flash:w:sint2.hex -F
REMOVE = rm -f
objects = uart.o adc.o sht11.o sensor.o

.PHONY: clean indent
.SILENT: help
.SUFFIXES: .c, .o

all: $(objects)
	$(CC) $(CFLAGS) -o sint2.elf main.c $(objects) $(LFLAGS)
	$(OBJCOPY) sint2.elf sint2.hex

program:
	avrdude -c stk500v1 -p $(MCU) -P /dev/ttyUSB0 -e -U flash:w:sint2.hex

clean :
	rm $(objects) *.elf *.hex

indent :
	indent *.c *.h
