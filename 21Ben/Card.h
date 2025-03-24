#pragma once
#include <string>

// Card rank enumeration, with explicit integer values
enum class Rank {
    Two = 2,
    Three = 3,
    Four = 4,
    Five = 5,
    Six = 6,
    Seven = 7,
    Eight = 8,
    Nine = 9,
    Ten = 10,
    Jack = 11,
    Queen = 12,
    King = 13,
    Ace = 14
};

// Card suit enumeration
enum class Suit {
    Clubs,
    Diamonds,
    Hearts,
    Spades
};

// Class representing a playing card
class Card {
private:
    Rank rank;   // Card rank (2–Ace)
    Suit suit;   // Card suit (Clubs, Diamonds, etc.)

public:
    // Constructor
    Card(Rank r, Suit s);

    // Accessors
    Rank getRank() const;
    Suit getSuit() const;

    // Returns a string representation (e.g., "Ace of Spades")
    std::string toString() const;
};
