#pragma once

#include <string>
#include "Deck.h"
#include "Hand.h"
#include "Counter.h"

// Class representing the dealer in a Blackjack game
class Dealer {
private:
    Hand hand;  // Dealer's hand of cards

public:
    // Executes the dealer's turn logic; returns hand total
    int dealerTurn(Deck& deck, Counter& counter);

    // Dealer draws one card from the deck
    Card hit(Deck& deck, Counter& counter, bool faceDown);

    // Clears the dealer's hand
    void clear();

    // Checks if the dealer has a natural blackjack (Ace + 10/J/Q/K)
    bool checkBlackjack() const;

    // Added getter for hand for display purposes
    const Hand& getHand() const;
};
