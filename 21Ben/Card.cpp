#include <iostream>
#include <string>
#include "Card.h"

// Constructor initializes rank and suit
Card::Card(Rank r, Suit s) : rank(r), suit(s) {}

// Returns the rank of the card
Rank Card::getRank() const {
    return rank;
}

// Returns the suit of the card
Suit Card::getSuit() const {
    return suit;
}

// Converts the card to a readable string (e.g., "Queen of Hearts")
std::string Card::toString() const {
    static const std::string rankStrings[] = {
        "2", "3", "4", "5", "6", "7", "8", "9", "10",
        "Jack", "Queen", "King", "Ace"
    };
    static const std::string suitStrings[] = {
        "Clubs", "Diamonds", "Hearts", "Spades"
    };

    // Adjust index by subtracting 2 since enum Rank starts at 2
    return rankStrings[static_cast<int>(rank) - 2] + " of " + suitStrings[static_cast<int>(suit)];
}
