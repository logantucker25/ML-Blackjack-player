# Makefile for project testing scripts
#

C = ../card_lib
N = ../network
LIBS = -lm
LLIBS = $C/card_lib.a $N/network.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$N -I$C
CC = gcc
MAKE = make

# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: all valgrind clean

########### default ##################

all: test_cards test_network test_player_network

########### test cards ###############
test_cards: test_cards.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) $(LLIBS) -o $@

test_cards.o: test_cards.c
test_cards.o: $C/cards.h $C/deck.h $C/hand.h

########## test network ##############
test_network: test_network.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) $(LLIBS) -o $@

test_network.o: test_network.c
test_network.o: $C/cards.h $C/deck.h $C/hand.h
test_network.o: $N/dealer_network.h $N/player_network.h

########## test player network ##############
test_player_network: test_player_network.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) $(LLIBS) -o $@

test_player_network.o: test_player_network.c
test_player_network.o: $C/cards.h $C/deck.h $C/hand.h
test_player_network.o: $N/dealer_network.h $N/player_network.h

########### phonies ##################

valgrind:
	$(VALGRIND) ./test_cards

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f test_cards
	rm -f test_network
	rm -f test_player_network
	rm -f core
