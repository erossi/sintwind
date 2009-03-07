BASEDIR = /home/enrico/avr/sintwind/opensint
SRCDIR = $(BASEDIR)/src
INC = -I /usr/avr/include -I $(SRCDIR)
LIBS = m
OPTLEV = 2
CFLAGS = $(INC) -Wall -Wstrict-prototypes -pedantic -O$(OPTLEV)
LFLAGS = -l$(LIBS)
CC = gcc

REMOVE = rm -f

objects = media.o

.PHONY: clean indent
.SILENT: help
.SUFFIXES: .c, .o

all: $(objects)
	$(CC) $(CFLAGS) -o test_media test_media.c $(objects) $(LFLAGS)

clean: $(SUBDIRS)
	$(REMOVE) $(objects)
