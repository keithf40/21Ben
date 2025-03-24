#include <iostream>
#include "Dealer.h"

// Clears the dealer's hand
void Dealer::clear() {
    hand.clear();
}

// Dealer draws one card from the deck
void Dealer::hit(Deck& deck) {
    hand.addCard(deck.dealCard());
}

// Dealer keeps hitting until hand total is 17 or more
int Dealer::dealerTurn(Deck& deck) {
    while (hand.getTotalValue() < 17) {
        hit(deck);
    }
    return hand.getTotalValue();
}

// Checks if the dealer has a natural blackjack (exactly 2 cards: Ace + 10-value)
bool Dealer::checkBlackjack() const {
    return hand.isBlackjack();
}
