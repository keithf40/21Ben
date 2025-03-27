#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include "Deck.h"

// Constructor initializes and shuffles a full deck
Deck::Deck() {
    reset();
};

// Sets the deck to a specific number of decks
Deck::Deck(int decks) {
    numOfDecks = decks;
    reset();
}

// Fills the deck with 52 cards and shuffles them
void Deck::reset() {
    cards.clear();
    currentIndex = 0;
    for (int i = 0; i < numOfDecks; i++) {
        for (int suit = 0; suit <= 3; ++suit) {
            for (int rank = 2; rank <= 14; ++rank) {
                Rank cardRank = static_cast<Rank>(rank);
                Suit cardSuit = static_cast<Suit>(suit);
                cards.emplace_back(cardRank, cardSuit);
            }
        }
    }
    shuffle();
}

// Shuffles the deck using a random number generator
void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
    currentIndex = 0;
}

// Returns true if no cards remain to be dealt
bool Deck::isEmpty() const {
    return currentIndex >= cards.size();
}

// Deals one card from the top of the deck
Card Deck::dealCard() {
    if (isEmpty()) {
        throw std::runtime_error("No more cards in the deck");
    }
    return cards[currentIndex++];
}

bool Deck::shuffleReady() {
    return cards.size() / 2 <= currentIndex;
}

int Deck::getDecksRemaining() const {
    float decksRemaining = cards.size() - currentIndex;
    return round(decksRemaining / 52);
}
