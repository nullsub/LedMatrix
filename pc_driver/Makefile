CC = gcc
CFLAGS  = -g -Wall -O2 -std=gnu99 -Isrc 
CFLAGS += $(shell pkg-config fftw3f --cflags)
CFLAGS += $(shell pkg-config libpulse --cflags)
LIBS  = -lrt -pthread -lfftw3
LIBS += $(shell pkg-config fftw3f --libs)
LIBS += $(shell pkg-config libpulse --libs)

HEADERS = src/monitor.h
SRC_FILES = $(wildcard src/*.c) $(wildcard ccan/*/*.c)
OJB_FILES = $(SRC_FILES:.c=.o)

all: imonpulse

%.o: %.c Makefile $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

imonpulse: $(OJB_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

install: imonpulse
	install imonpulse /usr/local/bin

clean:
	$(RM) $(OJB_FILES) imonpulse

.PHONY: all install clean
