# Makefile for 'cards', 'deck', 'hand' modules
# See testing directory for test script
#
# By Grace Turner for CS50 Final Project

OBJS = cards.o deck.o hand.o file.o
LIB = card_lib.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make

# Build $(LIB) by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files
cards.o: cards.h
deck.o: deck.h
hand.o: hand.h
file.o: file.h

.PHONY: clean

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o
