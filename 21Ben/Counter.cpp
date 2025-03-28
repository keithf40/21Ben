#include <iostream>
#include <string>
#include "Counter.h"
#include <functional>
// Constructor: initializes count and default strategy
Counter::Counter() {
    count = 0;
    strategy = "HiLo";
    stratFunc = HiLo;
}

// Returns the current running count
int Counter::getCount() const {
    return count;
}
//when the deck is shuffled the count needs to be reset
void Counter::resetCount() {
    count = 0;
}

// Modifies the running count based on the Hi-Lo strategy
void Counter::modifyCount(Card card) {
    count += stratFunc(card);
}

// Returns the current counting strategy
std::string Counter::getStrategy() {
    return strategy;
}

// Sets a new counting strategy
void Counter::setStrategy(std::string newStrategy) {
    count = 0;
    strategy = newStrategy;
    // to add a new strategy, just put its string name in the if statement
    // and then make a function thats not part of the class that gets the value
    if (strategy == "HiLo") {
        stratFunc = HiLo;
    }
    else if (strategy == "KO") {
        stratFunc = KO;
    }
}

// Different counting strategies, is set to func variable
int HiLo(Card& card) {
    switch (card.getRank()) {
    case Rank::Two:
    case Rank::Three:
    case Rank::Four:
    case Rank::Five:
    case Rank::Six:
        return 1;

    case Rank::Seven:
    case Rank::Eight:
    case Rank::Nine:
        return 0;

    case Rank::Ten:
    case Rank::Jack:
    case Rank::Queen:
    case Rank::King:
    case Rank::Ace:
        return -1;
    }
    return 0;
}

int KO(Card& card) {
    switch (card.getRank()) {
    case Rank::Two:
    case Rank::Three:
    case Rank::Four:
    case Rank::Five:
    case Rank::Six:
    case Rank::Seven:
        return 1;

    case Rank::Eight:
    case Rank::Nine:
        return 0;

    case Rank::Ten:
    case Rank::Jack:
    case Rank::Queen:
    case Rank::King:
    case Rank::Ace:
        return -1;
    }
    return 0;
}