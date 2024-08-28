/* player.c — player module for training and playing blackjack
 *
 * Compile: make
 * Usages: ./player playerName trainingFile.txt
 * 
 * By Logan Tucker for CS50 Final Project
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../card_lib/cards.h"
#include "../card_lib/hand.h"
#include "../network/player_network.h"
#include "file.h"
#include <time.h>
#include <ctype.h>

bool trainingMode = false;

/************************ create global tables ***************************/
static float q[22][22][2] = {0}; // holds probability of hit and stand at every hand combo
static int qCount[22][22][2] = {0}; // holds count of decisions made at every hand combo

/************************ hand history structures and funcs (modified from cs50's bag) ***************************/
// a strcut to hold card total and a decision made at every decision point in the game
typedef struct handEvent {
    int player;
    int dealer;
    int decision;
    struct handEvent *next;
} handEvent_t;

typedef struct handHistory {
    struct handEvent *head;
} handHistory_t;

// create a new hand event and set values to given ints
static handEvent_t* handEvent_new(int player, int dealer, int decision) {
    handEvent_t* handEvent = malloc(sizeof(handEvent_t));
    if (handEvent == NULL) {
        return NULL;
    } 
    else {
        handEvent->player = player;
        handEvent->dealer = dealer;
        handEvent->decision = decision;
        return handEvent;
    }
}

// create a new hand history (linked list of hand events), occurs at the start of games and start of new rounds
handHistory_t* handHistory_new(void) {
    handHistory_t* handHistory = malloc(sizeof(handHistory_t));
    if (handHistory == NULL) {
        return NULL;              
    } 
    else {
        handHistory->head = NULL;
        return handHistory;
    }
}

// insert a hand event into a given hand history
void handHistory_insert(handHistory_t* handHistory, int player, int dealer, int decision) {
    if (handHistory != NULL && player != 0 && dealer != 0 && decision != 0) {
        
        // make new hand event
        handEvent_t* new = handEvent_new(player, dealer, decision);
        if (new != NULL) {

            // set this event to the head of the handHistory
            new->next = handHistory->head;
            handHistory->head = new;
        }
    }
}

// after a game has finished save the hand history and outcome (reward) to the data tables
void inputHandHistoryToTable(handHistory_t* handHistory, int reward) {
    if (handHistory != NULL) {

        // loop through each event
        for (handEvent_t* handEvent = handHistory->head; handEvent != NULL; handEvent = handEvent->next) {
            
            //update tables
            qCount[handEvent->player][handEvent->dealer][handEvent->decision] += 1;
            
            q[handEvent->player][handEvent->dealer][handEvent->decision] +=
            ((1/((float)qCount[handEvent->player][handEvent->dealer][handEvent->decision])) *
            (reward - q[handEvent->player][handEvent->dealer][handEvent->decision]));

        }
    }
}

// delete hand history and its contents
void handHistory_delete(handHistory_t* handHistory) {
    if (handHistory != NULL) {
        for (handEvent_t* handEvent = handHistory->head; handEvent != NULL; ) {
            handEvent_t* next = handEvent->next;     
            free(handEvent);                   
            handEvent = next;                     
        }
        free(handHistory);
    }
}

/************************ game set up funcs ***************************/
// check if args from command line are valid
static void parseArgs(const int argc, char* argv[]) {
    
    // num args
    if (argc != 3) {
        fprintf(stderr, "invalid number of args\n");
        exit(1);
    }

    // arg 1 player name
    if (strlen(argv[1]) == 0) {
        fprintf(stderr, "invalid name\n");
        exit(2);
    }

    // arg 2 traing data file
    if (trainingMode) {
        FILE* fp = fopen(argv[2], "w");
        if(fp == NULL) {
            fprintf(stderr, "unable to write into training file\n");
            exit (3);
        }
        fclose(fp);
    }
    else if (!trainingMode) {
        FILE* fp = fopen(argv[2], "r");
        if(fp == NULL) {
            fprintf(stderr, "unable to read from training file\n");
            exit (4);
        }
        fclose(fp);
    }
}

/************************ training data funcs ***************************/
// update the training data chart based on round just played
void noteResult(int result, handHistory_t* handHistory) {
    
    // set reward based on win (1) loss (-1) or push (0)
    int reward;
    if (result == 1) {
        reward = 1;
    }
    else if (result == 2) {
        reward = -1;
    }
    else if (result == 3) {
        reward = 0;
    }

    // send reward and and hand history to helper fun c that inputs it into the tables
    inputHandHistoryToTable(handHistory, reward);
}

// (train helper) this helper finds the number of words in a line of text and is used to minmize the amount of slots we make in arrays
int getNumTokens(char* string) {
    int num = 0;
    for (int i = 0; i < strlen(string); i++) {
        if (i == 0 && !isspace(string[i])) {
            num ++;
        }
        if (isspace(string[i]) && !isspace(string[i+1])) {
            num++;
        }
    }
    return num;
}


// download training data from a text file to tables
void train(char* trainingData) {

    // open file containing training data
    FILE* fp = fopen(trainingData, "r");
    if (fp != NULL) {

        // get string of each line in the file
        for (int i = 0; i < file_numLines(fp); i++) {

            // cycles through lines already read
            for (int j = 0; j < i; j ++){       
                char* garbage_line = file_readLine(fp);
                free(garbage_line);
            }

            // read line from text file
            char* line = file_readLine(fp);

            // confirm that all four data (playercards dealercards hitProb standProb) are in the line
            if (getNumTokens(line) == 4) {
                float tokens[4];

                    // collect data from line into an array of floats
                    sscanf(line, "%f %f %f %f", &tokens[0], &tokens[1], &tokens[2], &tokens[3]);
                    
                    // save data from line into the tables
                    int player = (int)(tokens[0]);
                    int dealer = (int)(tokens[1]);

                    float hitProb = tokens[2];

                    // put hit Prob into q
                    q[player][dealer][1] = hitProb;

                    float standProb = tokens[3];

                    // put stand Prob into q
                    q[player][dealer][2] = standProb;
            }
            free(line);
        }
        fclose(fp);
    }
}

// save newly recorded training table into a text file
void saveTraining(char* trainingData) {
    FILE* fp = fopen(trainingData, "w");
    if (fp != NULL) {

        // loop through every possible player/dealer hand combo
        for (int p = 1; p <=21; p++) {
            for (int d = 1; d <=21; d++) {

                // print data for this hand combo in the table to the text file
                fprintf(fp, "%d %d %f %f\n", p, d, q[p][d][1], q[p][d][2]);
            }
        }
        fclose(fp);
    }
}

/************************ basics aka player brain ***************************/
// give a new card to a specified hand
void giveCard(hand_t* hand, card_t* card) {
    insert_card(card, hand);
}

// if you make a decision call this function to save the evemnt into hand history 
void save_decision(handHistory_t* handHistory, hand_t* myHand, hand_t* dealerHand, int decision) {
    handHistory_insert(handHistory, get_hand_points(myHand), get_hand_points(dealerHand), decision);
}

// make an optimal hit or stand decision based on current hand values
int getDecision(hand_t* myHand, hand_t* dealerHand) {
    
    // if in training mode return 1 or 2 (hit or stand) randomly
    if (trainingMode) {
        return (rand()%2) + 1;
    }
    
    // if not in training mode get probabilities of winning if hit and if stand from q table
    float hitProb = q[get_hand_points(myHand)][get_hand_points(dealerHand)][1];
    float standProb = q[get_hand_points(myHand)][get_hand_points(dealerHand)][2];

    // compare hit and stand probs, return whichever descision has a higher prob
    if (hitProb > standProb) {
        return 1;
    }
    else {
        return 2;
    }
}

/************************ main ***************************/
int main(const int argc, char* argv[]) {

    // set up args
    parseArgs(argc, argv);    
    char* playerName = argv[1];
    char* trainingData = argv[2];

    // seed for random training generator
    srand(time(0)); 
    
    // if in training mode read in training from a text file
    if (!trainingMode) {
        train(trainingData);
    }

    // connect to dealer and set up game mode
    int new_socket = begin_game(playerName);
    bool gameOn = true;
    
    // set up data storage
    hand_t* myHand = create_hand();
    hand_t* dealerHand = create_hand();
    handHistory_t* handHistory = handHistory_new(); // a record of every hand state that we made a decicion at and the descioon we made
    
    heard_info_t* received;
    int command;

    char queue[100];
    strcpy(queue, "aa");
    
    // gameplay loop
    while (gameOn) {
        
        // get command
        received = player_listen(new_socket, queue);
        command = get_heard_command(received);

        // CARD
        if (command == 2) {
            
            // put card in my hand
            giveCard(myHand, get_heard_card(received));
        }

        // DEALER
        if (command == 3) {
            // put card in dealer hand
            giveCard(dealerHand, get_heard_card(received));
        }

        // DECISION
        if (command == 4) {

            // get a decision based on current point totals in my and and dealer hand
            int decision = getDecision(myHand, dealerHand);

            // save decision made and point hand totals in handHistory
            save_decision(handHistory, myHand, dealerHand, decision);

            // send decision to dealer
            send_decision(new_socket, decision);
        }

        // RESULT
        if (command == 5) {

            // if you are training save game data to training table
            if (trainingMode) {
                noteResult(get_heard_result(received), handHistory);
            }

            // delete hand hist
            handHistory_delete(handHistory);

            // make new empty hand hist
            handHistory = handHistory_new();
            
            // delete hands
            delete_hand(myHand);
            delete_hand(dealerHand);

            // make new empty hands
            myHand = create_hand();
            dealerHand = create_hand();
        }
        
        // QUIT
        if (command == 6) {

            // delet stored items
            handHistory_delete(handHistory);
            delete_hand(myHand);
            delete_hand(dealerHand);

            // if training save data from games into text file
            if (trainingMode) {
                saveTraining(trainingData);
            }

            gameOn = false;
        }
        delete_heard_info(received);
    }
    end_game(new_socket);
    return 0;
}