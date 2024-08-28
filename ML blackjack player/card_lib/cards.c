/* cards.c — function file for cards module
 *
 * module for handling individual cards
 * 
 * By Grace Turner for CS50 Final Project
 */

#include <stdio.h>
#include <stdlib.h>
#include "cards.h"

/**************** Structure **************/
typedef struct card {
    int number;
    int suite;
} card_t;

/************** create_card **************/
/* See card.h for description */
card_t* create_card(int number_in, int suite_in){

    card_t* my_card = malloc(sizeof(card_t));

    if (my_card == NULL) {
        return NULL;              // error allocating new card
    } else {

        my_card->number = number_in;    //initialize values
        my_card->suite = suite_in;
        return my_card;
  }
}

/************** get_card_num **************/
int get_card_num(card_t* my_card){

    if (my_card == NULL){
        return 0;      //0 means bad card
    } else {
        int num = my_card->number;
        return num;
    }
}

/************** get_card_suite ************/
int get_card_suite(card_t* my_card){

    if (my_card == NULL){
        return 0;       //0 means bad card
    } else {
        int suite = my_card->suite;
        return suite;
    }
}

/************** get_card_value ************/
/* See card.h for description */
int get_card_value(card_t* my_card){
    int value = 0;             //returns 0 if card is null

    if (my_card != NULL){
        value = my_card->number;    //set value to number
    }

    if (value > 11){    //number > 11 signifies a face card
        value = 10;
    }

    return value;
}

/**************** delete_card *************/
/* See card.h for description */
void delete_card(card_t* my_card){

    if (my_card != NULL) {
        free(my_card);
    }
}