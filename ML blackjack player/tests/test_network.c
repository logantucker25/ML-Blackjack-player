/* test_network.c — test file for network module
 *
 * module for handling network for game
 * 
 * By Grace Turner for CS50 Final Project
 */

#include <stdio.h>
#include "../network/dealer_network.h"

int main(void){
    int out;
    
    int new_socket = connect_dealer_network();
    printf("\n[TEST] Socket = %d\n", new_socket);

    if (new_socket == -1){
        printf("Error connecting\n");
        exit(1);
    }

    char* player_name = send_begin(new_socket);
    printf("[TEST] Test playerName: %s\n", player_name);

    //Send CARD
    printf("\n[TEST] Testing send_card...\n");
    card_t* test_card = create_card(1,1);
    out = send_card(new_socket, test_card);

    if (out == 1){
        printf("[TEST] Sending card successful\n");
    }

    //Send CARD Again
    printf("\n[TEST] Testing send_card...\n");
    card_t* test_card_1 = create_card(2,2);
    out = send_card(new_socket, test_card_1);

    if (out == 1){
        printf("[TEST] Sending card successful\n");
    }

    //Send CARD Again
    printf("\n[TEST] Testing send_card...\n");
    card_t* test_card_2 = create_card(13,3);
    out = send_card(new_socket, test_card_2);

    if (out == 1){
        printf("[TEST] Sending card successful\n");
    }

    //Send CARD Again
    printf("\n[TEST] Testing send_card...\n");
    card_t* test_card_3 = create_card(11,3);
    out = send_card(new_socket, test_card_3);

    if (out == 1){
        printf("[TEST] Sending card successful\n");
    }

    //Send DEALER cards
    printf("\n[TEST] Testing send_dealer_card...\n");
    card_t* test_card_4 = create_card(12,4);
    out = send_dealer_card(new_socket, test_card_4);

    if (out == 1){
        printf("[TEST] Sending card successful\n");
    }

    printf("\n[TEST] Testing send_dealer_card...\n");
    card_t* test_card_5 = create_card(3,2);
    out = send_dealer_card(new_socket, test_card_5);

    if (out == 1){
        printf("[TEST] Sending card successful\n");
    }

    //TEST RESULT
    printf("\n[TEST] Testing win...\n");
    out = send_result(new_socket, 1);

    printf("\n[TEST] Testing lose...\n");
    out = send_result(new_socket, 2);

    printf("\n[TEST] Testing push...\n");
    out = send_result(new_socket, 3);
    
    //PROMPT DECISION
    printf("\n[TEST] Testing prompt decision...\n");
    int decide = prompt_decision(new_socket);
    printf("[TEST] Player decided %d\n", decide);
    
    printf("\n[TEST] Testing prompt decision...\n");
    decide = prompt_decision(new_socket);
    printf("[TEST] Player decided %d\n", decide);

    printf("\n[TEST] Quitting game...\n");
    quit_game(new_socket);

    delete_card(test_card);
    delete_card(test_card_1); 
    delete_card(test_card_2); 
    delete_card(test_card_3); 
    delete_card(test_card_4);
    delete_card(test_card_5);

    printf("[TEST] Done!\n");

    return 0;
}
