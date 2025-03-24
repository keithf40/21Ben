#pragma once
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "Deck.h"
#include "Dealer.h"
#include "Player.h"
#include "Texture.h"
#include "OptimalPlay.h"  // For bot strategy

class Game {
public:
    enum class Option { DEAL, NONE };

    Game(float width, float height);

    // Draw the game screen including card images, messages, and action buttons.
    void draw(sf::RenderWindow& window);
    // Handle events (both mouse and keyboard) for human actions.
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);

    Option getSelectedOption() const;

    // Start a new round.
    void startNewRound();

private:
    sf::Font font;

    // UI Buttons:
    sf::Text dealButton;      // Shown when round is not active (for human).
    sf::Text hitButton;       // For human.
    sf::Text standButton;     // For human.
    sf::Text doubleButton;    // For human.
    sf::Text splitButton;     // For human.

    // Core game components.
    Deck deck;
    Dealer dealer;
    std::vector<Player> players;  // 7 players (index 3 is human)

    // The index of the human player.
    const int humanIndex = 3;

    bool roundInProgress;
    std::string message;

    // Message text (e.g., outcome messages)
    sf::Text messageText;

    // Vectors of sprites for displaying cards for each player.
    // We'll maintain a vector of vector of sprites – one vector per player.
    std::vector<std::vector<sf::Sprite>> playersCardSprites;
    // And one for the dealer.
    std::vector<sf::Sprite> dealerCardSprites;

    // Card dimensions (original size before scaling)
    const float cardWidth = 71.f;
    const float cardHeight = 96.f;
    const float cardMargin = 10.f;
    const float scaleFactor = 0.25f;  // Scale down to 25%

    // Screen dimensions.
    float screenWidth;
    float screenHeight;

    // Predefined positions for players’ hands.
    // We assume players 0,1,2 on the left, 3 (human) in center-bottom,
    // and 4,5,6 on the right.
    std::vector<sf::Vector2f> playerPositions;

    // Texture holder for card images.
    Texture textures;

    // Optimal play strategy (used for bot players).
    OptimalPlay optimalPlay;

    // Update display: update card sprites and message text.
    void updateDisplay();

    // Process dealer turn and resolve round outcomes for all players.
    void finishRound();

    // Automatically simulate bot moves.
    void simulateBotMoves();

    // Check if a given text object was clicked.
    bool isTextClicked(const sf::Text& text, sf::RenderWindow& window);

    // Helper: Returns the appropriate texture for a given card.
    sf::Texture& getCardTexture(const Card& card);
};

#endif // GAME_H
