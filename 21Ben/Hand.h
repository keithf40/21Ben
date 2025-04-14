#pragma once

#include <string>
#include <vector>
#include "Card.h"

// Class representing a player's or dealer's hand in Blackjack
class Hand {
private:
    std::vector<Card> cards;  // Cards currently in the hand
    int totalCount = 0;       // Running total value of the hand
    int aceCount = 0;         // Number of Aces in the hand
    int bet = 0;
    bool busted = false;
public:
    // Constructor
    Hand();

    // Clears all cards from the hand
    void clear();

    // Returns the total value of the hand (adjusting Aces as needed)
    int getTotalValue() const;

    // Adds a card to the hand
    void addCard(const Card& card);

    // Returns true if the hand is over 21
    bool isBust();

    // Returns true if the hand is a natural blackjack (Ace + 10)
    bool isBlackjack() const;

    // Returns a string representation of the hand
    std::string toString() const;

    // Returns true if the hand is "soft" (contains Ace counted as 11)
    bool isSoft() const;

    // Returns true if the hand is a pair (first two cards have same rank or same 10-value for face cards)
    bool isPair() const;

    // Returns the vector of cards (for image display)
    const std::vector<Card>& getCards() const;

    // Removes the last card from the hand (used during split)
    void removeLastCard();

    bool isBusted() const;

    void addBet(int bet);

    int getBet() const;
};
