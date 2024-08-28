/* test_player_network.c — test file for network module
 *
 * Module for handling network for game
 * To be run alongside test_network in same directory
 * 
 * By Grace Turner for CS50 Final Project
 */

#include <stdio.h>
#include "../card_lib/hand.h"
#include "../network/player_network.h"

int main(void){
    
    //CONNECTING TO SOCKET

    int new_socket = begin_game("TestGame21");

    if (new_socket == -1){
        printf("Error connecting\n");
        exit(1);
    }

    printf("\n[TEST] Socket = %d\n", new_socket);
    hand_t* test_hand = create_hand();

    //RECIEVING CARDS
    printf("\n[TEST] Reading card 0 ...\n");
    char* queue = malloc(sizeof(char)*100);
    heard_info_t* test_info = player_listen(new_socket, queue);
    print_heard_info(test_info);
    insert_card(get_heard_card(test_info), test_hand);

    printf("\n[TEST] Reading card 1...\n");
    heard_info_t* test_info_1 = player_listen(new_socket, queue);
    print_heard_info(test_info_1);
    insert_card(get_heard_card(test_info_1), test_hand);

    printf("\n[TEST] Reading card 2...\n");
    heard_info_t* test_info_2 = player_listen(new_socket, queue);
    print_heard_info(test_info_2);
    insert_card(get_heard_card(test_info_2), test_hand);

    printf("\n[TEST] Reading card 3...\n");
    heard_info_t* test_info_3 = player_listen(new_socket, queue);
    print_heard_info(test_info_3);
    insert_card(get_heard_card(test_info_3), test_hand);

    //READ DEALER CARDS
    printf("\n[TEST] Reading card 4...\n");
    heard_info_t* test_info_4 = player_listen(new_socket, queue);
    print_heard_info(test_info_4);
    insert_card(get_heard_card(test_info_4), test_hand);

    printf("\n[TEST] Reading card 5...\n");
    heard_info_t* test_info_5 = player_listen(new_socket, queue);
    print_heard_info(test_info_5);
    insert_card(get_heard_card(test_info_5), test_hand);
    

    //READ RESULT
    printf("\n[TEST] Reading result...\n");
    heard_info_t* test_info_6 = player_listen(new_socket, queue);
    print_heard_info(test_info_6);
    insert_card(get_heard_card(test_info_6), test_hand);

    printf("\n[TEST] Reading result...\n");
    heard_info_t* test_info_7 = player_listen(new_socket, queue);
    print_heard_info(test_info_7);
    insert_card(get_heard_card(test_info_7), test_hand);

    printf("\n[TEST] Reading result...\n");
    heard_info_t* test_info_8 = player_listen(new_socket, queue);
    print_heard_info(test_info_8);
    insert_card(get_heard_card(test_info_8), test_hand);

    //READ DECISION AND SEND ANSWER
    printf("\n[TEST] Reading decision...\n");
    heard_info_t* test_info_9 = player_listen(new_socket, queue);
    print_heard_info(test_info_9);
    insert_card(get_heard_card(test_info_9), test_hand);

    printf("[TEST] Sending answer...\n");
    int out = send_decision(new_socket, 1);
    printf("\n[TEST] Send_decision out = %d\n", out);

    printf("\n[TEST] Reading decision...\n");
    heard_info_t* test_info_10 = player_listen(new_socket, queue);
    print_heard_info(test_info_10);
    insert_card(get_heard_card(test_info_10), test_hand);
    
    printf("[TEST] Sending answer...\n");
    out = send_decision(new_socket, 2);
    printf("\n[TEST] Send_decision out = %d\n", out);

    //QUIT GAME
    printf("[TEST] Quitting game...\n");
    end_game(new_socket);

    //DELETE INFO STRUCTURES
    delete_heard_info(test_info);
    delete_heard_info(test_info_1);
    delete_heard_info(test_info_2);
    delete_heard_info(test_info_3);
    delete_heard_info(test_info_4);
    delete_heard_info(test_info_5);
    delete_heard_info(test_info_6);
    delete_heard_info(test_info_7);
    delete_heard_info(test_info_8);
    delete_heard_info(test_info_9);
    delete_heard_info(test_info_10);
    delete_hand(test_hand);
    free(queue);

    printf("[TEST] Done!\n");

    return 0;
}