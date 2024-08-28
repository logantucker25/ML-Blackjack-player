# Makefile for 'network' modules
# Based off of a Makefile for TSE common.a library
#
# By Grace Turner for CS50 Final Project

OBJS = player_network.o dealer_network.o
LIB = network.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make

# Build $(LIB) by archiving object files
$(LIB): $(OBJS) ../card_lib/card_lib.a
	ar cr $(LIB) $(OBJS) ../card_lib/card_lib.a

# Dependencies: object files depend on header files
player_network.o: player_network.h
dealer_network.o: dealer_network.h

.PHONY: clean

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o