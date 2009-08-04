MCU = atmega8
INC = -I/usr/avr/include/
LIBS = m
OPTLEV = 2
FCPU = 4000000UL
CFLAGS = $(INC) -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV) -D F_CPU=$(FCPU)
# for test disable 1h delay routine
# CFLAGS += -D NO_DELAY_AT_BOOT
LFLAGS = -l$(LIBS)
CC = avr-gcc

REMOVE = rm -f

objects = sht11.o cell.o init.o adc.o anemometer.o synth.o media.o isr.o utils.o

.PHONY: clean indent
.SILENT: help
.SUFFIXES: .c, .o

all: $(objects)
	$(CC) $(CFLAGS) -o ../sint.elf main.c $(objects) $(LFLAGS)

test_sht11: $(objects)
	$(CC) $(CFLAGS) -o ../sint.elf test_sht11.c $(objects) $(LFLAGS)

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
