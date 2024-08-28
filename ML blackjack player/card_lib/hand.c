/* hand.c — function file for hand module
 *
 * module for handling a hand of up to 11 cards
 * 
 * By Grace Turner for CS50 Final Project
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cards.h"

/****************STRUCTURES***************/
typedef struct hand {
    card_t* hand_cards[11];     //maximum number of cards in a hand is 11
    int points_sum;
} hand_t;

/****************FUNCTIONS****************/
/*************** create_hand *************/
hand_t* create_hand(){

    hand_t* my_hand = malloc(sizeof(hand_t));

    if (my_hand == NULL) {
        return NULL;              // error allocating new hand
    } else {
        int i;
        for (i = 0; i < 11; i++){
            my_hand->hand_cards[i] = NULL;
        }

        my_hand->points_sum = 0;
        return my_hand;
  }
}

/*************** insert_card *************/
bool insert_card(card_t* my_card, hand_t* my_hand){
    int i;

    if ((my_hand == NULL) || (my_card == NULL)){    //if NULL, return 0
        return 0;
    }

    for (i = 0; i < 11; i++){       //start at first card and move through the array
        if (my_hand->hand_cards[i] == NULL){    //if slot is empty, put card in it!
            my_hand->hand_cards[i] = my_card;

            my_hand->points_sum += get_card_value(my_card); //add value to total sum

            return 1;           //will return 1 after card is entered once
        }
    }

    return 0;   //return 0 if all 11 cards are full
}

/************* get_hand_points ***********/
int get_hand_points(hand_t* my_hand){

    if (my_hand != NULL){
        return my_hand->points_sum;
    } else {
        return 0;       //returns 0 if NULL or if hand is empty
    }
}

/*************** print_hand *************/
void print_hand(hand_t* my_hand){
    int i;

    if (my_hand != NULL){
        for (i = 0; i < 11; i++){

            if (my_hand->hand_cards[i] != NULL){
                printf("Num: %3d of Suite: %3d\n", get_card_num(my_hand->hand_cards[i]), get_card_suite(my_hand->hand_cards[i]));
            } else {
                printf("-\n");
            }
        }
    }
}

/*************** delete_hand *************/
void delete_hand(hand_t* my_hand){
    int i;

    if (my_hand != NULL){

        for (i = 0; i < 11; i++){
            if (my_hand->hand_cards[i] != NULL){
                delete_card(my_hand->hand_cards[i]);
            }
        }
        
        free(my_hand);
    }
}