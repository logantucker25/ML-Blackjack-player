# CS50 Final Project
## Design Specification

> The PROJECT-TEAM-13 is a standalone program that proceeds the game "Blackjack" between a dealer and a player through a server. They will communicate through the server using a terminal.

### User Interface

> The PROJECT-TEAM-13's only interface with a dealer and player is on the command-line;

### Input and Outputs

> A dealer and a player will be communicate in certain server with socket 8093. Through the server, by sending and reading / listening the contents on the file, Blackjack will be proceeded. 

### Decomposition into modules

> Cards module to handle card structures
  We anticipate the following sub-modules and functions:
1. cards.h
    - *create_card*, which creates a card structure
    - *get_card_num*, which finds the number of the provided card
    - *get_card_suite*, which finds the suite of the provided card
    - *get_card_value*, which finds the value of the provided card
    - *delete_card*, which deletes the card

2. hand.h
    - *create_hand*, which creates a hand structure
    - *insert_card*, which inserts the provided card to the hand
    - *get_hand_points*, which returns the sum of the card values provided
    - *print_hand* to be used for debugging, which prints the cards in the hand
    - *delete_hand*, which deletes the hand and all contained cards

3. deck.h
    - *create_deck*, which creates a deck structure and fills it with cards
    - *shuffle_deck*, which randomly rearranges the provided deck
    - *draw_deck*, which draws the top card from the deck
    - *get_deck_count*, which returns the number of cards left in the deck
    - *print_deck*, which prints the deck contents
    - *delete_deck*, which deletes the deck and all contained cards

4. file.h
   This module is based off of the file.h module used for the TSE project, and the player will use the following functions for reading from files:
    - *file_readLine*, for reading a line from a file
    - *file_numLines*, for counting the number of lines in a file

*Network module to handle communication between dealer and player*
*We anticipate the following sub-modules or functions:*
1. dealer_network.c
    - *connect_dealer_network*, which connects a dealer to network 
    - *send_begin*, which sends JOIN and BEGIN to the player to begin the blackjack
    - *translate_card*, which converts card information from int to string
    - *send_card*, which sends concatenated string to a player 
    - *send_dealer_card*, which sends concatenated string to a player 
    - *prompt_decision*, which reads the decision for the player and convert his/her decision to int 
    - *send_result*, which converts the result from int to string and send it to the player
    - *quit_game*, which quits the game

2. player_network.c
    - *begin_game*, which connects a player to the network to play game
    - *player_listen*, which converts a string in the file to int to match a module that fits
    - *send_decision*, which converts a player's decision from int to string to send it to the dealer
    - *end_game*, which quits the game
    - *delete_heard_info*, which is a local function to delete the structure and contained info
    - *print_heard_info*, which is a local function to help testing module


### Pseudocode for the modules

#### Cards module:
*Card structure and functions*
- *create_card*
    1. It needs two parameters: valid card number (1-14) and suite (1-4)
    2. Function returns a pointer to a card initialized with values
    3. Returns NULL if memory allocation error, otherwise return my_card struct
- *get_card_num*
    1. If a value of the parameter is NULL, return 0
    2. Return the number on the card
- *get_card_suite*
    1. If a value of the parameter is NULL, return 0
    2. Return the suite on the card
- *get_card_value*
    1. Get the value of the card of the parameter
    2. If the value of the parameter is NULL, return 0
    3. If the number on the card is bigger than 11 return 10, otherwise return the number on the card
- *delete_card*
1. If the value on the parameter is not NULL, free the parameter

*Hand structure and functions*
- *create_hand*
    1. If error, return NULL
    2. If not, allocate memory for a hand by malloc
- *insert_card*
    1. If error, return 0
    2. for loop : 
        1) start at first card and move through the array
        2) if slot is empty, put card in it
        3) add value to total sum
        4) return 1, if card is entered once
    3. return 0, if all 11 cards are full
- *get_hand_points*
    1. Return sum of all cards in hand
    2. return 0, if error or NULL
- *print_hand*
    1. print num and suite of cards in the hand using for loop
- *delete_hand*
    1. If the parameter isn't empty, delete the hand and cards contained inside
    2. Free hand pointer

*Deck structure and functions*
- *create_deck*
    1. If error, return NULL
    2. Using for loop, create 52 cards using create_card for a deck
    3. Return a deck struct
- *shuffle_deck*
    1. if the parameter is not NULL, shuffles the deck randomly
    2. Return nothing but the order of the cards of the deck will be changed
- *draw_deck*
    1. If deck is NULL or out of cards, return NULL
    2. Create a copy of the top card of the deck
    3. Delete the top card of the deck
- *get_deck_count*
    1. Return the number of cards in the deck
    2. If error, return 0
- *print_deck*
    1. If the pararmeter is not NULL, print a num and a suite of the cards in the deck
- *delete_deck*
    1. Deletes any cards left in a deck, as well as deck itself
    2. Frees associated pointers


#### Network module:
*Dealer interactions with network (dealer_network.c)*
- *connect_dealer_network*
    1. Setting up socket
    2. Connecting to port
    3. Binding to port
    4. Listening 
    5. Accepting
    6. print the messege, if the setting up is done correctly
    7. If error, EXIT_FAILURE. Otherwise, return socket number
- *send_begin*
    1. Read the messege provided by socket number
    2. Check the file content if "join" or "JOIN" in the file that player sent
    3. If JOIN is in the file, prase a player name in the file
    4. Send BEGIN to player
    5. Function return NULL if error, otherwise reuturn a player name
- *translate_card*
    1.
- *send_card*
    1. Send concatenate string to the player 
    2. Concatenate string "CARD" and the string of return value of translate_card
    3. Send the concatenate string to the player
    4. Function return FALSE if error, otherwise reuturn TRUE
- *send_dealer_card*
    1. Concatenate string "DEALER" and the string of return value of translate_card
    2.Send the concatenate string to the player
    3. Function return FALSE if error, otherwise reuturn TRUE
- *prompt_decision*
    1. Send "DECISION" to the player
    2. Read the string, the decision player made
    3. If there's the string "HIT" in the file, return 1, the string "STAND" in the file, return 2, return 0 if error.
- *send_result*
    1. If result == 1, send "WIN" to the player
    2. If result == 2, send "lOSE" to the player
    3. If result == 3, send "PUSH" to the player
    4. If succeed, return true. Return 0 if error
- *quit_game*
    1. Close the socket
    2. Shutdown the server
    3. print the message "Quit Game"
    4. If succeed, return true

*Player interactions with network (player_network.c)*
- *begin_game*
    1. Creating socket
    2. Setting up socket
    3. Send a concatenate string : "JOIN playername"
    4. print the succeed msg
    5. free the buffer
    6. Return the socket number
- *player_listen*
    1. If "BEGIN" or "begin", info->command = 1
    2. If "CARD" or "card", info->command =2
    3. If "DEALER" or "dealer", info->command=3
    4. If "DECISION" or "decision", info->command=4
    5. If "RESULT" or "result", info->command =5

    6. If info->command 3 or 4 :
        1) Convert a card info from string to int
        2) save these converted ints to create_card()

    7. If info->commannd 5:
        1) convert result from string to int
        2) If win -> info->result =1
        3) If lose -> info->result =2
        4) If push -> info->result =3
    
    8. return structure

- *send_decision*
    1. If decision == 1 : send HIT to the dealer
    2. If decision == 2 : send STAND to the dealer
    3. Return false if error, otherwise return true

- *end_game*
    1. Close the socket
    2. Shutdown the server
    3. print the message "Quit Game"
    4. If succeed, return true

#### Dealer module:
- *main* - driver of the program: begins with command line arguments, ends with QUIT message
    1. parse arguments
    2. call network functions to set up connection
    3. call play() function num_games times
    4. send quit message
* NOTE: *main* expects ONE argument: the number of games to be played

- *play* - plays ONE game of 21
    1. create & shuffle deck
    2. create 2 hands for player, 2 hands for dealer
    3. deal first 2 cards in each hand
    4. while the player is still hitting and has not busted,
        1) deal cards
        2) decide if the dealer hits as you go
    5. calculate who won and send the result
    6. free hands and deck

- *calculate_result* - given current hand information, decides if player or dealer win (or PUSH)
    1. if the dealer busts, the player wins (because we know the player hasn't busted at this point)
    2. get the total score of all 4 hands
    3. pick the top player hand and top dealer hand
    4. decide if the player or dealer win/push/lose with </> statments
    
- *adding card to hands*
    1. if the card is not an ace
        1) add it to both hands (ex. player_11_hand & player_1_hand)
    2. else 
        1) if the hand has an ace already
        2)    insert the card into both hands as a 1
        3) else // the hand does not have an ace yet
        4)    insert the card into the "11" hand as an 11
        5)    insert the card into the "1" hand as a 1


#### Player module:
##### Primary Functions*

*main* - driver of the program: begins with command line arguments, ends after recieving QUIT message from dealer (command 6 from network)
    parse arguments - ensure that text file can be written to to save training data or read from to dowload traing data
    call network functions to set up connection
    if network connects and recieve "BEGIN" from dealer turn gamOn bool to true and run gameplay loop until it is false
    recieve "QUIT" message from dealer change gameOn to false exit gameplay loop 
* 

*inside the gameplay loop* 
    call networks player_listen and recieve a heard_info_t structure
    get one of command:
        2 - CARD give card to player
        3 - DEALER give card to dealer
        4 - DECISION make a decision (hit or stand)
        5 - RESULT save result from round prepare for new round
        6 - QUIT end game free memory
*

> Playing mode
    -*CARD*
        1. get integer value (points of card recieved) 
        2. pass that value into a hand_t structure named myHand
        3. Updated the total points of my hand (add the point value of the new card)
    -*DEALER*
        1. get integer value (points of card recieved) 
        2. pass that value into a hand_t structure named dealerHand
        3. Updated the total points of my hand (add the point value of the new card)
    -*DECISION*
        1. call getDeccision func
            1) take in myHand and dealerHand
            2) get the probablity (of winning the hand) value in the table for hit and stand given these hands
            3) return a descicion hit (1) or stand (2) based on whichever probability is greater
        2. send decision to the dealer
    -*RESULT*
        1. delete hands
        2. make new empty hands
    -*QUIT*
        1. delete hands
        2 turn gameOn to false
    
> Training mode
    -*CARD*
        1. get integer value (points of card recieved) 
        2. pass that value into a hand_t structure named myHand
        3. Updated the total points of my hand (add the point value of the new card)
    -*DEALER*
        1. get integer value (points of card recieved) 
        2. pass that value into a hand_t structure named dealerHand
        3. Updated the total points of my hand (add the point value of the new card)
    -*DECISION*
        1. call getDeccision func
            1) return a decison hit(1) or stand(2) randomly
        2. save the decision you made and what hands each side had to handHistory for later use in training the tables
        3. send decision to dealer
    -*RESULT*
        1. get the result
            1) set a training reward: win (1) loss (-1) push (0)
        2. save result and hand history into trasining tables
            1) for each event n hand history 
            2) visit its location in the table
            3) for count table incriment the number +1 (show we made a decison x times at a ccertiant hand event)
            4) for the prob table incriment the probability of victory at this event for this decision based on the new count
    -*QUIT*
        1. save training results (tables) to a text file
        2. delete hand hstory and hands
        3. turn gameOn to false and exit gameply loop

*NOTE: hand history*
    data structure within player that is used to train the tables
    collects deatils of every decicion made so that they can be 
    used to train the tables one we get the result (win loss or 
    push) of those decisions
    handHistory is a linked list of nodes holding player card 
    values dealer card values and the decision made at that event
*

### Data structures

There are a variety of data structures to aid communcation between the modules. The following data structures will be passed between one module and another: card_t, hand_t, deck_t, and heard_info_t.

> card_t
    This structure contains the information for one card, and is contained in the hand and the deck.

> hand_t
    This structure contains the information for eleven cards, and can have a card inserted into it. It is used by both the player and the dealer to keep track of the cards in each person's "hand".

> deck_t
    This structure is initialized with 52 cards, which can be randomly arranged in the array ("shuffled") or removed ("drawn"). It is mainly used by the dealer to manage gameplay.

> heard_info_t
    This structure is used by the network to pass information to the player. This allows the player to parse through a variety of types of information that may be recieved.

### Testing plan

See TESTING.md for a more in-depth description of our testing plan.

> Unit testing:
    For both the cards module and the network module, we will write a testing script contained in an independent tests directory. These scripts will cycle through a variety of tests verifying that the modules work independently of the dealer and player. These tests will also serve as templates for the dealer and player to use in their implementation.

> Integration testing:
    Once the network and cards modules have been thoroughly tested, we will test the player network communication by playing it against the provided pre-complied dealer. The dealer can be tested without network functionality. When we are confident in both of these files, we will combine them together and test with valgrind, starting with a smaller number of games increasing to a larger number of games, keeping track of the logic decisions made and network messages recieved and handled.