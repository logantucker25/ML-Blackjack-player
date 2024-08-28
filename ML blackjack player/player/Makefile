# Makefile for player
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

all: player

########### player ###############
player: player.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) $(LLIBS) -o $@


########### phonies ##################
clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f player
	rm -f file
	rm -f core
