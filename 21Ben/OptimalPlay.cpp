#include "OptimalPlay.h"

// Determines the optimal move based on the hand and the dealer's upcard
char OptimalPlay::getMove(Hand& hand, Card dealerShowing) {
    int totalHand = hand.getTotalValue();
    int dealerCard = dealerShowing.getValue();
    if (totalHand > 21) return 'B';
    if (hand.isPair()) {
        char checkY = SplitPair[(totalHand / 2) - 2][dealerCard - 2];
        if (checkY == 'Y') return checkY;
    }
    if (hand.isSoft()) {
        return SoftTotal[totalHand - 11][dealerCard - 2];
    }
    return HardTotal[totalHand][dealerCard - 2];
}


