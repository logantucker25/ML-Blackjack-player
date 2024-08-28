/* cards.h — header file for cards module
 *
 * module for handling individual cards
 * 
 * By Grace Turner for CS50 Final Project
 */

#include <stdio.h>
#include <stdlib.h>

/****************STRUCTURES***************/
typedef struct card card_t;

/****************FUNCTIONS****************/
/************** create_card **************/
/* Allocates memory and intializes card based on given info
    - User must provide valid card number (1-14) and suite (1-4)
    - User is responsible for calling delete_card
    - Function returns a pointer to a card initialized with values
    - Returns NULL if memory allocation error
*/
card_t* create_card(int number_in, int suite_in);

/************** get_card_num **************/
/* Returns the number on the card
    - User must input a valid card
    - Returns 0 if card is NULL
*/
int get_card_num(card_t* my_card);

/************** get_card_suite ************/
/* Returns the suite on the card
    - User must input a valid card
    - Returns 0 if card is NULL
*/
int get_card_suite(card_t* my_card);

/************* get_value_card ************/
/* Returns the value of the card
    - User must provide pointer to valid card
    - Function returns integer value of the card
        Card numbers <= 11 are valued by number
        Card numbers > 11 are 10
    - Card is unchanged by this function
    - If card is NULL, returns 0
*/
int get_card_value(card_t* my_card);

/************* delete_card **************/
/* Deletes card
    - User must provide valid card
    - Frees associated pointers
    - Returns nothing
*/
void delete_card(card_t*);