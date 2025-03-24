#include "OptimalPlay.h"

// Determines the optimal move based on the hand and the dealer's upcard
char OptimalPlay::getMove(Hand& hand, Card dealerShowing) {
    int totalHand = hand.getTotalValue();
    bool isSoft = hand.isSoft();

    // Normalize dealer card value: face cards → 10, Ace → 11
    int dealerCard = static_cast<int>(dealerShowing.getRank());
    if (dealerCard > 10 && dealerCard < 14) dealerCard = 10;
    if (dealerCard == 14) dealerCard = 11;

    // Convert dealerCard into column index (2–11 → 0–9)
    int dealerIndex = dealerCard - 2;

    // Pair strategy
    if (hand.isPair()) {
        int pairRank = static_cast<int>(hand.toString()[0]) - '0'; // Extract rank from string
        if (pairRank > 10 && pairRank < 14) pairRank = 10;
        if (pairRank == 1) pairRank = 11;
        int pairIndex = (totalHand / 2) - 2;
        return SplitPair[pairIndex][dealerIndex];
    }

    // Soft total strategy
    if (isSoft) {
        int softIndex = totalHand - 13;
        return SoftTotal[softIndex][dealerIndex];
    }

    // Hard total strategy
    int hardIndex = totalHand - 4;
    return HardTotal[hardIndex][dealerIndex];
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
