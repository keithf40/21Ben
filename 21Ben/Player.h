#pragma once

#include <string>
#include <vector>
#include "Deck.h"
#include "Hand.h"
#include "Counter.h"

// Class representing a player in Blackjack with support for splitting hands.
class Player {
private:
    std::string name;              // Player's name
    std::vector<Hand> hands;       // Player's hands (for splits)
    int currentHandIndex;          // Index of the currently active hand
    long long balance;                   // Player's current money
    long long currentBet;                // Bet for each hand (assumed equal for simplicity)

public:
    // Constructor with player name and starting balance
    Player(const std::string& playerName, int startingBalance);

    // Resets player state for a new round (clears hands and starts with one hand)
    void reset();

    // Places a bet for the round (bet applies to each hand)
    void placeBet(long long amount);

    // Player draws a card from the deck for the current hand
    Card hit(Deck& deck, Counter& counter);

    // Player stands on the current hand (no action here; game advances hands)
    void stand();

    // Check if the current hand has busted
    bool isBusted() const;

    // Checks if the current hand has a natural blackjack
    bool hasBlackjack() const;

    // Adds winnings to balance
    void addWinnings(int amount);

    // Getters
    std::string getName() const;
    long long getBalance() const;
    long long getCurrentBet() const;
    int getTotalHands() const;

    // Returns a string describing all the player's hands and balance.
    std::string toString() const;

    // Returns reference to the current hand.
    Hand& getCurrentHand();

    // Returns const reference to the current hand.
    const Hand& getCurrentHand() const;

    // Returns vector of all hands.
    const std::vector<Hand>& getHands() const;

    // Splits the current hand if possible. Returns true if split occurred.
    bool split();

    // Advances to the next hand, if any. Returns true if advanced, false if no more hands.
    bool advanceHand();

    // Checks if all hands have been played.
    bool allHandsPlayed() const;

    void setBank(long long bank);

    bool doubleDown();

    long long totalWinnings(int dealerTotal, bool dealerBlackjack);
};
