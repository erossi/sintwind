#MCU = atmega8515
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
AVRDUDE = avrdude -c stk500 -p m8 -P /dev/ttyUSB0 -e -U flash:w:
REMOVE = rm -f
objects = bus.o ace128.o anemometer.o init.o synth.o cell.o

.PHONY: clean indent
.SILENT: help
.SUFFIXES: .c, .o

all: $(objects)
	$(CC) $(CFLAGS) -o sintwind.elf main.c $(objects) $(LFLAGS)
	$(OBJCOPY) sintwind.elf sintwind.hex

usart.o: usart.c usart.h default.h
	$(CC) $(CFLAGS) -c usart.c

test_ace128: ace128.o
	$(CC) $(CFLAGS) -o test_ace128.elf test_ace128.c ace128.o
	$(OBJCOPY) test_ace128.elf test_ace128.hex

test_anemometer: ace128.o anemometer.o usart.o
	$(CC) $(CFLAGS) -o test_anemometer.elf test_anemometer.c \
	ace128.o anemometer.o usart.o $(LFLAGS)
	$(OBJCOPY) test_anemometer.elf test_anemometer.hex

test_bus: $(objects)
	$(CC) $(CFLAGS) -o test_bus.elf test_bus.c $(objects) $(LFLAGS)
	$(OBJCOPY) test_bus.elf test_bus.hex

test_synth: $(objects)
	$(CC) $(CFLAGS) -o test_synth.elf test_synth.c $(objects) $(LFLAGS)
	$(OBJCOPY) test_synth.elf test_synth.hex

test_wind_synth.o: test_wind_synth.c synth.h default.h
	$(CC) $(CFLAGS) -c test_wind_synth.c

test_wind: bus.o ace128.o anemometer.o init.o cell.o test_wind_synth.o
	$(CC) $(CFLAGS) -o test_wind.elf test_wind.c bus.o ace128.o anemometer.o init.o cell.o test_wind_synth.o $(LFLAGS)
	$(OBJCOPY) test_wind.elf test_wind.hex

clean :
	rm $(objects) *.elf *.hex

indent :
	indent *.c *.h
