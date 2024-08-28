/* dealer.c — function file for network module
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
#include <time.h>

#include "../card_lib/cards.h"


/*************** LOCAL FUNCTIONS ****************/
char* translate_card(card_t* mycard);
int nanosleep(const struct timespec *req, struct timespec *rem);

/************** connect_network *****************/
int connect_dealer_network(void){
     printf("Entered connect_network!\n");
    const int PORT = 8093;

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int num_players = 1;

    // setting up socket
    printf("    Setting up socket\n");
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("    socket failed");
        exit(EXIT_FAILURE);
    }

    //connecting to port 
    printf("    Connecting to port...\n");
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("    setsockopt");
        exit(EXIT_FAILURE);
    }

    //binding to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    printf("    Binding to port...\n");
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("    bind failed");
        exit(EXIT_FAILURE);
    }

    //listening
    printf("    Listening...\n");
    if (listen(server_fd, num_players) < 0) {
        perror("    listen");
        exit(EXIT_FAILURE);
    }

    //accepting
    printf("    Accepting...\n");
    fflush(stdout); //ensure line above prints before waiting for connection
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        printf("\n");
        perror("    accept");
        exit(EXIT_FAILURE);
    }

    printf("    Done setting up socket!\n");
    return new_socket;
}

/************** send_begin *****************/
char* send_begin(int new_socket){

    int valread;
    char buffer[1024] = { 0 };  //need malloc to be added
 //   char* buffer=malloc(sizeof(char)*1024);

    valread = read(new_socket, buffer, 1024);   //read what is returned from player
    printf("    Message Recieved: %s\n", buffer);
    
    if (valread < 0) {
        perror("    Error reading message");      //if failure when reading
        return NULL;
    }
    
    char* upper_join = "JOIN";  //create strings for comparison
    char* lower_join = "join";
    char* begin = "BEGIN ";

    if( (strstr(buffer,upper_join) != NULL) || (strstr(buffer,lower_join) != NULL)){  //if recieve JOIN
        send(new_socket,begin,strlen(begin),0);     //send BEGIN
        nanosleep((const struct timespec[]){{0, 30000000l}},NULL);
    }

    return "TestGame21";
}

/************** send_card *****************/
bool send_card(int new_socket, card_t* my_card){

    char card_string[30];
    char* card_s = translate_card(my_card);

    if (card_s == NULL){    //if translating function failed
        return false;
    }

    snprintf(card_string, 30, "CARD %s", card_s);   //concatenate strings
    printf("    Sending message: %s\n", card_string);

    int out = send(new_socket, card_string, strlen(card_string),0);
    nanosleep((const struct timespec[]){{0, 26000000l}},NULL);

    if (out < 0) {      //check that sending worked
        return false;
    }

    free(card_s);
    return true;
}

/************** send_dealer_card *****************/
bool send_dealer_card(int new_socket, card_t* my_card){
    
    char card_string[30];
    char* card_s = translate_card(my_card);

    if (card_s == NULL){    //if translating function failed
        return false;
    }

    snprintf(card_string, 30, "DEALER %s", card_s);   //concatenate strings
    printf("    Sending message: %s\n", card_string);

    int out = send(new_socket, card_string, strlen(card_string),0);
    nanosleep((const struct timespec[]){{0, 26000000l}},NULL);

    if (out < 0) {      //check that sending worked
        return false;
    }
    
    free(card_s);
    return true;
}

/************** prompt_decision *****************/
int prompt_decision(int new_socket){

    char* decision="DECISION";  //create comparison strings
    
    send(new_socket, decision, strlen(decision),0);
    nanosleep((const struct timespec[]){{0, 26000000l}},NULL);
    printf("    Sending message: %s\n", decision);

    char buffer[1024] = { 0 };

    do {
        read(new_socket, buffer, 1024);
    } while (buffer == NULL);
    printf("    Message Recieved: %s\n", buffer);

    char* hit="HIT";
    char* stand="STAND";

    if((strncmp(buffer,hit, 3)) == 0){     //if it's HIT, return 1
        return 1;
    } else if((strncmp(buffer,stand, 3)) == 0){  //if it's STAND, return 2
        return 2;
    }

    return 0;
}

/************** send_result *****************/
bool send_result(int new_socket, int result){

    char* win= "WIN";
    char* lose = "LOOSE";
    char* push = "PUSH";
    char* bust = "BUST";
    char result_string[20];

    if(result == 1){
        snprintf(result_string, 20, "RESULT %s", win);
    }
    else if(result==2){     //if 2, send LOSE
        snprintf(result_string, 20, "RESULT %s", lose);
    }
    else if(result==3){     //if 3, send PUSH
        snprintf(result_string, 20, "RESULT %s", push);
    } 
    else if(result==4){     //if 4, send BUST
        snprintf(result_string, 20, "RESULT %s", bust);
    } else {
        return false;
    }

    send(new_socket, result_string, strlen(result_string),0);
    nanosleep((const struct timespec[]){{0, 26000000l}},NULL);
    printf("    Sending message: %s\n", result_string);
    
    return true;
}


/************** quit_game *****************/
bool quit_game(int new_socket){

    // closing the connected socket
    close(new_socket);
    printf("    Quit Game\n");

    return true;
}

/*************** LOCAL FUNCTIONS ****************/
/*These functions do not leave this file*/

/************** translate_card *****************/
char* translate_card(card_t* mycard){

    int card_suite = get_card_suite(mycard);
    char suite_trn[10];
    
    int card_num = get_card_num(mycard);    
    char num_trn[10];

    if(card_suite == 1){
        strncpy(suite_trn, "Hearts ",10);
    }
    else if(card_suite == 2){
        strncpy(suite_trn, "Diamonds ",10);
    }
    else if(card_suite == 3){
        strncpy(suite_trn, "Spades ",10);
    }
    else if(card_suite == 4){
        strncpy(suite_trn, "Clubs ",10);
    }
    else {
        return NULL;    //return NULL if error
    }

    //1 and 11 are both return to ace?
    if(card_num == 1){
        strncpy(num_trn, "Ace",10);
    }
    else if(card_num == 2){
        strncpy(num_trn, "Two",10);
    }
    else if(card_num == 3){
        strncpy(num_trn, "Three",10);
    }
    else if(card_num == 4){
        strncpy(num_trn, "Four",10);
    }
    else if(card_num == 5){
        strncpy(num_trn, "Five",10);
    }
    else if(card_num == 6){
        strncpy(num_trn, "Six",10);
    }
    else if(card_num==7){
        strncpy(num_trn, "Seven",10);
    }
    else if(card_num==8){
        strncpy(num_trn, "Eight",10);
    }
    else if(card_num == 9){
        strncpy(num_trn, "Nine",10);
    }
    else if(card_num == 10){
        strncpy(num_trn, "Ten",10);
    }
    else if(card_num == 11){
        strncpy(num_trn, "Ace",10);
    }
    else if(card_num == 12){
        strncpy(num_trn, "Jack",10);
    }
    else if(card_num == 13){
        strncpy(num_trn, "Queen",10);
    }
    else if(card_num == 14){
        strncpy(num_trn, "King",10);
    } else{
        return NULL;        //return NULL if error
    }

    char* string_output = malloc(sizeof(char)*100);
    snprintf(string_output, 100, "%s of %s", num_trn, suite_trn);

    return string_output;
}