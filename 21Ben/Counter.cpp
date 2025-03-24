#include <iostream>
#include <string>
#include "Counter.h"

// Constructor: initializes count and default strategy
Counter::Counter() {
    count = 0;
    strategy = "Standard";
}

// Returns the current running count
int Counter::getCount() const {
    return count;
}

// Gets the value of a card based on the current counting strategy
int Counter::getCardValue(Card card) {
    int value = 0;
    Rank rank = card.getRank();

    if (strategy == "HiLo") {
        switch (rank) {
        case Rank::Two:
        case Rank::Three:
        case Rank::Four:
        case Rank::Five:
        case Rank::Six:
            value = 1;
            break;
        case Rank::Seven:
        case Rank::Eight:
        case Rank::Nine:
            value = 0;
            break;
        case Rank::Ten:
        case Rank::Jack:
        case Rank::Queen:
        case Rank::King:
        case Rank::Ace:
            value = -1;
            break;
        }
    }
    else if (strategy == "KO") {
        switch (rank) {
        case Rank::Two:
        case Rank::Three:
        case Rank::Four:
        case Rank::Five:
        case Rank::Six:
        case Rank::Seven:
            value = 1;
            break;
        case Rank::Eight:
        case Rank::Nine:
            value = 0;
            break;
        case Rank::Ten:
        case Rank::Jack:
        case Rank::Queen:
        case Rank::King:
        case Rank::Ace:
            value = -1;
            break;
        }
    }

    return value;
}

// Modifies the running count based on the Hi-Lo strategy
void Counter::modifyCountHiLo(Card card) {
    int value = getCardValue(card);
    count += value;
}

// Returns the current counting strategy
std::string Counter::getStrategy() {
    return strategy;
}

// Sets a new counting strategy
void Counter::setStrategy(std::string newStrategy) {
    strategy = newStrategy;
}
