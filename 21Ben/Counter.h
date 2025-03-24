#pragma once

#include <string>
#include "Card.h"

// Class to manage the running count of cards
// using a card counting strategy like Hi-Lo
class Counter {
private:
    int count;               // Running count
    std::string strategy;    // Name of the counting strategy (e.g., "Hi-Lo")

public:
    // Constructor
    Counter();

    // Get the current count
    int getCount() const;

    // Return the count value of a given card (based on strategy)
    int getCardValue(Card card);

    // Adjust the running count using Hi-Lo logic
    void modifyCountHiLo(Card card);

    // Get the current strategy name
    std::string getStrategy();

    // Set a new strategy name
    void setStrategy(std::string newStrategy);
};
