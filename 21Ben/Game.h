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
#include "Texture.h"      // For card images
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
    sf::Text dealButton;      // Shown when round is not active.
    sf::Text hitButton;       // For human actions.
    sf::Text standButton;     // For human actions.
    sf::Text doubleButton;    // For human actions.
    sf::Text splitButton;     // For human actions.

    // Core game components.
    Deck deck;
    Dealer dealer;
    std::vector<Player> players;  // Total of 5 players: 4 bots and 1 human.

    // Human player index is now 4.
    const int humanIndex = 4;

    bool roundInProgress;
    std::string message;

    // Message text.
    sf::Text messageText;

    // Vectors of sprites for displaying players’ cards.
    std::vector<std::vector<sf::Sprite>> playersCardSprites;
    // Sprites for the dealer's cards.
    std::vector<sf::Sprite> dealerCardSprites;

    // Card dimensions.
    const float cardWidth = 71.f;
    const float cardHeight = 96.f;
    const float cardMargin = 10.f;
    const float scaleFactor = 0.25f;

    // Screen dimensions.
    float screenWidth;
    float screenHeight;

    // Predefined positions for each player's hand.
    // We'll set:
    // Bots:
    //   Index 0: top left
    //   Index 1: top right
    //   Index 2: bottom left
    //   Index 3: bottom right
    // Human:
    //   Index 4: bottom center
    std::vector<sf::Vector2f> playerPositions;

    // Texture holder.
    Texture textures;

    // Optimal play strategy.
    OptimalPlay optimalPlay;

    // For bot delay.
    int currentPlayerTurn;
    sf::Clock botClock;

    // Update display.
    void updateDisplay();

    // Process dealer turn and resolve outcomes.
    void finishRound();

    // Update bot moves.
    void updateBotMoves();

    // Check if text was clicked.
    bool isTextClicked(const sf::Text& text, sf::RenderWindow& window);

    // Get card texture.
    sf::Texture& getCardTexture(const Card& card);
};

#endif // GAME_H
