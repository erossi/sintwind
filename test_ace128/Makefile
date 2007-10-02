MCU = atmega8515
#MCU = atmega8
INC = -I/usr/avr/include/
LIBS = m
OPTLEV = 2
CFLAGS = $(INC) -g -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV)
LFLAGS = -l$(LIBS)
objects = ace128.o
CC = avr-gcc
OBJCOPY = avr-objcopy -j .text -j .data -O ihex
OBJDUMP = avr-objdump
SIZE = avr-size
AVRDUDE = avrdude -c stk500 -p m8515 -P /dev/ttyUSB0 -e -U flash:w:
REMOVE = rm -f

.PHONY: clean indent

all: $(objects)
	$(CC) $(CFLAGS) -o test_ace128.elf test_ace128.c $(objects) $(LFLAGS)
	$(OBJCOPY) test_ace128.elf test_ace128.hex

ace128.o: ace128.c ace128.h default.h
	$(CC) $(CFLAGS) -c ace128.c

clean :
	rm $(objects) *.elf *.hex

indent :
	indent *.c *.h
