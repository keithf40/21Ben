#include "OptimalPlay.h"

// Determines the optimal move based on the hand and the dealer's upcard
char OptimalPlay::getMove(Hand& hand, Card dealerShowing) {
    int totalHand = hand.getTotalValue();
    int dealerCard = dealerShowing.getValue();

    if (hand.isPair()) {
        char checkY = SplitPair[(totalHand / 2) - 2][dealerCard - 2];
        if (checkY == 'Y') return checkY;
    }
    if (hand.isSoft()) {
        return SoftTotal[totalHand - 11][dealerCard - 2];
    }
    return HardTotal[totalHand - 2][dealerCard - 2];
}

// Calculates bet size based on count, decks remaining, bankroll, and stealth mode
int OptimalPlay::getBet(int runningCount, int decksRemaining, int bankroll, bool sleuthMode) {
    int trueCount = runningCount / decksRemaining;
    int bettingUnit = bankroll / 1000;
    int totalBetSize = bettingUnit * (trueCount - 1);

    // Minimum table bet
    if (totalBetSize < 5) totalBetSize = 5;

    // Cap bets if sleuth mode is on (avoid detection)
    if (totalBetSize > bankroll / 4 && sleuthMode) {
        totalBetSize = bankroll / 4;
    }

    return totalBetSize;
}
