#include "SimPlayer.h"
#include <iostream>

SimPlayer::SimPlayer(const std::string& playerName, int startingBalance)
    : name(playerName), currentHandIndex(0)
{
    Balance[0] = startingBalance, Balance[1] = startingBalance;
    Bet[0] = 0, Bet[1] = 0;
}


void SimPlayer::reset() {
    hands.clear();
    Hand newHand;
    hands.push_back(newHand);
    currentHandIndex = 0;
    Bet[0] = 0, Bet[1] = 0;
}

bool SimPlayer::placeBet(long long amount, int whichBank) {
    if (amount > Balance[whichBank]) {
        std::cerr << "Not enough balance to place this bet\n";
        return false;
    }
    Bet[whichBank] = amount;
    Balance[whichBank] -= amount;
    return true;
}

Card SimPlayer::hit(Deck& deck) {
    Card card = deck.dealCard();
    hands[currentHandIndex].addCard(card);
    return card;
}


bool SimPlayer::isBusted() const {
    return hands[currentHandIndex].getTotalValue() > 21;
}

bool SimPlayer::hasBlackjack() const {
    return hands[currentHandIndex].isBlackjack();
}

void SimPlayer::addWinnings(int amount, int whichBank) {
    Balance[whichBank] += amount;
}

std::string SimPlayer::getName() const {
    return name;
}

long long SimPlayer::getBalance(int whichBank) const {
    return Balance[whichBank];
}

long long SimPlayer::getCurrentBet(int whichBet) const {
    return Bet[whichBet];
}

Hand& SimPlayer::getCurrentHand() {
    return hands[currentHandIndex];
}

const Hand& SimPlayer::getCurrentHand() const {
    return hands[currentHandIndex];
}

const std::vector<Hand>& SimPlayer::getHands() const {
    return hands;
}

bool SimPlayer::split() {
    if (hands[currentHandIndex].isPair()) {
        if (Bet[0] > Balance[0]) {
            std::cerr << "Not enough balance to split\n";
            return false;
        }
        //causes issues assuming one balance can split and the other cant
        Balance[0] -= Bet[0];
        Balance[1] -= Bet[1];
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

bool SimPlayer::advanceHand() {
    if (currentHandIndex + 1 < hands.size()) {
        currentHandIndex++;
        return true;
    }
    return false;
}

bool SimPlayer::allHandsPlayed() const {
    return currentHandIndex >= hands.size() - 1;
}

int SimPlayer::getTotalHands() const {
    return hands.size();
}

void SimPlayer::setBank(long long bank) {
    Balance[0] = bank;
    Balance[1] = bank;
}
/*
bool doubleDown(Deck& deck) {
    if ()
}*/