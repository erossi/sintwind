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
objects = uart.o sht11.o

.PHONY: clean indent
.SILENT: help
.SUFFIXES: .c, .o

all: $(objects)
	$(CC) $(CFLAGS) -o test_sht11.elf test_sht11.c $(objects) $(LFLAGS)
	$(OBJCOPY) test_sht11.elf test_sht11.hex

program:
	avrdude -c stk500v1 -p $(MCU) -P /dev/ttyUSB0 -e -U flash:w:test_sht11.hex

clean :
	rm $(objects) *.elf *.hex

indent :
	indent *.c *.h
