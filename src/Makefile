# Copyright (C) 2011 Enrico Rossi
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

PRG_NAME = osint
MCU = atmega16
OPTLEV = 2
FCPU = 4000000UL
PWD = $(shell pwd)
INC = -I/usr/lib/avr/include/

CFLAGS = $(INC) -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV) -D F_CPU=$(FCPU)
LFLAGS = -lm

PRGNAME = $(PRG_NAME)
GIT_TAG = "Unknown"
# Uncomment if git tag is in use
GIT_TAG = "$(shell git describe --tags)"
PRGNAME = $(PRG_NAME)_$(GIT_TAG)

AR = avr-ar
CC = avr-gcc

DUDEPORT = /dev/ttyUSB0
DUDEDEV = stk500v2
DUDEPORT = usb
DUDEDEV = avrispmkII
# Use sudo for USB avrispmkII
DUDE = sudo avrdude -c $(DUDEDEV) -p $(MCU) -P $(DUDEPORT) -e -U flash:w:$(PRGNAME).hex

OBJCOPY = avr-objcopy -j .text -j .data -O ihex
OBJDUMP = avr-objdump
SIZE = avr-size --format=avr --mcu=$(MCU) $(PRGNAME).elf

REMOVE = rm -f

init_obj = init.o
anemometer_obj = davis.o lacrosse.o anemometer.o adc.o
phone_obj = cell.o uart.o 
synth_obj = sht11.o init.o adc.o anemometer.o synth.o davis.o
sht11_obj = sht11_avr_io.o sht11.o
objects = cell.o init.o synth.o
objects += media.o uart.o utils.o debug.o
objects += $(anemometer_obj) $(sht11_obj)

.PHONY: clean indent
.SILENT: help
.SUFFIXES: .c, .o

all: $(objects)
	$(CC) $(CFLAGS) -o $(PRGNAME).elf main.c $(objects) $(LFLAGS)
	$(OBJCOPY) $(PRGNAME).elf $(PRGNAME).hex

debug.o:
	$(CC) $(CFLAGS) -D GITREL=\"$(GIT_TAG)\" -c debug.c

uart.o:
	$(CC) $(CFLAGS) -D TXRXI -c uart.c
	 
program:
	$(DUDE)

clean:
	$(REMOVE) *.elf *.hex $(objects)

version:
	# Last Git tag: $(GIT_TAG)

doc:
	cd ../doc && ( cat Doxyfile ; echo "PROJECT_NUMBER=$(GIT_TAG)" ) | doxygen -

size:
	$(SIZE)

# Not sure these are still working
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
