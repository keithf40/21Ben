#pragma once

#include <vector>
#include "Card.h"

// Class representing a deck of playing cards
class Deck {
private:
    std::vector<Card> cards;  // All cards in the deck
    int currentIndex;         // Index of the next card to be dealt
    int numOfDecks = 1;
public:
    // Constructor initializes and shuffles the deck
    Deck();

    Deck(int decks);
    // Resets the deck to a full 52-card set
    void reset();

    // Shuffles the deck randomly
    void shuffle();

    // Checks if the deck is empty
    bool isEmpty() const;

    // Deals the next card from the deck
    Card dealCard();

    // Gets amount of decks remaining in the shoe for optimal betting
    int getDecksRemaining() const;

    // checks to see if shoe is half or less and ready to shuffle
    bool shuffleReady();
};
