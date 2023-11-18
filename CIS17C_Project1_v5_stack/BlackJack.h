/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BlackJack.h
 * Author: error
 *
 * Created on December 7, 2022, 9:05 PM
 */

#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <list>
#include <set>
#include <map>
#include <stack>

using namespace std;

// structs for error exceptions.
struct BadSuit {};
struct BadNumberDecks {};

// Some global constants.
const int PLAYER_CHIP = 100;	// Initial number of player's chips.
const int DEALER_CHIP = 10000;	// Initial number of dealer's chips.
const int MAX_BET = 5;		// Maximum number of chips for a bet in a round.
const int MAX_CHARACTER = 1000;	// Maximum number of characters in a line
				// 	to be used for standard input.


// Class that represents a card. It needs a value (rank) and a suit.
class Card {
    private:
        int num;	// The value (rank) of the card.
        char suit;	// The suit of the card (c, s, h, d).
    public:
        // Constructor.
        // num should be in the range of 1<= num <=13 
        // 	(1: Ace, 11:J, 12:Q, 13:K),
        // suits: 'c' (club), 's' (spade), 'h' (heart), 'd' (diamond).
        Card(int num1 = 1, char suit1 = 's') {
            // Assigning the rank of the card using a number.
            if (num1 < 1) num = 1;
            else if (num1 > 13) num = 13;
            else num=num1 ;
            // Assigning the suit of the card.
            if (suit1 == 'c' || suit1 == 'C') suit = 'c';
            else if (suit1 == 's' || suit1 == 'S') suit = 's';
            else if (suit1 == 'h' || suit1 == 'H') suit = 'h';
            else if (suit1 == 'd' || suit1 == 'D') suit = 'd';
            else throw BadSuit(); // Error exception for bad input.
        }

        // Get the value of the card (1~13).
        int getValue() const {
                return num;
        }

        // Get the rank of the card (A, 2,..., 10, J, Q, K) as strings.
        string getRank() const {
            if (num == 1) return "A";
            else if (num == 2) return "2";
            else if (num == 3) return "3";
            else if (num == 4) return "4";
            else if (num == 5) return "5";
            else if (num == 6) return "6";
            else if (num == 7) return "7";
            else if (num == 8) return "8";
            else if (num == 9) return "9";
            else if (num == 10) return "10";
            else if (num == 11) return "J";
            else if (num == 12) return "Q";
            else if (num == 13) return "K";
        }

        // Get the suit of the given card.
        char getSuit() const {
            return suit;
        }
};		

// outstream operator overloading for the Card class.
ostream & operator<<(ostream &s, const Card &c) {
    return s << c.getRank() << "(" << c.getSuit() << ')';
}

// Class that represents the decks of cards for the game of blackjack.
// Number of decks allowed here are 1, 2, and 4.		
class Decks {
private:
    list<Card> cards;  // Using list for the cards
    list<Card>::iterator current; // Iterator for the current card
    stack<Card> cardStack; // Stack of cards for dealing

public:
    // Constructor. The number of decks should be given (default=1).
    Decks(int nDeck = 1) {
        create(nDeck); // create all cards.
    }

    // Create all cards of given number of decks.
    void create(int nDeck) {
        if (nDeck != 1 && nDeck != 2 && nDeck != 4) 
            throw BadNumberDecks();

        cards.clear(); // Clear existing cards
        while (!cardStack.empty()) {
            cardStack.pop(); // Clear the stack as well
        }

        for (int i = 0; i < nDeck; i++) {
            // Creating and storing cards for nDeck decks.
            for (int n = 1; n <= 13; n++) { 
                cards.push_back(Card(n, 'c'));
                cards.push_back(Card(n, 's'));
                cards.push_back(Card(n, 'h'));
                cards.push_back(Card(n, 'd'));
            }
        }
        shuffle(); // Shuffle the cards and fill the stack
    }

    // Randomly shuffle all cards in the deck and fill the stack.
    void shuffle() {
        vector<Card> tempDeck; // Temporary vector to hold cards for shuffling
        for (const Card& card : cards) {
            tempDeck.push_back(card);
        }

        random_shuffle(tempDeck.begin(), tempDeck.end()); // Shuffle the vector
        cards.assign(tempDeck.begin(), tempDeck.end()); // Assign back to the list
        current = cards.begin(); // Reset the current iterator

        // Now fill the stack with the shuffled cards
        for (const Card& card : tempDeck) {
            cardStack.push(card);
        }
    }

    // Deal a card from the top of the stack.
    Card dealStack() {
        if (cardStack.empty()) {
            throw runtime_error("Out of cards. Need to shuffle.");
        }
        Card dealtCard = cardStack.top();
        cardStack.pop();
        return dealtCard;
    }

    // Deal a card that is pointed to by 'current' and move to the next.
    Card deal() {
        if (current == cards.end()) {
            shuffle();
            current = cards.begin();
        }
        Card dealtCard = *current;
        ++current;
        return dealtCard;
    }

    // Print all cards at the current shuffled state.
    void print() const {
        for (list<Card>::const_iterator it = cards.begin(); it != cards.end(); ++it) {
            cout << *it << endl;
        }
    }
};


// Class that represents a hand of a player or a dealer.
class Hand {
    private:
        vector<Card> crdsAtHnd;	// Array of cards in a hand.
    public:
        // Constructor.
        Hand(): crdsAtHnd() {}
        
        //function isn't called because it causes come type of error in the code
        //so I destroy the array of cards in another function
        //this is just here for demonstration purposes
        ~Hand(){
            crdsAtHnd.clear();
        }

        // Add a card to the hand.
        void addCard(Card card) {
            crdsAtHnd.push_back(card);
        }

        int getValue() const {
            int value = 0;
            // true if an ace exists in the hand.
            bool ifAce = false;  
            for (int i = 0; i < crdsAtHnd.size(); i++) {
                int cardValue = crdsAtHnd[i].getValue();
                // Give 10 for all face cards (J, Q, K).
                if (cardValue > 10) cardValue = 10; 	
                if (cardValue == 1) ifAce=true;
                value += cardValue;
            }
            // Ace is always counted as 11 if doing so dose not 
            // make the hand bust (S17 rule for dealer's hand).
            if (ifAce && value < 12) value += 10;	
            return value;
        }

        // Return the number of cards of the hand.
        int size() const {
            return crdsAtHnd.size();
        }

        // Returns true if the hand is "blackjack" 
        // (getting the value 21 with 2 cards).
        bool blackjack() const {
            return (crdsAtHnd.size() == 2 && getValue() == 21);
        }

        // destructor for my vector
        void removeAllCards() {
            crdsAtHnd.clear();
        }

        // print the cards in the hand.
        // (if hideFirst==true, the first card will not be shown).
        void print(bool hideFirst=false) const {
            if (crdsAtHnd.size() == 0) 
                    cout << "No card." << endl;
            for (int i = 0; i < crdsAtHnd.size(); i++) 
                if (i == 0 && hideFirst) cout << "?(?) ";
                else cout << crdsAtHnd[i] << ' ';
            cout << endl;
        }
}; 

/* Class that represents the game (displaying texts using standard output, 
* 	and managing the flow of the game using 5 stages).
* User inputs are obtained also using standard input, and only the first
* 	character (excluding white spaces) of an input (a line ending with \n)
* 	is assumed to be the only user input to reduce confusions.
*/
class Game {
    private:
        Decks myDecks;	// All cards for the game.
        Hand playerHand, 
             dealerHand; // Hands for the game.
        set<char> suitTracker; // To track the suits of dealt cards
        map<int, string> roundOutcomes; // New round outcome tracker
        int roundNumber = 1;
        int nPlayerChip, 
            nDealerChip; // Numbers of chips for players.
        int nBet; 	// Bet of the current round.
        char temp[MAX_CHARACTER+1]; // C-string for temporary storages.
    public:
        // Constructor.
        Game(): myDecks(), 
            playerHand(), 
            dealerHand() {
            nPlayerChip = PLAYER_CHIP;
            nDealerChip = DEALER_CHIP;
        }

        // Manage the flow of the game.
        void play() {
            // Setting the random seed.
            srand(static_cast<unsigned int>(time(0))); 
            // Starting the game
            char input = beginGame();
            while (input == 'n') {
                beginRound(); // beginning of round
                do 
                {
                    input = inRound();
                } while (input == 'h');
                input = endRound(input);
            }
            endGame();
        }

        // Beginning of the game (stage 1).
        // Returns the input (n, r, q).
        // (n: next round, r: rule, q: quit.)
        char beginGame() {
                cout << "----------------------------" << endl;
                cout << "|  Welcome  to  Blackjack  |" << endl;
                cout << "----------------------------" << endl;
                cout << endl;
                
                int nDeck = 1;
                myDecks.create(nDeck); // Creating the decks of cards.
                cout << nDeck << " deck (52 cards) has";
                cout << " been created and shuffled." << endl;
                cout << "Your balance is: " << PLAYER_CHIP;
                cout << " chips now, and you can bet";
                cout << " upto " << MAX_BET;
                cout << " chips for each round." << endl << endl;

                char input; // For user input.
                do { // (will be repeated until a right input is given).
                    cout << "Type n for a new round, r for rules, ";
                    cout << "and q to quit [n/r/q] (default: n): ";
                    cin.getline(temp, MAX_CHARACTER);
                    // extract the first char.
                    input = firstChar(temp, MAX_CHARACTER); 
                    if (input == '0') input = 'n';
                    if (input == 'r') displayRules();
                } while (input != 'n' && input != 'q');
                cout << endl;
                return input;
        }
        
        void trackSuit(const Card& card) {
            suitTracker.insert(card.getSuit());
        }

        // Method to display suit statistics
        void displaySuitStatistics() {
            cout << "\nSuit Distribution Statistics:\n";
            for (char suit : suitTracker) {
                cout << "Suit " << suit << " appeared in the game.\n";
            }
        }

        // Beginning of a round (stage 2)
        // Returns the input (amount of bet; 1-5).
        void beginRound() {
                cout << "===================================" << endl;
                cout << "Starting a New Round (your chips: ";
                cout << nPlayerChip << ")." << endl << endl;

                bool noChips = false; 
                        // Used when checking (remaining chips) < nBet.
                do { // (will be repeated until a right input is given).
                    if (noChips) {
                            cout << "You only have " << nPlayerChip;
                            cout << " chip(s)." << endl; 
                    };
                    cout << "How many chips do you want to bet? ";
                    cout << "[1-5] (default: 1): ";
                    noChips = false; // Set it for a new input.

                    // Read a line and store in temp.
                    cin.getline(temp, MAX_CHARACTER); 
                    // extract a number from the first char.
                    nBet = firstChar(temp, MAX_CHARACTER) - '0';
                    if (nBet == 0) nBet = 1; // Default is 1.
                    // Check if chips are sufficient.
                    if (nBet > nPlayerChip || nBet > nDealerChip) 
                                noChips = true;
                } while (nBet < 1 || nBet > MAX_BET || noChips);
                cout << "You bet " << nBet << " chip";
                cout << (nBet == 1 ? "." : "s.") << endl;

                dealerHand.removeAllCards(); // return all cards
                playerHand.removeAllCards(); // return all cards
                myDecks.shuffle(); // Shuffle the cards.

                // Dealing two cards to each player.
                dealerHand.addCard(myDecks.deal());
                playerHand.addCard(myDecks.deal());
                dealerHand.addCard(myDecks.deal());
                playerHand.addCard(myDecks.deal());
        }

        char inRound() {
                int playerValue = playerHand.getValue();
                // Check for the blackjack.
                // Then stop the round automatically, and return 'j'
                if (playerHand.blackjack()) {
                        cout << "You got the blackjack!" << endl;
                        return 'j'; 
                };

                // Check for the player busting 
                // (if busted, stop the round and return 'b').
                if (playerValue > 21) return 'b'; 

                // Show hands (with dealer's first card hidden).
                showHands(true);	

                char input; // user input in this stage.
                do { // (will be repeated until a right input is given).
                        cout << "Type h for Hit, s for Stand, r for ";
                        cout << "rules, q to quit [h/s/r/q] ";
                        cout << "(default: h): ";
                        cin.getline(temp, MAX_CHARACTER);
                        // extract the first char.
                        input = firstChar(temp, MAX_CHARACTER);
                        // Default is h(Hit).
                        if (input == '0') input = 'h'; 
                        if (input == 'r') displayRules(); // for rules.
                } while (input != 'h' && input != 's' && input != 'q');

                // Add a card for "Hit".
                if (input == 'h') playerHand.addCard(myDecks.deal()); 
                trackSuit(myDecks.deal()); // tracks card suits
                return input;
        }

        // End of a round (stage 4)
        // (busted=true if the player got busted, false if not).
        char endRound(char in) {
            int playerValue = playerHand.getValue();
            string outcome; // Variable to store the outcome of the round

            if (in == 'b') { // if the player is busted
                cout << "\nYou have:\t ";
                playerHand.print();
                cout << "You got busted, and lost ";
                cout << nBet << " chips." << endl;
                takeChips(-nBet);
                outcome = "Lose - Busted";
            } else if (in == 'q') { // if the player quits the game
                cout << "\nYou lost " << nBet << " chips.";
                cout << endl;
                takeChips(-nBet);
                outcome = "Lose - Quit";
                return in;
            } else if (in == 'j') { // if the player gets the blackjack
                showHands(); // Show cards
                if (dealerHand.blackjack()) {
                    cout << "It is tied, and the bet is returned." << endl;
                    outcome = "Tie - Both Blackjack";
                } else {
                    cout << "You won, and gained ";
                    cout << nBet << " chips." << endl;
                    takeChips(nBet);
                    outcome = "Win - Blackjack";
                }
            } else { // other outcomes
                while (dealerHand.getValue() < 17) {
                    dealerHand.addCard(myDecks.deal());
                }
                showHands(); // Show cards

                int dealerValue = dealerHand.getValue();
                if (dealerValue > 21) { // Dealer gets busted
                    cout << "Dealer got busted, and you gained " << nBet << " chips." << endl;
                    takeChips(nBet);
                    outcome = "Win - Dealer Busted";
                } else if (dealerValue > playerValue) { // Dealer wins
                    if (dealerHand.blackjack()) {
                        cout << "Dealer got the blackjack!\n";
                    }
                    cout << "Dealer won, and you lost " << nBet << " chips." << endl;
                    takeChips(-nBet);
                    outcome = "Lose - Dealer Higher Value";
                } else if (dealerValue < playerValue) { // Player wins
                    cout << "You won, and you gained " << nBet << " chips." << endl;
                    takeChips(nBet);
                    outcome = "Win - Higher Value";
                } else { // A tie
                    if (dealerHand.blackjack()) {
                        cout << "Dealer got the blackjack! You lost " << nBet << " chips." << endl;
                        takeChips(-nBet);
                        outcome = "Lose - Dealer Blackjack Tie";
                    } else {
                        cout << "It is tied, and the bet is returned." << endl;
                        outcome = "Tie";
                    }
                }
            }

            // Update the map with the outcome
            roundOutcomes[roundNumber++] = outcome;

            cout << "\n* End of the Round (your chips: " << nPlayerChip << ")." << endl;
            cout << "===================================" << endl;

            // If all chips are used up, the game ends
            if (nPlayerChip == 0 || nDealerChip == 0) return 'q';

            char input; // Character for user input
            do {
                cout << endl << "Type n for a new round, r for rules, q to quit [n/r/q] (default: n): ";
                cin.getline(temp, MAX_CHARACTER);
                input = firstChar(temp, MAX_CHARACTER);
                if (input == '0') input = 'n';
                if (input == 'r') displayRules();
            } while (input != 'n' && input != 'q');

            return input;
        }

        
        void displayRoundOutcomes() const {
            cout << "\nRound Outcomes:\n";
            for (const pair<int, string>& pair : roundOutcomes) {
                cout << "Round " << pair.first << ": " << pair.second << "\n";
            }
        }

        void endGame() {
            cout << "\nYour remaining chips: " << nPlayerChip;
            cout << " (you ";
            int diff = nPlayerChip - PLAYER_CHIP;
            if (diff > 0) cout << "gained " << diff << " chips).";
            else if (diff < 0) 
                    cout << "lost " << -diff << " chips).";
            else cout << "have the same number of chips as started).";
            cout << endl << endl;
            cout << "---------------------------" << endl;
            cout << "     End of the Game     |" << endl;
            cout << "---------------------------" << endl;
            
            displaySuitStatistics(); // Display the suit statistics
            cout << endl;
            displayRoundOutcomes(); // Display the round outcomes
        }

        // Display rules.
        void displayRules() {
            cout << endl;
            cout << "===================================" << endl;
            cout << "Bet a min of 1 chip and a max of 5 chips, win up to twice your "<<
                    "original bet.\nGet 21 to win, or get a higher card value than " <<
                    "the dealer.\nYou can only lose 100% of your original bet, but " <<
                    "you can up to 200%."<< endl;
            cout << "===================================" << endl;
            cout << endl;
        }

        // A function that extracts a first character from a c-string 
        // (returns '0' if none exists).
        // temp: character array, size: size of temp.
        static char firstChar(char *temp, int size) {
            int i=0;
            while (i < size && temp[i] != '\0' && (temp[i] == ' '\
                    || temp[i] == '\t' || temp[i] == '\n'))
                    i++;
            // If there is no character in temp, return '0'.
            if (temp[i] == '\0' || i >= size) return '0';
            else return temp[i];
        }

        // Show hands of both players 
        // (if hideFirst==true, the first card will not be shown).
        void showHands(bool hideFirst = false) {
            cout << "\nDealer has: \t";
            dealerHand.print(hideFirst);
            cout << "You have:\t ";
            playerHand.print(); 
            cout << endl;
        }

        // Chips changing hands.
        // (amount > 0: dealer to player; < 0, player to dealer).
        void takeChips(int amount){
            nPlayerChip += amount;
            nDealerChip -= amount;
        }
        
};


#endif /* BLACKJACK_H */

