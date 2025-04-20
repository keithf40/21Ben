#pragma once

#include <string>
#include "Card.h"
#include <functional>
// Class to manage the running count of cards
// using a card counting strategy like Hi-Lo
class Counter {
private:
    int count;               // Running count
    std::string strategy;    // Name of the counting strategy (e.g., "Hi-Lo")
    std::function<int(Card&)> stratFunc;
public:
    // Constructor
    Counter();

    Counter(std::string strategy);

    // Get the current count
    int getCount() const;

    // Resets counter to 0
    void resetCount();

    // Adjust the running count using Hi-Lo logic
    void modifyCount(Card card);

    // Get the current strategy name
    std::string getStrategy();

    // Set a new strategy name
    void setStrategy(std::string newStrategy);

    int getBet(int decksRemaining, int bankroll, int minbet, bool sleuthMode);
};
// Card counting strategies
int HiLo(Card& card);

int KO(Card& card);

int HiOpt1(Card& card);

int HiOpt2(Card& card);

int Mentor(Card& card);

int Omega2(Card& card);

int RPC(Card& card);

int RAPC(Card& card);

int R14C(Card& card);

int WongHalves(Card& card);

int Zen(Card& card);

int None(Card& card);