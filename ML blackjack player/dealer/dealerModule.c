/* dealerModule.c — function file for dealer module, 
 *
 * module for refereeing the game of 21, evaluating wins and losses, and communicating results
 * creates/shuffles the deck, deals cards, receives player decisions
 * calls functions from `dealer.c` Network file for socket communication 
 *
 * By Will Elliott for CS50 Final Project, Fall 2022
 */

/****************LIBRARIES****************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "../card_lib/cards.h"
#include "../card_lib/hand.h"
#include "../card_lib/deck.h"
#include "../network/dealer_network.h"


/****************STATIC FUNCTION DECLARATIONS****************/
static void parse_args(const int argc, char* argv[], int* num_games);
static void play(int new_socket);
static void quit(int new_socket); 
static void setup_game(deck_t* deck, hand_t* player_11_hand, hand_t* player_1_hand, hand_t* dealer_11_hand, hand_t* dealer_1_hand, int new_socket);
static bool will_dealer_hit(hand_t* dealer_hand);
static bool check_bust(hand_t* hand);
int calculate_result(deck_t* deck, hand_t* player_11_hand, hand_t* player_1_hand, hand_t* dealer_11_hand, hand_t* dealer_1_hand);
static void add_player_card(deck_t* deck, hand_t* player_11_hand, hand_t* player_1_hand, int new_socket);
static void add_dealer_card(deck_t* deck, hand_t* dealer_11_hand, hand_t* dealer_1_hand, int new_socket);


bool player_has_ace = false;
bool dealer_has_ace = false;

/****************FUNCTION IMPLEMENTATIONS****************/

int main(int argc, char* argv[]) { 
    int num_games; // number of games the dealer and player will play
    parse_args(argc,argv,&num_games); // check arguments
    srand(time(NULL));     //seed shuffle function

    int new_socket;
    new_socket = connect_dealer_network(); // set up socket
    send_begin(new_socket); // wait for player to join

    int current_game_number = 1;
    // as long as you haven't played the max number of games yet, play another game
    while (current_game_number <= num_games) {
        play(new_socket);
        current_game_number++;
    }

    quit(new_socket); // send a QUIT message to the client when done

    return 0;
}


/****************play()****************/
/* Plays a new game of 21 with a fresh deck. Flagship function.
    Calls helper functions to:
    - create a deck of 52 cards
    - shuffle the deck
    - deal cards
    - calculate game result
    - clean up (free hands, deck)
    - returns nothing
   Takes socket int as input
*/
static void play(int new_socket) {
    deck_t* deck = create_deck(); // create a deck of 52 cards
    shuffle_deck(deck); // shuffle the deck

    hand_t* dealer_11_hand = create_hand(); // set up empty hand to hold dealer's cards, first ace is 11
    hand_t* dealer_1_hand = create_hand(); // set up empty hand to hold dealer's cards, all aces are 1
    hand_t* player_11_hand = create_hand(); // set up empty hand to hold player's cards, first ace is 11
    hand_t* player_1_hand = create_hand(); // set up empty hand to hold player's cards, all aces are 1


    int result = -1;
    // setup the game, dealing first cards
    setup_game(deck, player_11_hand, player_1_hand, dealer_11_hand, dealer_1_hand, new_socket);
    // prompt first decision
    
    
    int decision  = prompt_decision(new_socket);
    while(decision == 1) {  // while the player hits
        add_player_card(deck,player_11_hand,player_1_hand,new_socket); // hit the player

        if (will_dealer_hit(dealer_1_hand)) { // decide if the dealer will hit
            add_dealer_card(deck,dealer_11_hand,dealer_1_hand,new_socket);
        }
        
        bool player_bust = (check_bust(player_1_hand) && check_bust(player_11_hand)); 
        if (player_bust) { // does the player bust?
            result = 2;
            break; // break early if BUST
        }
        decision  = prompt_decision(new_socket);
    }
    // calculate the game’s result (WIN, LOOSE, BUST, or PUSH) and send a message to the client
    if (result == 2) { // automatic loss if player busts
        send_result(new_socket,result);
    } else { // we need to calculate
        result = calculate_result(deck,player_11_hand,player_1_hand,dealer_11_hand,dealer_1_hand);
        send_result(new_socket,result);
    }
    
    // clean up (free memory)
    player_has_ace = false;
    dealer_has_ace = false;
    delete_hand(player_11_hand);
    delete_hand(player_1_hand);
    delete_hand(dealer_11_hand);
    delete_hand(dealer_1_hand);
    delete_deck(deck);
}

/****************calculate_result()****************/
/*
    Given player and dealer hands
    return 1 if player wins
    return 2 if player loses
    return 3 if PUSH
*/
int calculate_result(deck_t* deck, hand_t* player_11_hand, hand_t* player_1_hand, hand_t* dealer_11_hand, hand_t* dealer_1_hand) {
    bool dealer_bust = (check_bust(dealer_1_hand) && check_bust(dealer_11_hand));
    if (dealer_bust) { // if the dealer busts, the player wins because we've already checked for player bust
        return 1; 
    }

    int player_11_value = get_hand_points(player_11_hand);
    if(player_11_value > 21) { // if the 11 hand busted
        player_11_value = -1; // can't use the busted hand
    }
    int player_1_value = get_hand_points(player_1_hand);

    int dealer_11_value = get_hand_points(dealer_11_hand);
    if(dealer_11_value > 21) { // if the 11 hand busted
        dealer_11_value = -1; // can't use the busted hand
    }
    int dealer_1_value = get_hand_points(dealer_1_hand);


    int final_dealer_value; // getting the greater of the two valid hands
    int final_player_value;

    if (dealer_11_value >= dealer_1_value) {  // what's the top hand from player & dealer? Is it the 11 hand or the 1 hand?
        final_dealer_value = dealer_11_value;
    } else {
        final_dealer_value = dealer_1_value;
    }
    if (player_11_value >= player_1_value) {
        final_player_value = player_11_value;
    } else {
        final_player_value = player_1_value;
    }
    
    if (final_dealer_value > final_player_value) { // LOSE
        return 2;
    } else if (final_dealer_value < final_player_value) { // WIN
        return 1;
    } else { // PUSH
        return 3;
    }
}

/****************add_player_card()****************/
/* gets a new card from deck, inserts it into the player's hands, and sends a message
    Handles floating ace by inputting 11 and a 1 into multiple hands, to be evaluated later.
*/
void add_player_card(deck_t* deck, hand_t* player_11_hand, hand_t* player_1_hand, int new_socket) {
    card_t* card;
    card = draw_deck(deck);

    if (get_card_value(card) == 1) { // if the card is an ace
        if (player_has_ace) { // if the player already has an ace
            insert_card(card,player_1_hand); // insert a 1-value card to both hands
            card_t* card1 = create_card(1,get_card_suite(card)); // create duplicate card for memory freeing later
            insert_card(card1,player_11_hand);
        } else { // IF THE PLAYER DOES NOT ALREADY HAVE AN ACE
            player_has_ace = true; // it does now
            insert_card(card,player_1_hand); // insert a 1 into the 1-hand
            card_t* card11 = create_card(11,get_card_suite(card));// create duplicate card for memory freeing later
            insert_card(card11,player_11_hand); // insert an 11 into the 11-hand
        }
    }else{ // input any non-ace as-is
        insert_card(card,player_1_hand);
        card_t* new_card = create_card(get_card_num(card),get_card_suite(card));// create duplicate card for memory freeing later
        insert_card(new_card,player_11_hand);
    }
    send_card(new_socket, card);
}

/****************add_dealer_card()****************/
/* gets a new card from deck, inserts it into the dealer's hands, and sends a message
    Handles floating ace by inputting 11 and a 1 into multiple hands, to be evaluated later.
*/
void add_dealer_card(deck_t* deck, hand_t* dealer_11_hand, hand_t* dealer_1_hand, int new_socket) {
    card_t* card;
    card = draw_deck(deck);
    if (get_card_value(card) == 1) { // if the card is an ace
        if (dealer_has_ace) { // if the dealer already has an ace
            insert_card(card,dealer_1_hand); // insert a 1-value card to both hands
            card_t* card1 = create_card(1,get_card_suite(card));// create duplicate card for memory freeing later
            insert_card(card1,dealer_11_hand);
        } else { // IF THE DEALER DOES NOT ALREADY HAVE AN ACE
            dealer_has_ace = true; // it does now
            insert_card(card,dealer_1_hand); // insert a 1 into the 1-hand
            card_t* card11 = create_card(11,get_card_suite(card));// create duplicate card for memory freeing later
            insert_card(card11,dealer_11_hand); // insert an 11 into the 11-hand
        }
    }else{ // input any non-ace as-is
        insert_card(card,dealer_1_hand);
        card_t* new_card = create_card(get_card_num(card),get_card_suite(card));// create duplicate card for memory freeing later
        insert_card(new_card,dealer_11_hand);
    }
    send_dealer_card(new_socket, card);

}

/****************setup_game()****************/
/* Deals the first cards to player/dealer
    - requires valid deck, player's hand, dealer's hand, socket number
    - returns nothing
*/
static void setup_game(deck_t* deck, hand_t* player_11_hand, hand_t* player_1_hand, hand_t* dealer_11_hand, hand_t* dealer_1_hand, int new_socket) {
    card_t* card;
    // deal the player one card
    add_player_card(deck,player_11_hand,player_1_hand,new_socket);
    // deal the dealer one card
    card = draw_deck(deck);
    if (get_card_value(card) == 1) {
        dealer_has_ace = true;
        insert_card(card,dealer_1_hand);
        card_t* card11 = create_card(11,get_card_suite(card));
        insert_card(card11,dealer_11_hand);
    }else{
        insert_card(card,dealer_1_hand);
        card_t* new_card = create_card(get_card_num(card),get_card_suite(card));
        insert_card(new_card,dealer_11_hand);
    }

    // deal the player a second card
    add_player_card(deck,player_11_hand,player_1_hand,new_socket);
    // deal the dealer a second card
    add_dealer_card(deck,dealer_11_hand,dealer_1_hand,new_socket);
}

/****************will_dealer_hit()****************/
/*
    returns true if dealer's hand value < 17
        (indicates HIT)
    returns false if dealer's hand value >= 17
        (indicate STAND)
*/
static bool will_dealer_hit(hand_t* dealer_1_hand) {
    int value = get_hand_points(dealer_1_hand);
    if (value < 17) {
        return true;
    } else {
        return false;
    }
}

/****************check_bust()****************/
/*
    returns true if the value of the given hand > 21
    false otherwise
*/
static bool check_bust(hand_t* hand) {
    if (get_hand_points(hand) > 21) {
        return true;
    }
    return false;
}

/****************quit()****************/
/* Sends "QUIT" message to the socket
    Calls functions from network directory:
    - uses currently connected new_socket
    - returns nothing
*/
static void quit(int new_socket) {
    char* quit = "QUIT";
    send(new_socket, quit, strlen(quit),0);

}


/****************parse_args()****************/
/* Handles command line inputs 
    - INPUTS: argument count, array of argument strings, pointer (reference to an int) to number of games
    - PRINTS: error messages if call is invalid
    - EXITS if there is an error
*/
static void parse_args(const int argc, char* argv[], int* num_games) {
    // too many or too few arguments
    if (argc != 2) {
        fprintf(stderr, "Invalid number of arguments.\nUSAGE: ./dealer <number of games>\n");
        exit(1);
    }

    int length = strlen(argv[1]);
    // number of games inputted cannot be converted to int
    for (int i = 0; i < length;i++) {
        if(!isdigit(argv[1][i])) {
            fprintf(stderr, "Given number of games is not an integer.\nUSAGE: ./dealer <number of games>\n");
            exit(2);
        }
    }
    
    sscanf(argv[1], "%d",num_games);
    printf("NUM GAMES: %d\n",*num_games);
    // too few games. Need to play more than 0 games.
    if (*num_games <= 0) {
        fprintf(stderr, "Number of games must be greater than 0.\nUSAGE: ./dealer <number of games>\n");
        exit(3);
    }
    return;
}

