#pragma once

#include <string>
#include "Deck.h"
#include "Hand.h"

// Class representing a player in Blackjack
class Player {
private:
    std::string name;    // Player's name
    Hand playerHand;     // Player's current hand
    int balance;         // Player's current money
    int currentBet;      // Current round bet
    bool isStanding;     // Whether the player has chosen to stand

public:
    // Constructor with player name and starting balance
    Player(const std::string& playerName, int startingBalance);

    // Resets player state for a new round
    void reset();

    // Places a bet for the round
    void placeBet(int amount);

    // Player draws a card from the deck
    void hit(Deck& deck);

    // Player stands (takes no more cards)
    void stand();

    // Checks if player has busted
    bool isBusted() const;

    // Checks if player has a natural blackjack
    bool hasBlackjack() const;

    // Adds winnings to balance
    void addWinnings(int amount);

    // Getters
    std::string getName() const;
    int getBalance() const;
    int getCurrentBet() const;

    // Returns a string describing the player's hand
    std::string toString() const;
};
