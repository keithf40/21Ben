#include "Player.h"
#include <iostream>

Player::Player(const std::string& playerName, int startingBalance)
    : name(playerName), balance(startingBalance), currentBet(0), currentHandIndex(0)
{
}

void Player::reset() {
    hands.clear();
    Hand newHand;
    hands.push_back(newHand);
    currentHandIndex = 0;
    currentBet = 0;
}

void Player::placeBet(int amount) {
    if (amount > balance) {
        std::cerr << "Not enough balance to place this bet\n";
        return;
    }
    currentBet = amount;
    balance -= amount;
}

Card Player::hit(Deck& deck, Counter& counter) {
    Card card = deck.dealCard();
    hands[currentHandIndex].addCard(card);
    counter.modifyCount(card);
    return card;
}

void Player::stand() {
    // No flag is used; game logic calls advanceHand() as needed.
}

bool Player::isBusted() const {
    return hands[currentHandIndex].getTotalValue() > 21;
}

bool Player::hasBlackjack() const {
    return hands[currentHandIndex].isBlackjack();
}

void Player::addWinnings(int amount) {
    balance += amount;
}

std::string Player::getName() const {
    return name;
}

int Player::getBalance() const {
    return balance;
}

int Player::getCurrentBet() const {
    return currentBet;
}

std::string Player::toString() const {
    std::string result = name + "'s Hands:\n";
    for (size_t i = 0; i < hands.size(); i++) {
        result += "Hand " + std::to_string(i + 1) + ": " + hands[i].toString() + "\n";
    }
    result += "Balance: $" + std::to_string(balance);
    return result;
}

Hand& Player::getCurrentHand() {
    return hands[currentHandIndex];
}

const Hand& Player::getCurrentHand() const {
    return hands[currentHandIndex];
}

const std::vector<Hand>& Player::getHands() const {
    return hands;
}

bool Player::split() {
    if (hands[currentHandIndex].isPair()) {
        if (currentBet > balance) {
            std::cerr << "Not enough balance to split\n";
            return false;
        }
        balance -= currentBet; // Deduct additional bet for new hand.
        // Create a new hand with the removed card.
        Hand newHand;
        newHand.addCard(hands[currentHandIndex].getCards().back());
        // Add the new hand.
        hands.push_back(newHand);
        // Remove the second card from the current hand.
        hands[currentHandIndex].removeLastCard();
        return true;
    }
    return false;
}

bool Player::advanceHand() {
    if (currentHandIndex + 1 < hands.size()) {
        currentHandIndex++;
        return true;
    }
    return false;
}

bool Player::allHandsPlayed() const {
    return currentHandIndex >= hands.size() - 1;
}

int Player::getTotalHands() const {
    return hands.size();
}

void Player::setBank(int bank) {
    this->balance = bank;
}