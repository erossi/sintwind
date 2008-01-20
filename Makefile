MCU = atmega8
INC = -I/usr/avr/include/
LIBS = m
OPTLEV = 2
export CFLAGS = $(INC) -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV)
export LFLAGS = -l$(LIBS)
export CC = avr-gcc

OBJCOPY = avr-objcopy -j .text -j .data -O ihex
OBJDUMP = avr-objdump
SIZE = avr-size
# DUDE = avrdude -c stk500v1 -p m8515 -P /dev/ttyUSB0 -e -U flash:w:sint2.hex -F
REMOVE = rm -f
objects = uart.o adc.o
SUBDIRS = sht11 synth anemometer

.PHONY: clean indent $(SUBDIRS)
.SILENT: help
.SUFFIXES: .c, .o

all: $(SUBDIRS) $(objects)
	$(CC) $(CFLAGS) -o sint2.elf main.c $(objects) $(LFLAGS)
	$(OBJCOPY) sint2.elf sint2.hex

program:
	avrdude -c stk500v1 -p $(MCU) -P /dev/ttyUSB0 -e -U flash:w:sint2.hex

$(SUBDIRS):
	@echo $(MAKECMDGOALS)
	$(MAKE) -C $@ $(MAKECMDGOALS)

test_adc:
	$(MAKE) -C anemometer
	$(MAKE) -C synth
	$(CC) $(CFLAGS) -o test.elf test_adc.c anemometer/adc.o \
		anemometer/anemometer.o synth/synth.o 
	$(OBJCOPY) test.elf test.hex

test_anemometer: $(objects)
	$(CC) $(CFLAGS) -o test.elf test_anemometer.c $(objects) $(LFLAGS)
	$(OBJCOPY) test.elf test.hex

test_synth:
	$(MAKE) -C synth
	$(CC) $(CFLAGS) -o test.elf test_synth.c synth/synth.o 
	$(OBJCOPY) test.elf test.hex

clean: $(SUBDIRS)
	rm $(objects) *.elf *.hex

indent:
	indent *.c *.h
