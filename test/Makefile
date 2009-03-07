MCU = atmega16
BASEDIR = /home/enrico/avr/sintwind/opensint/
SRCDIR = $(BASEDIR)/src/
INC = -I /usr/avr/include/ -I $(SRCDIR)
LIBS = m
OPTLEV = 2
FCPU = 4000000UL
AVRCFLAGS = $(INC) -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV) -D F_CPU=$(FCPU)
CFLAGS = $(INC) -Wall -Wstrict-prototypes -pedantic -O$(OPTLEV)
LFLAGS = -l$(LIBS)
AVRCC = avr-gcc
CC = gcc

REMOVE = rm -f

init_obj = init.o
anemometer_obj = davis.o lacrosse.o anemometer.o adc.o
phone_obj = cell.o uart_isr.o uart.o 
synth_obj = sht11.o init.o adc.o anemometer.o synth.o davis.o
objects = sht11.o cell.o init.o synth.o 
objects += media.o uart_isr.o uart.o utils.o debug.o
objects += $(anemometer_obj)
test_media_obj = media.o anemometer.o

.PHONY: clean indent
.SILENT: help
.SUFFIXES: .c, .o

all: $(objects)
	$(CC) $(CFLAGS) -o ../sint.elf main.c $(objects) $(LFLAGS)

media.o:
	$(CC) $(CFLAGS) -c $(SRCDIR)/media.c

anemometer.o:
	$(CC) $(CFLAGS) -c $(SRCDIR)/anemometer.c

uart.o:
	$(CC) $(CFLAGS) -D TXRXI -c uart.c
	 
test_sht11: $(objects)
	$(CC) $(CFLAGS) -o ../sint.elf test_sht11.c $(objects) $(LFLAGS)

test_media: $(test_media_obj)
	$(CC) $(CFLAGS) -o test_media test_media.c $(test_media_obj) $(LFLAGS)

test_anemometer: $(anemometer_obj) init.o
	$(CC) $(CFLAGS) -o ../sint.elf test_anemometer.c $(anemometer_obj) init.o $(LFLAGS)

test_adc: $(adc_obj)
	$(CC) $(CFLAGS) -o ../sint.elf test_adc.c $(adc_obj) $(LFLAGS)

test_synth: $(synth_obj)
	$(CC) $(CFLAGS) -o ../sint.elf test_synth.c $(synth_obj) $(LFLAGS)

test_uart: $(objects)
	$(CC) $(CFLAGS) -o ../sint.elf test_uart.c $(objects) $(LFLAGS)

clean: $(SUBDIRS)
	$(REMOVE) $(objects)

indent:
	indent *.c *.h
