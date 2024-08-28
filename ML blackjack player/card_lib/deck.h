/* deck.h — header file for deck module
 *
 * module for handling deck of 52 cards based on cards.h module
 * 
 * By Grace Turner for CS50 Final Project
 */

#include <stdio.h>
#include <stdlib.h>
#include "cards.h"

/****************STRUCTURES***************/
typedef struct deck deck_t;

/****************FUNCTIONS****************/
/*************** create_deck *************/
/* Allocates memory for a deck and fills with 52 cards of 4 suites
    - Originally in order
    - No cards have num 11 (reserved for soft ace)
    - Returns NULL if error
    - User is responsible for later calling delete_deck
*/
deck_t* create_deck(void);

/************** shuffle_deck *************/
/* Shuffles the deck provided randomly
    - User provides pointer to valid deck
    - Returns nothing, but deck provided is changed
    - Does nothing if deck provided is NULL
*/
void shuffle_deck(deck_t* my_deck);

/************** draw_deck ****************/
/* Removes top card from the deck and returns it
    - User must provide pointer to valid deck
    - Makes a copy of the top card from the deck
    - Deletes top card, NULLs the spot in the array
    - User is responsible for later calling delete_card on returned card
    - Returns NULL if deck is NULL or if deck is empty
*/
card_t* draw_deck(deck_t* my_deck);

/************* get_deck_count ************/
/* Returns the number of cards in the deck
    - User must provide pointer to a valid deck
    - Returns 0 if not a valid pointer or if deck is empty
*/
int get_deck_count(deck_t* my_deck);

/************* print_deck ***************/
/* Prints num and suite of cards in the deck
    - User must provide pointer to a valid deck
    - To be used for debugging
*/
void print_deck(deck_t* my_deck);

/************* delete_deck ***************/
/* Deletes any cards left in a deck, as well as deck itself
    - User must provide valid deck
    - Frees associated pointers
    - Does nothing if deck is NULL
*/
void delete_deck(deck_t* my_deck);