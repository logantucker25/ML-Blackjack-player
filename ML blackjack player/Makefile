# Makefile for Project 13 Final Project
#
# By Grace Turner for CS50 Final Project

.PHONY: all clean

########### default ###########
all: 
	make -C card_lib
	make -C network
	make -C tests
	make -C dealer
	make -C player

############## clean  ##########
clean:
	rm -f *~
	make -C card_lib clean
	make -C network clean
	make -C tests clean
	make -C dealer clean
	make -C player clean