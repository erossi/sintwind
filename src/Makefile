MCU = atmega16
INC = -I/usr/avr/include/
LIBS = m
OPTLEV = 2
FCPU = 4000000UL
DFLAGS = -D F_CPU=$(FCPU) -D HAVE_DEFAULT
CFLAGS = $(INC) -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV) $(DFLAGS)
LFLAGS = -l$(LIBS)
CC = avr-gcc

PRG_NAME = sint
GIT_TAG = "Unknown"
# Uncomment if git tag is in use
GIT_TAG = "$(shell git describe --tags)"
PRGNAME = $(PRG_NAME)_$(GIT_TAG)

REMOVE = rm -f

init_obj = init.o
anemometer_obj = davis.o lacrosse.o anemometer.o adc.o
phone_obj = cell.o uart_isr.o uart.o 
synth_obj = sht11.o init.o adc.o anemometer.o synth.o davis.o
sht11_obj = sht11_avr_io.o sht11.o
objects = cell.o init.o synth.o
objects += media.o uart_isr.o uart.o utils.o debug.o
objects += $(anemometer_obj) $(sht11_obj)

.PHONY: clean indent
.SILENT: help
.SUFFIXES: .c, .o

all: $(objects)
	$(CC) $(CFLAGS) -o ../$(PRGNAME).elf main.c $(objects) $(LFLAGS)

debug.o:
	$(CC) $(CFLAGS) -D GITREL=\"$(GIT_TAG)\" -c debug.c

uart.o:
	$(CC) $(CFLAGS) -D TXRXI -c uart.c
	 
test_sht11: $(objects)
	$(CC) $(CFLAGS) -o ../$(PRGNAME).elf test_sht11.c $(objects) $(LFLAGS)

test_media: $(objects)
	$(CC) $(CFLAGS) -o ../$(PRGNAME).elf test_media.c $(objects) $(LFLAGS)

test_anemometer: $(anemometer_obj) init.o
	$(CC) $(CFLAGS) -o ../$(PRGNAME).elf test_anemometer.c $(anemometer_obj) init.o $(LFLAGS)

test_adc: $(adc_obj)
	$(CC) $(CFLAGS) -o ../$(PRGNAME).elf test_adc.c $(adc_obj) $(LFLAGS)

test_synth: $(synth_obj)
	$(CC) $(CFLAGS) -o ../$(PRGNAME).elf test_synth.c $(synth_obj) $(LFLAGS)

test_uart: $(objects)
	$(CC) $(CFLAGS) -o ../$(PRGNAME).elf test_uart.c $(objects) $(LFLAGS)

clean: $(SUBDIRS)
	$(REMOVE) $(objects)

indent:
	indent *.c *.h

version:
	# Last Git tag: $(GIT_TAG)
