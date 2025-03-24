#include "Player.h"
#include <iostream>

// Constructor - initializes player with name and starting balance
Player::Player(const std::string& playerName, int startingBalance)
    : name(playerName), balance(startingBalance), currentBet(0), isStanding(false) {
}

// Reset player's hand and state for a new round
void Player::reset() {
    playerHand.clear();
    currentBet = 0;
    isStanding = false;
}

// Place a bet, subtracting it from the player's balance
void Player::placeBet(int amount) {
    if (amount > balance) {
        std::cerr << "Not enough balance to place this bet\n";
        return;
    }
    currentBet = amount;
    balance -= amount;
}

// Add a card to the player's hand (if not standing)
void Player::hit(Deck& deck) {
    if (!isStanding) {
        playerHand.addCard(deck.dealCard());
    }
}

// Player chooses to stand
void Player::stand() {
    isStanding = true;
}

// Check if the player has busted
bool Player::isBusted() const {
    return playerHand.getTotalValue() > 21;
}

// Check for natural blackjack (exactly two cards totaling 21)
bool Player::hasBlackjack() const {
    return playerHand.getTotalValue() == 21 &&
        playerHand.toString().find(",") == std::string::npos;
}

// Add winnings to player's balance
void Player::addWinnings(int amount) {
    balance += amount;
}

// Get player's name
std::string Player::getName() const {
    return name;
}

// Get current balance
int Player::getBalance() const {
    return balance;
}

// Get the amount currently bet
int Player::getCurrentBet() const {
    return currentBet;
}

// Convert player's status to string
std::string Player::toString() const {
    return name + "'s Hand: " + playerHand.toString() + " | Balance: $" + std::to_string(balance);
}
