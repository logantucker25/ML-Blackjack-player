/* hand.h — header file for hand module
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
typedef struct hand hand_t;

/****************FUNCTIONS****************/
/*************** create_hand *************/
/* Allocates memory for a hand
    - Can contain max 11 cards
    - Returns NULL if error
    - User is responsible for later calling delete_hand
*/
hand_t* create_hand();

/*************** insert_card *************/
/* Inserts a card into a hand
    - Card must already be created and space allocated
    - Once card is loaded into hand, user can delete card with delete_hand
    - Returns false if NULL parameter or if hand is full
    - Otherwise, returns true
*/
bool insert_card(card_t* my_card, hand_t* my_hand);

/************* get_hand_points ***********/
/* Get sum of points in the hand
    - User must enter a valid hand
    - Returns sum of all cards in hand
    - Returns 0 if hand is empty or if hand is NULL
*/
int get_hand_points(hand_t* my_hand);

/*************** print_hand *************/
/* Prints num and suite of cards in the hand
    - User must provide pointer to a valid hand
    - To be used for debugging
*/
void print_hand(hand_t* my_hand);

/*************** delete_hand *************/
/* Deletes hand and any cards contained inside
    - User must provie pointer to a valid hand
    - Deletes any cards loaded inside and frees associated pointers
    - Frees hand pointer
*/
void delete_hand(hand_t* my_hand);



