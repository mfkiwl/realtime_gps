# Makefile for Linux etc.

.PHONY: all clean time
all: rt_gpssim

SHELL=/bin/bash
CC=gcc
CFLAGS=-O3 -Wall
LDFLAGS=-lm -pthread

gps-sdr-sim: gpssim.o
	${CC} $< ${LDFLAGS} -o $@

rt_gpssim: gpssim.o
	${CC} $< ${LDFLAGS} -o $@


clean:
	rm -f gpssim.o gps-sdr-sim  rt_gpssim *.bin

time: gps-sdr-sim
	time ./gps-sdr-sim -e brdc3540.14n -u circle.csv -b 1
	time ./gps-sdr-sim -e brdc3540.14n -u circle.csv -b 8
	time ./gps-sdr-sim -e brdc3540.14n -u circle.csv -b 16

