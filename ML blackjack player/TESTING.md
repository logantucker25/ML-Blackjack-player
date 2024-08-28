# CS50 Final Project
## Testing Specification

The PROJECT-TEAM-13 is a standalone program that proceeds the game "Blackjack" between a dealer and a player through a server. They will communicate through the server using a terminal.

## Unit Testing

We tested the following units individually, using test scripts or other systems to ensure that everything was working before integrating them with the other modules. We tested the player module by running it against the pre-compiled dealer_given until we were able to integrate it into the project.

#### Cards module testing

The cards module is tested using the script test_cards in the directory tests. Smaller tests on the card structure were run prior to this testing script. The test_cards script tests the following:
    - Creates and prints a deck, thus verifying that intended cards are created.
    - Shuffles and prints the deck several times, verifying that shuffling does occur randomly
    - Draws from the deck, verifying that the card is removed and is indeed the top card
    - Inserts card into a hand, verifying that the hand stores accurately
    - Deleting hand and deck, confirming that all cards contained were deleted

See test_card_output.txt for the output of this program. When run with valgrind, we have no memory leaks.

#### Network module testing

The network module is tested using two scripts to be run at the same time: test_network and test_player_network. Since we use two network modules, it made the most sense to test these modules in tandem. As we build the modules together, we added to each of the test functions as we went. Run the test_network module first, and immediately run the test_player_network in another window. These test scripts check as follows:
    - Socket creation (dealer) and connection (player)
    - CARD sent (dealer) and CARD recieved (player)
    - DEALER sent (dealer) and DEALER recieved (player)
    - RESULT sent (dealer) and RESULT received (player)
    - DECISION send (dealer) and DECISION recieved (player)
    - Response to DECISION sent (player) and response received (dealer)
    - Closing socket and quitting game


See test_network_output.txt and test_player_network_output.txt for the output of these programs. When run with valgrind, we have no memory leaks.

#### Dealer module testing

The dealer module is tested as a unit as following:
    - Modified the dealerModule to take inputs from the command line and work without networking. 
    - To simulate the player, user would input "1" for hit or "2" for stand. This mimics the input we would get from dealer_network.
    - While our network modules were still being built, we played games one-at-a-time to plug memory leaks and ensure proper control flow.

## Integration testing

The first step of implementation testing was running the player module against the dealer_given to verify that network and player module integrate well. Next is to run the player module against the dealer module to verify that the dealer and player interact. Lastly, we increased the number of games to 10000 to verify that our system can handle many runs.

## Valgrind

We tested valgrind on unit tests as well as integration tests to catch errors early. We have found no memory leaks!

## Training Game Number
*We decided to train the Player with 10,000 games. We settled on this figure because the training duration was brief enough for us to wait for it to complete in one sitting, but thorough enough to give us data on most possible scenarios.*