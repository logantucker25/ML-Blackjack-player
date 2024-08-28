/* deck.h — header file for deck module
 *
 * module for handling deck of 52 cards based on cards.h module
 * 
 * By Grace Turner for CS50 Final Project
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "deck.h"

/****************STRUCTURES***************/
typedef struct deck{
    card_t* deck_cards[52];
    int count;
} deck_t;

/****************FUNCTIONS****************/
/*************** create_deck *************/
/* See deck.h for description */
deck_t* create_deck(void){

    int num, suite;
    int card_count = 0;

    deck_t* my_deck = malloc(sizeof(deck_t));

    if (my_deck == NULL){
        return NULL;
    } else {

        for (suite = 0; suite < 4; suite++){        //cycle through each suite
            for (num = 0; num < 14; num++){   //cycle through each number (skip 11 for soft ace)
            
                if (num == 10){
                    num++;
                }

                card_t* my_card = create_card((num+1), (suite+1));   //+1 because num/suite = 0 is reserved for problem
                my_deck->deck_cards[card_count] = my_card;

                card_count++;
            }
        }
        my_deck->count = 52;    //set count of cards to 52
        return my_deck;
    }
}

/************** shuffle_deck *************/
/* See deck.h for description */
void shuffle_deck(deck_t* my_deck){
    int rand_num, i, n;
    card_t* shuffled_card;

    if (my_deck != NULL){       //ignore if deck is NULL
        for (i = 0; i < 1000; i++){
            rand_num = rand()%52;
            shuffled_card = my_deck->deck_cards[rand_num];

            for (n = rand_num; n > 0; n--){
                my_deck->deck_cards[n] = my_deck->deck_cards[n-1];
            }

            my_deck->deck_cards[0] = shuffled_card;
        }
    }
}

/************** draw_deck ****************/
/* See deck.h for description */
card_t* draw_deck(deck_t* my_deck){
    card_t* my_card;
    int i = 0;

    if ((my_deck == NULL)||(my_deck->count == 0)){  //if deck is NULL or out of cards
        return NULL;
    } else {
        my_card = my_deck->deck_cards[i];

        while (my_card == NULL){
            i++;
            my_card = my_deck->deck_cards[i];
        }

        card_t* card_to_play = create_card(get_card_num(my_card), get_card_suite(my_card));

        delete_card(my_card);
        my_deck->deck_cards[i] = NULL;
        my_deck->count--;

        return card_to_play;
    }
}

/************* get_deck_count ************/
/* See deck.h for description */
int get_deck_count(deck_t* my_deck){

    if (my_deck == NULL){
        return 0;
    } else {
        return my_deck->count;
    }
}

/************* print_deck ***************/
/* See deck.h for description */
void print_deck(deck_t* my_deck){
    int i;

    if (my_deck != NULL){
        for (i = 0; i < 52; i++){

            if (my_deck->deck_cards[i] != NULL){
                printf("Num: %3d of Suite: %3d\n", get_card_num(my_deck->deck_cards[i]), get_card_suite(my_deck->deck_cards[i]));
            } else {
                printf("-\n");
            }
        }
    }
}

/************* delete_deck ***************/
/* See deck.h for description */
void delete_deck(deck_t* my_deck){
    int i;

    if (my_deck != NULL){

        for (i = 0; i < 52; i++){
            if (my_deck->deck_cards[i] != NULL){
                delete_card(my_deck->deck_cards[i]);
            }
        }
        free(my_deck);
    }
}