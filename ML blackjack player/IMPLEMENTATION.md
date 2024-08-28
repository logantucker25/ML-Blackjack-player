# CS50 Final Project
## Implementation Specification

The PROJECT-TEAM-13 is a standalone program that proceeds the game "Blackjack" between a dealer and a player through a server. They will communicate through the server using a terminal.

### Data structures

As mentioned in DESIGN.md, we implement a variety of data structures to aid communication between the modules.

- card_t
    This structure contains the information for one card, and is contained in the hand and the deck.

- hand_t
    This structure contains the information for eleven cards, and can have a card inserted into it. It is used by both the player and the dealer to keep track of the cards in each person's "hand".

- deck_t
    This structure is initialized with 52 cards, which can be randomly arranged in the array ("shuffled") or removed ("drawn"). It is mainly used by the dealer to manage gameplay.

- heard_info_t
    This structure is used by the network to pass information to the player. This allows the player to parse through a variety of types of information that may be recieved.

## Dealer control flow
- Get command line arguments and validate them in `main()`
- play num_games games, handled by `play()`
- while in `play()`, as long as the player responds with HIT to `prompt_decision()`, deal cards until they BUST or STAND
    * DEALING CARDS: Soft ace handled in `add_dealer_card` and `add_player_card`, which add freshly dealt cards to hands
- `calculate_result()` and send the result
- Send QUIT message
- clean up memory

## Player control flow
- Get command line arguments and validate them in `main()`
- play games until the dealer sends quit message
- while in game play loop 
    - track cards given to both dealer and player
    make decision hit or stand when prompted by dealer
- clean up

### SOFT ACE NOTE
The dealer uses 4 hands to keep track of the floating ace. 2 for the dealer and 2 for the player. If an ace is dealt to player/dealer, one hand will consider that ace an 11, and the other will consider that ace a 1. Every successive ace will be treated as a 1. The highest value hand of the two (that does not bust) will be used in the final win evaluation.

## Other modules

### Cards module
```c
// function prototype declarations
card_t* create_card(int number_in, int suite_in);   // creates a card structure
int get_card_num(card_t* my_card);                  // returns the number of a card
int get_card_suite(card_t* my_card);                // returns the suite of a card
int get_card_value(card_t* my_card);                // returns the value of a card
void delete_card(card_t*);                          // deletes the card
```

### Hand module
```c
// function prototype declarations
hand_t* create_hand();                              // creates a hand structure
bool insert_card(card_t* my_card, hand_t* my_hand); // inserts a card into the hand structure
int get_hand_points(hand_t* my_hand);               // returns the total points in a hand structure
void print_hand(hand_t* my_hand);                   // prints the hand
void delete_hand(hand_t* my_hand);                  // deletes the hand and all cards contained
```

### Deck module
```c
// function prototype declarations
deck_t* create_deck(void);              // creates a deck structure
void shuffle_deck(deck_t* my_deck);     // shuffles the deck
card_t* draw_deck(deck_t* my_deck);     // draws the top card from the deck
int get_deck_count(deck_t* my_deck);    // returns the number of cards in the deck
void print_deck(deck_t* my_deck);       // prints the cards in a deck
void delete_deck(deck_t* my_deck);      // deletes the deck structure and all cards contained
```

### Network
### Dealer network module
```c
// function prototype declarations
int connect_dealer_network(void);                   // sets up a socket connection
char* send_begin(int new_socket);                   // waits for JOIN, returns player name and sends BEGIN
bool send_card(int new_socket,card_t* my_card);     // sends CARD and the card in the provided structure
bool send_dealer_card(int new_socket, card_t* my_card);     //sends DEALER and the card in the provided structure
int prompt_decision(int new_socket);                // sends DECISION and waits for a decision
bool send_result(int new_socket, int result);       // sends RESULT along with provided result
bool quit_game(int new_socket);                      // quits the game

//local function
char* translate_card(card_t* mycard);       //translates the card structure to necessary format ("Ace of Clubs")
```

### Player network module

```c
// function prototype declarations
int begin_game(char* playername);                   // connects to socket
heard_info_t* player_listen(int socket_number);     // listens for message from dealer and returns in heard_info_t structure
bool send_decision(int socket_number, int decision);    // sends a decision, where decision = 1 means HIT and decision = 2 means STAND
bool end_game(int socket_number);                   // closes connection
int get_heard_command(heard_info_t* info);          // returns the command saved in structure
card_t* get_heard_card(heard_info_t* info);         // returns the card saved in structure
int get_heard_result(heard_info_t* info);           // returns the result saved in structure
void delete_heard_info(heard_info_t* info);         // deletes the heard info structure
void print_heard_info(heard_info_t* info);          // prints the heard info structure for testing
```

## List of Function Prototypes

### Dealer

```c
static void parse_args(const int argc, char* argv[], int* num_games); // confirms that the arguments are valid
static void play(int new_socket); // plays one game of 21 and frees memory
static void quit(int new_socket); // sends quit message to the network/player
static void setup_game(deck_t* deck, hand_t* player_11_hand, hand_t* player_1_hand, hand_t* dealer_11_hand, hand_t* dealer_1_hand, int new_socket); // deals starting cards
static bool will_dealer_hit(hand_t* dealer_hand); // returns true if the dealer hand value is less than 17
static bool check_bust(hand_t* hand); // returns true if the given hand has busted
int calculate_result(deck_t* deck, hand_t* player_11_hand, hand_t* player_1_hand, hand_t* dealer_11_hand, hand_t* dealer_1_hand); // returns 1 if player won, 2 if player lost, 3 if PUSH
static void add_player_card(deck_t* deck, hand_t* player_11_hand, hand_t* player_1_hand, int new_socket); // adds newly dealt card to player hands
static void add_dealer_card(deck_t* deck, hand_t* dealer_11_hand, hand_t* dealer_1_hand, int new_socket); // adds newly dealt card to dealer hands
```


### Player
```c
static handEvent_t* handEvent_new(int player, int dealer, int decision);// create a new hand event and set values to given ints
handHistory_t* handHistory_new(void); // create a new hand history (linked list of hand events), occurs at the start of games and start of new rounds
void handHistory_insert(handHistory_t* handHistory, int player, int dealer, int decision); // insert a hand event into a given hand history
void inputHandHistoryToTable(handHistory_t* handHistory, int reward); // after a game has finished save the hand history and outcome (reward) to the data tables
void handHistory_delete(handHistory_t* handHistory); // delete hand history and its contents
```

```c
static void parseArgs(const int argc, char* argv[]);// check if args from command line are valid
void noteResult(int result, handHistory_t* handHistory); // update the training data chart based on round just played
int getNumTokens(char* string); // (train helper) this helper finds the number of words in a line of text and is used to minmize the amount of slots we make in arrays
void train(char* trainingData); // download training data from a text file to tables
void saveTraining(char* trainingData); // save newly recorded training table into a text file
void giveCard(hand_t* hand, card_t* card); // give a new card to a specified hand
void save_decision(handHistory_t* handHistory, hand_t* myHand, hand_t* dealerHand, int decision); // if you make a decision call this function to save the evemnt into hand history 
int getDecision(hand_t* myHand, hand_t* dealerHand); / make an optimal hit or stand decision based on current hand values
int main(const int argc, char* argv[]); // run game play loop
```

