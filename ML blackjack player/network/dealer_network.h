/* dealer.h — header file for network module
 *
 * module for handling dealer's network interactions for game
 * 
 * By Sookyoung Park for CS50 Final Project
*/

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include <strings.h>

#include "../card_lib/cards.h"

/********************FUNCTIONS*********************/

/************ connect_dealer_network *********/
/* Connect dealer to network 
    - Setting up socket
    - Connecting to port
    - Binding to port
    - Listening 
    - Accepting
    - print the messege, if the setting up is done correctly
    - If error, EXIT_FAILURE. Otherwise, return socket number
*/
int connect_dealer_network(void);


/****************** send_begin *****************/
/* Send JOIN and BEGIN to the player
    - Read the messege provided by the socket of socket number
    - Check the file content if "join" or "JOIN" in the file that player sent
    - If JOIN is in the file, prase a player name in the file
    - Send BEGIN to player
    - Function return NULL if error, otherwise reuturn a player name
*/
char* send_begin(int new_socket);


/**************** send_card *********************/
/* Send concatenate string to the player 
    - Concatenate string "CARD" and the string of return value of translate_card
    - Send the concatenate string to the player
    - Function return FALSE if error, otherwise reuturn TRUE
*/
bool send_card(int new_socket,card_t* my_card);


/*************** send_dealer_card **************/
/* Send concatenate string to the player 
    - Concatenate string "DEALER" and the string of return value of translate_card
    - Send the concatenate string to the player
    - Function return FALSE if error, otherwise reuturn TRUE
*/
bool send_dealer_card(int new_socket, card_t* my_card);


/************** prompt_decision **************/
/* Read the decision for the player and convert his/her decision to int 
    - Send "DECISION" to the player
    - Read the string, the decision player made
    - If there's the string "HIT" in the file, return 1, the string "STAND" in the file, return 2
      return 0 if error.
*/
int prompt_decision(int new_socket);


/*************** send_result ******************/
/* Convert the result from int to string and send it to the player
    - If result == 1, send "WIN" to the player
    - If result == 2, send "lOSE" to the player
    - If result == 3, send "PUSH" to the player
    - If succeed, return true. Return 0 if error
*/
bool send_result(int new_socket, int result);


/**************** quit_game *******************/
/* Quit the game
    - Close the socket
    - Shutdown the server
    - print the message "Quit Game"
    - If succeed, return true
*/
bool quit_game(int new_socket);


/************* LOCAL FUNCTIONS ****************/
/* These functions do not leave this module */

/************* translate_card ******************/
/* Convert card info from int to string
    - Read the int card info from get_card_num() and get_card_suite()
    - suite
        * 1: Heart
        * 2: Diamond
        * 3: Spade
        * 4: Club
    - num
        * 1: one
        * 2: two
        ...
        * 11: ace
        * 12: jack
        * 13: queen
        * 14: king
    - Concatenate three strings (ex. Three of Heart / Five of Club)
    - Function return NULL if error, otherwise reuturn a string of the card info
    */
char* translate_card(card_t* mycard);
