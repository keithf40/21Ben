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
    int cardValue = card.getValue();
    if (card.getRank() == Rank::Ace) {
        aceCount++;
    }
    totalCount += cardValue;

    // Adjust for soft Aces if total exceeds 21
    while (totalCount > 21 && aceCount > 0) {
        totalCount -= 10;
        aceCount--;
    }
}

// Returns true if the hand is over 21
bool Hand::isBust() {
    busted = getTotalValue() > 21;
    return busted;
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

// Returns true if the first two cards form a pair (by rank or by 10-value for face cards)
bool Hand::isPair() const {
    if (cards.size() == 2) {
        int cardOne = cards[0].getValue();
        int cardTwo = cards[1].getValue();
        return cardOne == cardTwo;
    }
    return false;
}

// Returns the vector of cards
const std::vector<Card>& Hand::getCards() const {
    return cards;
}

// Removes the last card from the hand and adjusts totalCount and aceCount accordingly.
void Hand::removeLastCard() {
    if (!cards.empty()) {
        Card last = cards.back();
        int cardValue = last.getValue();
        if (last.getRank() == Rank::Ace) {
            if (aceCount > 0)
                aceCount--;
        }
        totalCount -= cardValue;
        cards.pop_back();
    }
}

bool Hand::isBusted() const {
    return totalCount > 21;
}

void Hand::addBet(int bet) {
    this->bet = bet;
}

int Hand::getBet() const {
    return this->bet;
}