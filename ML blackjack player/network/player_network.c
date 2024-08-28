/* player.c — function file for network module
 *
 * module for handling player's network interactions for game
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
#include <ctype.h>
#include <arpa/inet.h>
#include <time.h>

#include "../card_lib/cards.h"

/***************** STRUCTURES ******************/
typedef struct heard_info{
    int command;
    card_t* card;
    int result;
} heard_info_t;

/*************** LOCAL FUNCTIONS ****************/
int nanosleep(const struct timespec *req, struct timespec *rem);

/************** connect_network *****************/
/* See player_network.h for description */
int begin_game(char* playername){

    const int PORT = 8093;    //setting up the socket
    int client_fd;
    struct sockaddr_in serv_addr;
    
    int new_socket;

    printf("    Creating socket...\n");
    if ((new_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("    Socket creation error\n");
        return -1;
    }
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {    //setting up address
        perror("\n  Invalid address/Address not supported \n");
        return -1;
    }
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    printf("    Setting up socket...\n");   //setting up the socket
    if ((client_fd = connect(new_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\n  Connection Failed \n");
        return -1;
    }

    printf("    Joining game...\n");    //send join
    char join[100];
    snprintf(join, strlen(playername)+6, "JOIN %s", playername);

    send(new_socket, join, strlen(join)+1, 0);
    printf("    Message Sent: %s\n", join);
    nanosleep((const struct timespec[]){{0, 26000000l}},NULL);

    char buffer[1024] = { 0 };
    char* begin_l="begin";
    char* begin_u="BEGIN";

    do {
        read(new_socket, buffer, 1024);
    } while (strstr(buffer,begin_u) == NULL && strstr(buffer,begin_l) == NULL);
    printf("    Message Recieved: %s\n", buffer);

    printf("    Game beginning...\n");

    return new_socket;
}

/************** player_listen *****************/
/* See player_network.h for description */
heard_info_t* player_listen(int socket_number, char* queue){

    char buffer[1024] = { 0 };      //allocate and initialize the structures
    heard_info_t* info = malloc(sizeof(heard_info_t));
    info->command = 0;
    info->card = NULL;
    info->result = 0;

    do {
        read(socket_number, buffer, 1024);
    } while (buffer == NULL);
    printf("    Message Recieved: %s\n", buffer);
    
    char* begin_l="begin";          //compare strings
    char* begin_u="BEGIN";

    char* card_l="card";
    char* card_u="CARD";
    
    char* dealer_l="dealer";
    char* dealer_u="DEALER";

    char* decision_l="decision";
    char* decision_u="DECISION";

    char* result_l="result";
    char* result_u="RESULT";

    char* quit_l="quit";
    char* quit_u="QUIT";

    if ((strncmp(buffer,begin_u, 3)) == 0 || (strncmp(buffer,begin_l, 3)) == 0 ) {  //Compare commands to strings
        info->command = 1;  //BEGIN means 1
        if (strlen(buffer) > 6){
            strncpy(queue, &buffer[6],100);
        }
    }
    else if((strncmp(buffer,card_u, 3)) == 0  || (strncmp(buffer,card_l, 3)) == 0 ){
        info->command = 2;  //CARD means 2
    }
    else if((strncmp(buffer,dealer_u, 3)) == 0  || (strncmp(buffer,dealer_l, 3)) == 0 ){
        info->command = 3;  //DEALER means 3
    }
    else if((strncmp(buffer,decision_u, 3)) == 0 || (strncmp(buffer,decision_l, 3)) == 0  ){
        info->command = 4;  //DECISION means 4
        if (strlen(buffer) > 9){
            strncpy(queue, &buffer[9],100);
        }
    }
    else if((strncmp(buffer,result_u, 3)) == 0 || (strncmp(buffer,result_l, 3)) == 0 ){
        info->command = 5;  //RESULT means 5
    } 
    else if ((strncmp(buffer,quit_u, 3)) == 0 || (strncmp(buffer,quit_l, 3)) == 0 ){
        info->command = 6;  //QUIT means 6
        if (strlen(buffer) > 5){
            strncpy(queue, &buffer[5],100);
        }
    }

    if( (info->command == 2) || (info->command == 3) ){     //if command is CARD or DEALER, need to return card

        char card_arr[5][100];  //tokenize the string into arrays
        int newWord = 0;
        int wordNum = 0;
        int wordCharNum = 0;

        for(int charNum = 0; charNum < strlen(buffer); charNum++){
            
            if (isspace(buffer[charNum]) != 0) {
                if (newWord == 1){
                    card_arr[wordNum][wordCharNum] = 0; //null terminate the string

                    wordNum++;          //first space char means shift to new word slot in array

                    wordCharNum = 0;    //reset to beginning of word
                    newWord = 0;        //only do this once between words
                }
            } else {
                card_arr[wordNum][wordCharNum] = buffer[charNum];
                wordCharNum++;

                newWord = 1;    //new word flag for when the word is done
            }
        }

        card_arr[wordNum][wordCharNum] = 0; //null terminate the last string

        char* heart = "Heart";  //words to compare to
        char* diamond = "Diamond";
        char* spade = "Spade";
        char* club = "Club";

        char* ace = "Ace";
        char* two = "Two";
        char* three = "Three";
        char* four = "Four";
        char* five = "Five";
        char* six = "Six";
        char* seven = "Seven";
        char* eight = "Eight";
        char* nine = "Nine";
        char* ten = "Ten";
        char* jack = "Jack";
        char* king = "King";
        char* queen = "Queen";

        int player_num, player_suite;

        if(strncmp(card_arr[3], heart, 4) == 0){    //determine card suite
            player_suite=1;
        } else if(strncmp(card_arr[3], diamond, 4) == 0){
            player_suite=2;
        } else if(strncmp(card_arr[3], spade, 4) == 0){
            player_suite=3;
        } else if(strncmp(card_arr[3], club, 4) == 0){
            player_suite=4;
        }
            
        if(strncmp(card_arr[1],ace,2) == 0){    //determine card num
            player_num = 1;
        } else if(strncmp(card_arr[1],two,2) == 0){
            player_num = 2;
        } else if(strncmp(card_arr[1],three,2) == 0){
            player_num=3;
        } else if(strncmp(card_arr[1],four,2) == 0){
            player_num=4;
        } else if(strncmp(card_arr[1],five,2) == 0){
            player_num=5;
        } else if(strncmp(card_arr[1],six,2) == 0){
            player_num=6;
        } else if(strncmp(card_arr[1],seven,2) == 0){
            player_num=7;
        } else if(strncmp(card_arr[1],eight,2)== 0){
            player_num=8;
        } else if(strncmp(card_arr[1],nine,2) == 0){
            player_num=9;
        } else if(strncmp(card_arr[1],ten,2) == 0){
            player_num=10;
        } else if(strncmp(card_arr[1],jack,2) == 0){
            player_num=12;
        } else if(strncmp(card_arr[1],queen,2) == 0){
            player_num=13;
        } else if(strncmp(card_arr[1],king,2) == 0){
            player_num=14;
        }

        if (wordNum > 3){
            for (int count = 4; count < wordNum; count++){
                strcat(queue, card_arr[count]);
            }
        }

        card_t* player_card = create_card(player_num,player_suite);     //save as a new card
        info->card = player_card;   //save card in info structure

    }

    if(info->command == 5){     //if command recieved is result
        
        char result_arr[5][100];       //tokenize the string
        int newWord = 0;
        int wordNum = 0;
        int wordCharNum = 0;

        for(int charNum = 0; charNum < strlen(buffer); charNum++){
            
            if (isspace(buffer[charNum]) != 0) {
                if (newWord == 1){
                    result_arr[wordNum][wordCharNum] = 0; //null terminate the string

                    wordNum++;          //first space means shift to new word slot in array

                    wordCharNum = 0;    //reset to beginning of word
                    newWord = 0;        //only do this once between words
                }
            } else {
                result_arr[wordNum][wordCharNum] = buffer[charNum];
                wordCharNum++;

                newWord = 1;    //new word flag for when the word is done
            }
        }

        result_arr[wordNum][wordCharNum] = 0; //null terminate the last string

        char* win_u = "WIN";    //words to compare to
        char* win_l = "win";

        char* lose_u = "LOOSE";
        char* lose_l = "loose";

        char* push_u = "PUSH";
        char* push_l = "push";

        char* bust_u = "BUST";
        char* bust_l = "bust";

        if( ((strncmp(result_arr[1],win_u,3) == 0) || (strncmp(result_arr[1],win_l,3) == 0))){
            info->result = 1;
        } else if((strncmp(result_arr[1],lose_u,3) == 0)  || (strncmp(result_arr[1],lose_l,3) == 0) ){
            info->result = 2;
        } else if((strncmp(result_arr[1],push_u,3) == 0) || (strncmp(result_arr[1],push_l,3) == 0) ){
            info->result = 3;
        } else if((strncmp(result_arr[1],bust_u,3) == 0) || (strncmp(result_arr[1],bust_l,3) == 0) ){
            info->result = 2;
        }

        if (wordNum > 3){
            for (int count = 2; count < wordNum; count++){
                strcat(queue, result_arr[count]);
                strcat(queue, " ");
            }
        }
    }

    return info;    //return
}

/************** send_decision *****************/
/* See player_network.h for description */
bool send_decision(int socket_number, int decision){

    char* hit = "HIT";
    char* stand = "STAND";

    if (decision == 1) {
        send(socket_number, hit, strlen(hit)+1, 0);
        printf("    Message Sent: %s\n", hit);
        nanosleep((const struct timespec[]){{0, 26000000l}},NULL);
        return true;

    } else if(decision==2){
        send(socket_number, stand, strlen(stand)+1, 0);
        printf("    Message Sent: %s\n", stand);
        nanosleep((const struct timespec[]){{0, 26000000l}},NULL);
        return true;

    } else {
        return false;
    }
}

/************** end_game *****************/
/* See player_network.h for description */
bool end_game(int socket_number){

    printf("    Closing socket...\n");
    close(socket_number);   // closing the connected socket
    printf("    Game over\n");

    return true;
}

/************** delete_heard_info *****************/
/* See player_network.h for description */
void delete_heard_info(heard_info_t* info){

    if (info != NULL){              // if pointer is not NULL
        free(info);                 // info
    }
}

/************** print_heard_info *****************/
/* See player_network.h for description */
void print_heard_info(heard_info_t* info){

    printf("Command: %d\n", info->command);
    
    if (info->card != NULL){
        printf("Card sent: %d of %d\n", get_card_num(info->card), get_card_suite(info->card));
    } else {
        printf("No card sent\n");
    }

    printf("Result: %d\n", info->result);

}

/************** get_heard_command *****************/
/* See player_network.h for description */
int get_heard_command(heard_info_t* info){
    if (info != NULL){
        return info->command;
    } else {
        return 0;
    }
}

/************** get_heard_card *****************/
/* See player_network.h for description */
card_t* get_heard_card(heard_info_t* info){
    if (info != NULL){
        return info->card;
    } else {
        return NULL;
    }
}

/************** get_heard_result *****************/
/* See player_network.h for description */
int get_heard_result(heard_info_t* info){
    if (info != NULL){
        return info->result;
    } else {
        return 0;
    }
}