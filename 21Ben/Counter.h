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
};
// Card counting strategies
int HiLo(Card& card);

int KO(Card& card);
