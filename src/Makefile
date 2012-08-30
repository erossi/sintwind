# Copyright (C) 2012 Enrico Rossi
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
MCU = atmega1284p
OPTLEV = s
FCPU = 1000000UL
PWD = $(shell pwd)
INC = -I/usr/lib/avr/include/ -Isht11/

CFLAGS = $(INC) -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV) -D F_CPU=$(FCPU)
LFLAGS = -lm

PRGNAME = $(PRG_NAME)
GIT_TAG = "Unknown"
# Uncomment if git tag is in use
GIT_TAG = "$(shell git describe --tags)"
PRGNAME = $(PRG_NAME)_$(GIT_TAG)_$(MCU)

AR = avr-ar
CC = avr-gcc

DUDESPORT = /dev/ttyUSB0
DUDESDEV = stk500v2
DUDEUPORT = usb
DUDEUDEV = avrispmkII
# Use sudo for USB avrispmkII
DUDE = sudo avrdude -p $(MCU) -e -U flash:w:$(PRGNAME).hex

OBJCOPY = avr-objcopy -j .text -j .data -O ihex
OBJDUMP = avr-objdump
SIZE = avr-size --format=avr --mcu=$(MCU) $(PRGNAME).elf

REMOVE = rm -f

init_obj = init.o
anemometer_obj = davis.o lacrosse.o anemometer.o adc.o
phone_obj = cell.o uart.o 
synth_obj = sht11.o init.o adc.o anemometer.o synth.o davis.o
sht11_obj = sht11/sht11_io.o sht11/sht11.o
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
	 
programstk:
	$(DUDE) -c $(DUDESDEV) -P $(DUDESPORT)

program:
	$(DUDE) -c $(DUDEUDEV) -P $(DUDEUPORT)

clean:
	$(REMOVE) *.elf *.hex $(objects)

version:
	# Last Git tag: $(GIT_TAG)

doc:
	cd ../ && ( cat dox/Doxyfile ; echo "PROJECT_NUMBER=$(GIT_TAG)" ) | doxygen -

size:
	$(SIZE)

