#include <iostream>
#include <string>
#include "Counter.h"
#include <functional>
// Constructor: initializes count and default strategy
Counter::Counter() {
    count = 0;
    strategy = "None";
    stratFunc = None;
}

Counter::Counter(std::string strategy) {
    count = 0;
    setStrategy(strategy);
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
    else if (strategy == "HiOpt1") {
        stratFunc = HiOpt1;
    }
    else if (strategy == "HiOpt2") {
        stratFunc = HiOpt2;
    }
    else if (strategy == "Mentor") {
        stratFunc = Mentor;
    }
    else if (strategy == "Omega2") {
        stratFunc = Omega2;
    }
    else if (strategy == "Revere Point Count") {
        stratFunc = RPC;
    }
    else if (strategy == "Revere RAPC") {
        stratFunc = RAPC;
    }
    else if (strategy == "Revere 14 Count") {
        stratFunc = R14C;
    }
    else if (strategy == "Wong Halves") {
        stratFunc = WongHalves;
    }
    else if (strategy == "Zen Count") {
        stratFunc = Zen;
    }
    else if (strategy == "None") {
        stratFunc = None;
    }
    else stratFunc = None;
}

// Calculates bet size based on count, decks remaining, bankroll, and stealth mode
int Counter::getBet(int decksRemaining, int bankroll, int minbet, bool sleuthMode) {
    float trueCount = float(count) / float(decksRemaining);
    int bettingUnit = bankroll / 1000;
    int totalBetSize = bettingUnit * (trueCount - 1);

    // Minimum table bet
    if (totalBetSize < minbet) totalBetSize = minbet;

    // Cap bets if sleuth mode is on (avoid detection)
    if ((totalBetSize > bankroll / 4) && sleuthMode) {
        totalBetSize = bankroll / 4;
    }

    return totalBetSize;
}

// Different counting strategies, is set to func variable
int HiLo(Card& card) {
    switch (card.getRank()) {
    case Rank::Ace:
        return -1;
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
        return -1;
    }
    return 0;
}

int KO(Card& card) {
    switch (card.getRank()) {
    case Rank::Ace:
        return -1;

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
        return -1;
    }
    return 0;
}

int HiOpt1(Card& card) {
    switch (card.getRank()) {
    case Rank::Ace:
    case Rank::Two:
        return 0;

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
        return -1;
    }
    return 0;
}

int HiOpt2(Card& card) {
    switch (card.getRank()) {
    case Rank::Ace:
        return 0;

    case Rank::Two:
    case Rank::Three:
        return 1;

    case Rank::Four:
    case Rank::Five:
        return 2;

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
        return -2;
    }
    return 0;
}

int Mentor(Card& card) {
    switch (card.getRank()) {
    case Rank::Ace:
        return -1;

    case Rank::Two:
        return 1;

    case Rank::Three:
    case Rank::Four:
    case Rank::Five:
    case Rank::Six:
        return 2;

    case Rank::Seven:
        return 1;

    case Rank::Eight:
        return 0;

    case Rank::Nine:
        return -1;

    case Rank::Ten:
    case Rank::Jack:
    case Rank::Queen:
    case Rank::King:
        return -2;
    }
    return 0;
}

int Omega2(Card& card) {
    switch (card.getRank()) {
    case Rank::Ace:
        return 0;

    case Rank::Two:
    case Rank::Three:
        return 1;

    case Rank::Four:
    case Rank::Five:
    case Rank::Six:
        return 2;

    case Rank::Seven:
        return 1;

    case Rank::Eight:
        return 0;

    case Rank::Nine:
        return -1;

    case Rank::Ten:
    case Rank::Jack:
    case Rank::Queen:
    case Rank::King:
        return -2;
    }
    return 0;
}

int RPC(Card& card) {
    switch (card.getRank()) {
    case Rank::Ace:
        return -2;

    case Rank::Two:
        return 1;

    case Rank::Three:
    case Rank::Four:
    case Rank::Five:
    case Rank::Six:
        return 2;

    case Rank::Seven:
        return 1;

    case Rank::Eight:
    case Rank::Nine:
        return 0;

    case Rank::Ten:
    case Rank::Jack:
    case Rank::Queen:
    case Rank::King:
        return -2;
    }
    return 0;
}

int RAPC(Card& card) {
    switch (card.getRank()) {
    case Rank::Ace:
        return -4;

    case Rank::Two:
        return 2;

    case Rank::Three:
    case Rank::Four:
        return 3;

    case Rank::Five:
        return 4;

    case Rank::Six:
        return 3;

    case Rank::Seven:
        return 2;

    case Rank::Eight:
        return 0;

    case Rank::Nine:
        return -1;

    case Rank::Ten:
    case Rank::Jack:
    case Rank::Queen:
    case Rank::King:
        return -3;
    }
    return 0;
}

int R14C(Card& card) {
    switch (card.getRank()) {
    case Rank::Ace:
        return 0;

    case Rank::Two:
    case Rank::Three:
        return 2;

    case Rank::Four:
        return 3;

    case Rank::Five:
        return 4;

    case Rank::Six:
        return 2;

    case Rank::Seven:
        return 1;

    case Rank::Eight:
        return 0;

    case Rank::Nine:
        return -2;

    case Rank::Ten:
    case Rank::Jack:
    case Rank::Queen:
    case Rank::King:
        return -3;
    }
    return 0;
}
// these are supposed to be halved each but then int doesnt work so doubling should have a similar effect
int WongHalves(Card& card) {
    switch (card.getRank()) {
    case Rank::Ace:
        return -2;

    case Rank::Two:
        return 1;

    case Rank::Three:
    case Rank::Four:
        return 2;

    case Rank::Five:
        return 3;

    case Rank::Six:
        return 2;

    case Rank::Seven:
        return 1;

    case Rank::Eight:
        return 0;

    case Rank::Nine:
        return -1;

    case Rank::Ten:
    case Rank::Jack:
    case Rank::Queen:
    case Rank::King:
        return -2;
    }
    return 0;
}

int Zen(Card& card) {
    switch (card.getRank()) {
    case Rank::Ace:
        return -1;

    case Rank::Two:
    case Rank::Three:
        return 1;

    case Rank::Four:
    case Rank::Five:
    case Rank::Six:
        return 2;

    case Rank::Seven:
        return 1;

    case Rank::Eight:
    case Rank::Nine:
        return 0;

    case Rank::Ten:
    case Rank::Jack:
    case Rank::Queen:
    case Rank::King:
        return -2;
    }
    return 0;
}

int None(Card& card) {
    return 0;
}