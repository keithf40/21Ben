#include <iostream>
#include <string>
#include "Hand.h"

// Constructor
Hand::Hand() {}

// Clears all cards and resets counts
void Hand::clear() {
    cards.clear();
    aceCount = 0;
    totalCount = 0;
}

// Returns the current total value of the hand
int Hand::getTotalValue() const {
    return totalCount;
}

// Adds a card to the hand and updates count immediately
void Hand::addCard(const Card& card) {
    cards.push_back(card);
    int cardValue = static_cast<int>(card.getRank());

    if (card.getRank() >= Rank::Jack && card.getRank() <= Rank::King) {
        cardValue = 10;
    }
    else if (card.getRank() == Rank::Ace) {
        aceCount++;
        cardValue = 11;
    }

    totalCount += cardValue;

    // Adjust for soft Aces if total exceeds 21
    while (totalCount > 21 && aceCount > 0) {
        totalCount -= 10;
        aceCount--;
    }
}

// Returns true if the hand is over 21
bool Hand::isBust() const {
    return getTotalValue() > 21;
}

// Returns true if the hand is a natural blackjack (Ace + 10)
bool Hand::isBlackjack() const {
    return cards.size() == 2 && getTotalValue() == 21;
}

// Converts hand to a comma-separated string
std::string Hand::toString() const {
    std::string handString;
    for (const Card& card : cards) {
        handString += card.toString() + ", ";
    }
    return handString.empty() ? "Empty Hand" : handString.substr(0, handString.size() - 2);
}

// Returns true if the hand contains at least one Ace counted as 11
bool Hand::isSoft() const {
    return aceCount > 0;
}

// Returns true if the first two cards form a pair (same rank or same value)
bool Hand::isPair() const {
    if (cards.size() == 2) {
        int cardOne = static_cast<int>(cards[0].getRank());
        int cardTwo = static_cast<int>(cards[1].getRank());

        // Face cards count as 10 for pair logic
        if (cardOne > 10 && cardOne <= 13) cardOne = 10;
        if (cardTwo > 10 && cardTwo <= 13) cardTwo = 10;

        return cardOne == cardTwo;
    }
    return false;
}
