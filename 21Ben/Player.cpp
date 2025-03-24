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

void Player::hit(Deck& deck) {
    hands[currentHandIndex].addCard(deck.dealCard());
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
    // Can only split if current hand has exactly 2 cards and they form a pair.
    if (hands[currentHandIndex].getCards().size() == 2 && hands[currentHandIndex].isPair()) {
        // Check if player has enough balance to double the bet.
        if (currentBet > balance) {
            std::cerr << "Not enough balance to split\n";
            return false;
        }
        balance -= currentBet; // Deduct additional bet for new hand.
        // Remove the second card from the current hand.
        Card secondCard = hands[currentHandIndex].getCards().back();
        hands[currentHandIndex].removeLastCard();
        // Create a new hand with the removed card.
        Hand newHand;
        newHand.addCard(secondCard);
        // Add the new hand.
        hands.push_back(newHand);
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
    return currentHandIndex >= static_cast<int>(hands.size()) - 1;
}
