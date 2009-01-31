MCU = atmega16
INC = -I/usr/avr/include/
LIBS = m
OPTLEV = s
export CFLAGS = $(INC) -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV)
export LFLAGS = -l$(LIBS)
export CC = avr-gcc

OBJCOPY = avr-objcopy -j .text -j .data -O ihex
OBJDUMP = avr-objdump
SIZE = avr-size
DUDE = avrdude -c stk500v1 -p $(MCU) -P /dev/ttyUSB0 -e -U flash:w:sint.hex
REMOVE = rm -f
objects =
SUBDIRS = src

.PHONY: clean indent $(SUBDIRS)
.SILENT: help
.SUFFIXES: .c, .o

all: $(SUBDIRS)
	$(OBJCOPY) sint.elf sint.hex

program:
	$(DUDE)

$(SUBDIRS):
	@echo $(MAKECMDGOALS)
	$(MAKE) -C $@ $(MAKECMDGOALS)

test_sht11: $(SUBDIRS)
	$(OBJCOPY) sint.elf sint.hex

test_media: $(SUBDIRS)
	$(OBJCOPY) sint.elf sint.hex

test_anemometer: $(SUBDIRS)
	$(OBJCOPY) sint.elf sint.hex

test_adc: $(SUBDIRS)
	$(OBJCOPY) sint.elf sint.hex

test_synth: $(SUBDIRS)
	$(OBJCOPY) sint.elf sint.hex

test_uart: $(SUBDIRS)
	$(OBJCOPY) sint.elf sint.hex

clean: $(SUBDIRS)
	$(REMOVE) sint.elf sint.hex

indent: $(SUBDIRS)
	indent *.c *.h

