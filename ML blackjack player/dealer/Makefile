# Makefile for 'network' modules
# Based off of a Makefile for TSE common.a library
#
# By Will Elliott for CS50 Final Project

C = ../card_lib
N = ../network
LIBS = -lm
LLIBS = $C/card_lib.a $N/network.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$N -I$C
CC = gcc
MAKE = make

# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: clean

########### default ##################

dealerModule: dealerModule.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) $(LLIBS) -o $@


test: dealerModule
	./dealerModule 3

valgrind:
	$(VALGRIND) ./dealerModule 3

########### phonies ##################

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f dealerModule
	rm -f core
