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
#include "Texture.h" // For card images

class Game {
public:
    enum class Option { DEAL, NONE };

    Game(float width, float height);

    // Draw the game screen including card images, messages, and action buttons.
    void draw(sf::RenderWindow& window);
    // Handle events including mouse clicks for game actions.
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);

    Option getSelectedOption() const;

    // Start a new round.
    void startNewRound();

private:
    sf::Font font;

    // UI Buttons:
    sf::Text dealButton;      // Shown when round is not active.
    sf::Text hitButton;       // Shown during an active round.
    sf::Text standButton;     // Shown during an active round.
    sf::Text doubleButton;    // Shown during an active round.
    sf::Text splitButton;     // Shown during an active round.

    // Core game components.
    Deck deck;
    Dealer dealer;
    Player player;

    bool roundInProgress;
    std::string message;

    // Message text (e.g., outcome messages)
    sf::Text messageText;

    // Vectors of sprites for displaying cards.
    std::vector<sf::Sprite> playerCardSprites;
    std::vector<sf::Sprite> dealerCardSprites;

    // Card dimensions (original size before scaling)
    const float cardWidth = 71.f;
    const float cardHeight = 96.f;
    const float cardMargin = 10.f;
    const float scaleFactor = 0.25f;  // Scale down to 25%

    // Screen dimensions.
    float screenWidth;
    float screenHeight;

    // Texture holder for card images.
    Texture textures;

    // Update display: update card sprites and message text.
    void updateDisplay();

    // Process dealer turn and resolve round outcome for all player hands.
    void finishRound();

    // Check if a given text object was clicked.
    bool isTextClicked(const sf::Text& text, sf::RenderWindow& window);

    // Helper: Returns the appropriate texture for a given card.
    sf::Texture& getCardTexture(const Card& card);
};

#endif // GAME_H
