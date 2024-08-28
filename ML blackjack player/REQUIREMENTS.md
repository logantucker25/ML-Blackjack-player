# CS50 Final Project
## Requirements Spec

The PROJECT-TEAM-13 is a standalone program that proceeds the game "Blackjack" between a dealer and a player through a server. They will communicate through the server using a terminal.

### Cards

Cards module
    - Provides functionality for a single card
    - Must be able to create and delete cards, as well as interact with them
Hand module
    - Contains functionality of a hand of 11 cards
    - Must be able to create and delete a hand, as well as add cards to them
Deck module
    - Contains functionality of a deck of 52 cards
    - Must be able to create a deck and initialize it with the necessary 52 cards, as well as delete it and all cards contained.
    - Must be able to shuffle and draw from deck

### Network

Dealer network module
    - Must be able to set up network
    - Must recieve JOIN and return playername
    - Must send BEGIN once JOIN is recieved
    - Send CARD followed by description of card ("Three of Hearts")
    - Send DEALER followed by description of card ("Three of Hearts")
    - Send DECISION and return recieved decision
    - Send RESULT followed by provided result ("WIN")
    - Close network

Player network module
    - Must be able to connect to network
    - Must send JOIN followed by playername, and wait for BEGIN to be returned
    - Must recieve commands from dealer network module and parse information for player module
    - Close network

### Dealer

Must follow the rules playing as dealer, and accomodate a single player.
    - Run from the command line
    - Create and shuffle deck of cards (using Cards module)
    - Deal cards to the player (using Network module)
    - Receive HIT/STAND decisions from the client (using Network module)
    - Calculate the game’s result (WIN, LOOSE, BUST, or PUSH) and send a message to the client (using Network module)
    - Clean up memory and play again (until you've played the # of games that the caller specifies)

### Player

Must follow the rules playing as player
Must implement a training mode and a playing mode
    - Runs from the command line, taking player name
    - In training mode, plays games against the dealer program
        - Chooses random actions in each state
        - Writes tables to disk to record actions chosen, and reads them back
    - In playing mode, makes decisions on what it learned from training
        - Reads table from disk
        - Uses table to make decisions about what to do