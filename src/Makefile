MCU = atmega8
INC = -I/usr/avr/include/
LIBS = m
OPTLEV = 2
export CFLAGS = $(INC) -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV)
export LFLAGS = -l$(LIBS)
export CC = avr-gcc

REMOVE = rm -f

objects = cell.o init.o adc.o anemometer.o synth.o media.o isr.o
SUBDIRS =

.PHONY: clean indent $(SUBDIRS)
.SILENT: help
.SUFFIXES: .c, .o

all: $(objects)
	$(CC) $(CFLAGS) -o ../sint.elf main.c $(objects) $(LFLAGS)

test_media: $(objects)
	$(CC) $(CFLAGS) -o ../sint.elf test_media.c $(objects) $(LFLAGS)

test_anemometer: $(objects)
	$(CC) $(CFLAGS) -o ../sint.elf test_anemometer.c $(objects) $(LFLAGS)

test_adc: $(objects)
	$(CC) $(CFLAGS) -o ../sint.elf test_adc.c $(objects) $(LFLAGS)

test_synth: $(objects)
	$(CC) $(CFLAGS) -o ../test.elf test_synth.c $(objects) $(LFLAGS)

clean: $(SUBDIRS)
	$(REMOVE) $(objects)

indent:
	indent *.c *.h
