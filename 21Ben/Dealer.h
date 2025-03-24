#pragma once

#include <string>
#include "Deck.h"
#include "Hand.h"

// Class representing the dealer in a Blackjack game
class Dealer {
private:
    Hand hand;  // Dealer's hand of cards

public:
    // Executes the dealer's turn logic; returns hand total
    int dealerTurn(Deck& deck);

    // Dealer draws one card from the deck
    void hit(Deck& deck);

    // Clears the dealer's hand
    void clear();

    // Checks if the dealer has a natural blackjack (Ace + 10/J/Q/K)
    bool checkBlackjack() const;
};
