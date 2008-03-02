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

objects = init/init.o anemometer/adc.o anemometer/anemometer.o \
	  synth/synth.o media/media.o
SUBDIRS = synth anemometer

.PHONY: clean indent $(SUBDIRS)
.SILENT: help
.SUFFIXES: .c, .o

all: $(SUBDIRS) $(objects)
	$(CC) $(CFLAGS) -o sint.elf main.c $(objects) $(LFLAGS)
	$(OBJCOPY) sint.elf sint.hex

program:
	avrdude -c stk500v1 -p $(MCU) -P /dev/ttyUSB0 -e -U flash:w:sint.hex

$(SUBDIRS):
	@echo $(MAKECMDGOALS)
	$(MAKE) -C $@ $(MAKECMDGOALS)

test_media:
	$(MAKE) -C init
	$(MAKE) -C media
	$(MAKE) -C anemometer
	$(MAKE) -C synth
	$(CC) $(CFLAGS) -o sint.elf test_media.c \
		anemometer/adc.o anemometer/anemometer.o anemometer/isr.o \
		synth/synth.o \
		init/init.o \
		media/media.o
	$(OBJCOPY) sint.elf sint.hex

test_anemometer: $(objects)
	$(MAKE) -C anemometer
	$(MAKE) -C synth
	$(CC) $(CFLAGS) -o sint.elf test_anemometer.c $(objects) \
		anemometer/adc.o anemometer/isr.o \
		anemometer/anemometer.o synth/synth.o 
	$(OBJCOPY) sint.elf sint.hex

test_adc:
	$(MAKE) -C anemometer
	$(MAKE) -C synth
	$(CC) $(CFLAGS) -o sint.elf test_adc.c anemometer/adc.o \
		anemometer/anemometer.o synth/synth.o 
	$(OBJCOPY) sint.elf sint.hex

test_synth:
	$(MAKE) -C synth
	$(CC) $(CFLAGS) -o test.elf test_synth.c synth/synth.o 
	$(OBJCOPY) sint.elf sint.hex

clean: $(SUBDIRS)
	rm $(objects) sint.elf sint.hex

indent:
	indent *.c *.h
