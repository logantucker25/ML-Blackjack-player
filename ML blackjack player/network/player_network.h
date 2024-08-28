/* player.h — header file for network module
 *
 * module for handling player's network interactions for game
 * 
 * By Sookyoung Park for CS50 Final Project
 */

#include <stdio.h>
#include <stdbool.h>
#include <netinet/in.h>
#include "../card_lib/cards.h"


/****************STRUCTURES***************/
typedef struct heard_info heard_info_t;

/****************FUNCTIONS****************/
/************** begin_game **************/
/* Connect player to network 
    - Creating socket
    - Setting up socket
    - Send a concatenate string : "JOIN playername"
    - print the succeed msg
    - free the buffer
    - Return the socket number
*/
int begin_game(char* playername);


/************** player_listen **************/
/* Convert a string in the file to int
    - If "BEGIN" or "begin", info->command = 1
    - If "CARD" or "card", info->command =2
    - If "DEALER" or "dealer", info->command=3
    - If "DECISION" or "decision", info->command=4
    - If "RESULT" or "result", info->command =5
    
    - If info->command 3 or 4 :
        * Convert a card info from string to int
        * save these converted ints to create_card()

    - If info->commannd 5:
        * convert result from string to int
        * If win -> info->result =1
        * If lose -> info->result =2
        * If push -> info->result =3
    
    - return structure
*/
heard_info_t* player_listen(int socket_number, char* queue);


/************** send_decision **************/
/* Convert decision from int to string and send the string to the dealer
    - If decision ==1:
        * send HIT to the dealer
    - If decision ==2:
        * send STAND to the dealer
    - Return false if error, otherwise return true
*/
bool send_decision(int socket_number, int decision);


/******************** end_game ****************/
/* Quit the game
    - Close the socket
    - Shutdown the server
    - print the message "Quit Game"
    - If succeed, return true
*/
bool end_game(int socket_number);

/************** get_heard_command ***************/
/* Returns command integer of provided information structure
    - User must provide a valid heard_info_t
    - If info is NULL, 0 is returned
*/
int get_heard_command(heard_info_t* info);

/************** get_heard_card ***************/
/* Returns card structure from provided information structure
    - User must provide a valid heard_info_t
    - If info or card is NULL, NULL is returned
*/
card_t* get_heard_card(heard_info_t* info);

/************** get_heard_result***************/
/* Returns result from provided information structure
    - User must provide a valid heard_info_t
    - If info is NULL, 0 is returned
*/
int get_heard_result(heard_info_t* info);

/************** delete_heard_info ***************/
/* If info is not NULL, deletes the structure and contained info
    - Calls delete_card()
    - frees info
*/
void delete_heard_info(heard_info_t* info);

/************** print_heard_info ***************/
/* Prints the information in the heard_info_t structure to stdout
    - User must provide a valid heard_info_t
    - For testing/debugging
*/
void print_heard_info(heard_info_t* info);
