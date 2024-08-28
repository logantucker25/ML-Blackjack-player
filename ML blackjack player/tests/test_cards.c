/* test_cards.c — testing script for cards, deck, and hand modules
 *
 * Compile: make
 * Usages: ./cards
 * 
 * By Grace Turner for CS50 Final Project
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../card_lib/cards.h"
#include "../card_lib/deck.h"
#include "../card_lib/hand.h"

int main(void){

    //CREATE A DECK
    fprintf(stdout, "Creating new deck.... \n");
    deck_t* my_deck = create_deck();
    print_deck(my_deck);

    //SHUFFLE THE DECK
    fprintf(stdout, "\nShuffling deck.... \n");
    shuffle_deck(my_deck);
    print_deck(my_deck);

    fprintf(stdout, "\nShuffling deck.... \n");
    shuffle_deck(my_deck);
    print_deck(my_deck);

    fprintf(stdout, "\nShuffling deck.... \n");
    shuffle_deck(my_deck);
    print_deck(my_deck);

    //DRAW FROM DECK
    fprintf(stdout, "\nDrawing from deck.... \n");
    card_t* my_card_1 = draw_deck(my_deck);
    printf("Drawn card!! Num: %d, Suite: %d\n", get_card_num(my_card_1), get_card_suite(my_card_1));
    print_deck(my_deck);

    fprintf(stdout, "\nShuffling deck.... \n");
    shuffle_deck(my_deck);
    print_deck(my_deck);

    fprintf(stdout, "\nDrawing from deck.... \n");
    card_t* my_card_2 = draw_deck(my_deck);
    printf("Drawn card!! Num: %d, Suite: %d\n", get_card_num(my_card_2), get_card_suite(my_card_2));
    print_deck(my_deck);    

    fprintf(stdout, "\nShuffling deck.... \n");
    shuffle_deck(my_deck);
    print_deck(my_deck);

    fprintf(stdout, "\nDrawing from deck.... \n");
    card_t* my_card_3 = draw_deck(my_deck);
    printf("Drawn card!! Num: %d, Suite: %d\n", get_card_num(my_card_3), get_card_suite(my_card_3));
    print_deck(my_deck); 

    //INSERT INTO HAND
    fprintf(stdout, "\nCreating hand.... \n");
    hand_t* my_hand = create_hand();
    print_hand(my_hand);

    fprintf(stdout, "\nMaking new card (1,1) and putting into hand.... \n");
    card_t* my_card_4 = create_card(1, 1);
    insert_card(my_card_4, my_hand);
    print_hand(my_hand);

    fprintf(stdout, "\nMaking new card (2,2) and putting into hand.... \n");
    card_t* my_card_5 = create_card(2, 2);
    insert_card(my_card_5, my_hand);
    print_hand(my_hand);

    fprintf(stdout, "\nDrawing card from deck and putting into hand.... \n");
    card_t* my_card_6 = draw_deck(my_deck);
    insert_card(my_card_6, my_hand);
    print_hand(my_hand);

    fprintf(stdout, "\nDrawing card from deck and putting into hand.... \n");
    card_t* my_card_7 = draw_deck(my_deck);
    insert_card(my_card_7, my_hand);
    print_hand(my_hand);


    //DELETE 
    fprintf(stdout, "\nDeleting cards.... \n");
    delete_card(my_card_1);
    delete_card(my_card_2);
    delete_card(my_card_3);

    fprintf(stdout, "\nDeleting hand.... \n");
    delete_hand(my_hand);

    fprintf(stdout, "\nDeleting deck.... \n\n");
    delete_deck(my_deck);

}