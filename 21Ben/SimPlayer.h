#pragma once

#include <string>
#include <vector>
#include "Deck.h"
#include "Hand.h"
#include "Counter.h"


class SimPlayer {
private:
    std::string name;
    std::vector<Hand> hands;
    int currentHandIndex;
    long long Balance[2];
    long long Bet[2];

public:

    SimPlayer(const std::string& playerName, int startingBalance);

    void reset();

    bool placeBet(long long amount, int counter);

    Card hit(Deck& deck);

    bool isBusted() const;

    bool hasBlackjack() const;

    void addWinnings(int amount, int whichBank);

    std::string getName() const;

    long long getBalance(int whichBank) const;

    long long getCurrentBet(int whichBet) const;

    int getTotalHands() const;

    Hand& getCurrentHand();

    const Hand& getCurrentHand() const;

    const std::vector<Hand>& getHands() const;

    bool split();

    bool advanceHand();

    bool allHandsPlayed() const;

    void setBank(long long bank);

    bool doubleDown(Deck& deck);
};