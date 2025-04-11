#include <iostream>
#include "Dealer.h"

// Clears the dealer's hand
void Dealer::clear() {
    hand.clear();
}

// Dealer draws one card from the deck
Card Dealer::hit(Deck& deck, Counter& counter, bool faceDown) {
    Card card = deck.dealCard();
    hand.addCard(card);
    if (!faceDown) counter.modifyCount(card);
    return card;
}

// Dealer keeps hitting until hand total is 17 or more
int Dealer::dealerTurn(Deck& deck, Counter& counter) {
    while (hand.getTotalValue() < 17) {
        hit(deck, counter, false);
    }
    return hand.getTotalValue();
}

// Checks if the dealer has a natural blackjack (exactly 2 cards: Ace + 10-value)
bool Dealer::checkBlackjack() const {
    return hand.isBlackjack();
}

const Hand& Dealer::getHand() const {
    return hand;
}
