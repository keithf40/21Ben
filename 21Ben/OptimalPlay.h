#pragma once

#include <vector>
#include "Hand.h"

// Class that encapsulates Blackjack basic strategy and betting logic
class OptimalPlay {
private:
    // H = Hit, D = Double Down, S = Stand
    // Y = Split, N = Don't Split (SplitPair table only)

    // Hard total strategy table: [player total - 5 to 17+] x [dealer card 2–Ace (index 0–9)]
    std::vector<std::vector<char>> HardTotal = {
        {'H','H','H','H','H','H','H','H','H','H'}, // 5
        {'H','H','H','H','H','H','H','H','H','H'}, // 6
        {'H','H','H','H','H','H','H','H','H','H'}, // 7
        {'H','H','H','H','H','H','H','H','H','H'}, // 8
        {'H','H','H','H','H','H','H','H','H','H'}, // 9
        {'H','D','D','D','D','H','H','H','H','H'}, // 10
        {'D','D','D','D','D','D','D','D','H','H'}, // 11
        {'D','D','D','D','D','D','D','D','D','D'}, // 12
        {'H','H','S','S','S','H','H','H','H','H'}, // 13
        {'S','S','S','S','S','H','H','H','H','H'}, // 14
        {'S','S','S','S','S','H','H','H','H','H'}, // 15
        {'S','S','S','S','S','H','H','H','H','H'}, // 16
        {'S','S','S','S','S','H','H','H','H','H'}, // 17
        {'S','S','S','S','S','S','S','S','S','S'}, // 18+
        {'S','S','S','S','S','S','S','S','S','S'},
        {'S','S','S','S','S','S','S','S','S','S'},
        {'S','S','S','S','S','S','S','S','S','S'},
        {'S','S','S','S','S','S','S','S','S','S'},
    };

    // Soft total strategy table: [total with Ace counted as 11 (13 to 21)] x [dealer card 2–Ace]
    std::vector<std::vector<char>> SoftTotal = {
        {'H','H','H','D','D','H','H','H','H','H'}, // A+2 (13)
        {'H','H','H','D','D','H','H','H','H','H'}, // A+3
        {'H','H','D','D','D','H','H','H','H','H'}, // A+4
        {'H','H','D','D','D','H','H','H','H','H'}, // A+5
        {'H','D','D','D','D','H','H','H','H','H'}, // A+6
        {'D','D','D','D','D','S','S','H','H','H'}, // A+7
        {'S','S','S','S','D','S','S','S','S','S'}, // A+8
        {'S','S','S','S','S','S','S','S','S','S'}, // A+9+
        {'S','S','S','S','S','S','S','S','S','S'},
    };

    // Split pair strategy: [pair rank 2 to Ace (index 0–9)] x [dealer card 2–Ace]
    std::vector<std::vector<char>> SplitPair = {
        {'N','N','Y','Y','Y','Y','N','N','N','N'}, // 2s
        {'N','N','Y','Y','Y','Y','N','N','N','N'}, // 3s
        {'N','N','N','N','N','N','N','N','N','N'}, // 4s
        {'N','N','N','N','N','N','N','N','N','N'}, // 5s
        {'N','Y','Y','Y','Y','N','N','N','N','N'}, // 6s
        {'Y','Y','Y','Y','Y','Y','N','N','N','N'}, // 7s
        {'Y','Y','Y','Y','Y','Y','Y','Y','Y','Y'}, // 8s
        {'Y','Y','Y','Y','Y','N','Y','Y','N','N'}, // 9s
        {'N','N','N','N','N','N','N','N','N','N'}, // 10s
        {'Y','Y','Y','Y','Y','Y','Y','Y','Y','Y'}, // Aces
    };

public:
    // Returns the best move ('H', 'S', 'D', 'Y', 'N') based on hand and dealer's upcard
    char getMove(Hand& hand, Card dealerShowing);

    // Returns a bet amount based on running count, decks remaining, bankroll, and "sleuth" mode
    int getBet(int runningCount, int decksRemaining, int bankroll, bool sleuthMode);
};
